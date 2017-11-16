/* 
 * File:   Terreno.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 11 de mayo de 2012, 14:32
 */

#include <Assimp/aiVector3D.h>

#include "Terreno.h"


Terreno::Terreno(float altura)
{
    SDL_Surface* surf_terreno = IMG_Load("terreno.png");
    
    anchoTerreno = surf_terreno->w;
    largoTerreno = surf_terreno->h;
    //Reservo la memoria de los arrays para el terreno ahora que se el ancho y alto de la imagen
    //3 coordenas (x, y, z), y anchoTerreno*altoTerreno vertices ya que la imagen del terreno es de anchoTerreno*altoTerreno pixeles
    verticesTerreno = new GLfloat[3 * anchoTerreno * largoTerreno];
    alturasVertices = new GLfloat*[anchoTerreno];
    for (int i = 0; i < anchoTerreno ; i++)
        alturasVertices[i] = new GLfloat[largoTerreno];
    normalesTerreno = new GLfloat[3 * anchoTerreno * largoTerreno];
    texturasTerreno = new GLfloat[2 * anchoTerreno * largoTerreno];
    //(anchoTerreno)*2 indices para cada quad_strip, tengo (altoTerreno - 1) quads_strips
    indicesTerreno  = new GLuint[2 * anchoTerreno * (largoTerreno - 1)];
    
    //Primero hay que guardar todos los vertices en un array para luego poder usar las ventajas de vertex arrays
    //Ademas usar displayList para mayor eficiencia
    GLfloat xVertice, yVertice, zVertice;
    /* Lock de la superficie */
    SDL_LockSurface(surf_terreno);
    //La profundidad de cada pixel es de 8 bits
    Uint8* pixels = (Uint8*) surf_terreno->pixels;
    int coordVert = -1;
    for(int i = 0; i < largoTerreno; i++)
    {
        for(int j = 0; j < anchoTerreno; j++)
        {
            int indice = (i * anchoTerreno) + j;
            xVertice = -153.6 + j*0.3;
            yVertice = ((pixels[indice] * altura) / 255.0) - altura/2; //de -altura/2 a altura/2, segun el color del pixel
            zVertice = -153.6 + i*0.3; 
            verticesTerreno[++coordVert] = xVertice;
            verticesTerreno[++coordVert] = yVertice;
            verticesTerreno[++coordVert] = zVertice;
        }
    }
    SDL_UnlockSurface(surf_terreno);
    SDL_FreeSurface(surf_terreno);
    
    //Pongo unas zona de aterrizaje
    colocarZonaDeAterrizaje(10, 220);
    colocarZonaDeAterrizaje(-20, 300);
    colocarZonaDeAterrizaje(-200, 50);
    colocarZonaDeAterrizaje(200, 50);
    colocarZonaDeAterrizaje(100, 100);
    colocarZonaDeAterrizaje(-100, -100);
    colocarZonaDeAterrizaje(-100, -10);
    colocarZonaDeAterrizaje(-100, -200);
    colocarZonaDeAterrizaje(150, -100);
    colocarZonaDeAterrizaje(130, -200);
    
    int vertY = 1;
    for(int i = 0; i < largoTerreno; i++)
    {
        for(int j = 0; j < anchoTerreno; j++)
        {
            alturasVertices[j][i] = verticesTerreno[vertY];
            vertY += 3;
        }
    }
    
    //Armo el arreglo de normales para el terreno
    int coordNorm = -1;
    for(int i = 0; i < largoTerreno; i++)
    {
        for(int j = 0; j < anchoTerreno; j++)
        {
            Vector3f normal = calcularNormal(i,j);
            normalesTerreno[++coordNorm] = normal.x;
            normalesTerreno[++coordNorm] = normal.y;
            normalesTerreno[++coordNorm] = normal.z;
        }
    }
    

    //Armo el arreglo de texturas del terreno
    int coordText = -1;
    for(int i = 0; i < largoTerreno; i++)
    {
        for(int j = 0; j < anchoTerreno; j++)
        {
            texturasTerreno[++coordText] = j*0.02;
            texturasTerreno[++coordText] = i*0.02;
        }
    }
    
    //Armo el arreglo de indices
    //para usar GL_QUAD_STRIPS
    int indVert = -1;
    for(int i = 0; i < largoTerreno - 1; i++)
    {
        for(int j = 0; j < anchoTerreno - 1; j = j + 2)
        {
            int indice1 = ((i + 1) * anchoTerreno) + j;
            int indice2 = (i * anchoTerreno) + j;
            indicesTerreno[++indVert] = indice1;
            indicesTerreno[++indVert] = indice2;
            indicesTerreno[++indVert] = indice1 + 1;
            indicesTerreno[++indVert] = indice2 + 1;
        }
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT , 0, verticesTerreno);
    glNormalPointer(GL_FLOAT, 0, normalesTerreno);
    glTexCoordPointer(2, GL_FLOAT, 0, texturasTerreno);
    
    int cantQuadStrips = (largoTerreno - 1);
    
    //Creo un display list para el dibujado del terreno
    displayListID = glGenLists(1);
    glNewList(displayListID, GL_COMPILE);
    for(int i = 0; i < cantQuadStrips; i++)
    {
        glDrawElements(GL_QUAD_STRIP, anchoTerreno * 2, GL_UNSIGNED_INT, &indicesTerreno[anchoTerreno*2*i]);
    }
    glEndList();
}

Terreno::~Terreno() 
{
    delete [] this->verticesTerreno;
    delete [] this->texturasTerreno;
    delete [] this->indicesTerreno;
}

GLfloat Terreno::getAlturaTerreno(float x, float z, int nivel)
{
    if (x < -153.6)
        x += 153.6 * 2;
    else if (x > 153.6)
        x -= 153.6 * 2;
    if (z < -153.6)
        z += 153.6 * 2;
    else if (z > 153.6)
        z -= 153.6 * 2;
    int tmp;
    if (nivel == 2)
    {
        tmp = x;
        x = -z;
        z = tmp;
    }
    else if (nivel == 3)
    {
        z = -z;
        x = -x;
    }
    else if (nivel == 4)
    {
        tmp = x;
        x = z;
        z = -tmp;
    }
               
    int verticeX = ((int) (x / 0.3) + anchoTerreno/2) % anchoTerreno;
    int verticeZ = ((int) (z / 0.3) + largoTerreno/2) % largoTerreno;
    return (this->alturasVertices[verticeX][verticeZ]);
}

int Terreno::getAlturaAlTerreno(Vector3f pos_lander, GLfloat minY, int nivel)
{
    GLfloat alturaTerreno = this->getAlturaTerreno(pos_lander.x, pos_lander.z, nivel);
    GLfloat alturaNave = pos_lander.y + minY;
    return (int) (alturaNave - alturaTerreno);
}

Vector3f Terreno::calcularNormal(int i, int j)
{
    int indice = ((i * anchoTerreno) + j) * 3;
    if (i == 0)
    {
        if (indice == 0)
            return calcularNormalIndice(indice);
        else if (indice == (3* (anchoTerreno - 1)))
            return calcularNormalIndice(indice - 3);
        else
            return (calcularNormalIndice(indice - 3) + calcularNormalIndice(indice)) * 0.5;
    }
    else if (i == (largoTerreno - 1))
    {   
        if (j == 0)
            return calcularNormalIndice(indice - (anchoTerreno) * 3);
        else if (j == (anchoTerreno - 1))
            return calcularNormalIndice(indice - 3 - (anchoTerreno) * 3);
        else
            return (calcularNormalIndice(indice - (anchoTerreno) * 3) + calcularNormalIndice(indice - 3 - (anchoTerreno) * 3)) * 0.5;
    }
    else
    {
        if (j == 0)
            return (calcularNormalIndice(indice - (anchoTerreno) * 3) + calcularNormalIndice(indice)) * 0.5;
        else if (j == (anchoTerreno - 1))
            return (calcularNormalIndice(indice - 3 - (anchoTerreno) * 3) + calcularNormalIndice(indice - 3)) * 0.5;
        else
            return (calcularNormalIndice(indice - (anchoTerreno) * 3) + calcularNormalIndice(indice - 3 - (anchoTerreno) * 3)
                    + calcularNormalIndice(indice - 3) + calcularNormalIndice(indice)) * 0.25; 
    }  
}

Vector3f Terreno::calcularNormalIndice(int indice)
{
    Vector3f vertice1(verticesTerreno[indice], verticesTerreno[indice + 1], verticesTerreno[indice + 2]);
    indice += 3;
    Vector3f vertice2(verticesTerreno[indice], verticesTerreno[indice + 1], verticesTerreno[indice + 2]);
    indice += 3 * (anchoTerreno - 1);
    Vector3f vertice3(verticesTerreno[indice], verticesTerreno[indice + 1], verticesTerreno[indice + 2]);
    Vector3f vector1 = vertice2 - vertice1;
    Vector3f vector2 = vertice3 - vertice1;
    return vector1.Cross(vector2); //Producto Vectorial
}

void Terreno::colocarZonaDeAterrizaje(int x, int z)
{
    int limite1 = (largoTerreno / 2) - z;
    int limite2 = (anchoTerreno / 2) + x;
    //Calculo la altura promedio de los alrededores
    int indiceSupIzq = (limite1 * anchoTerreno) + limite2;
    int indiceSupDer = indiceSupIzq + 80;
    int indiceInfIzq = ((limite1 + 70) * anchoTerreno) + limite2;
    int indiceInfDer = indiceInfIzq + 80;
    float alturaPromedio = (verticesTerreno[(indiceSupIzq * 3) + 1] + verticesTerreno[(indiceSupDer * 3) + 1] +
                            verticesTerreno[((indiceSupIzq + 25) * 3) + 1] + verticesTerreno[((indiceInfIzq + 25) * 3) + 1] +
                            verticesTerreno[((indiceSupIzq + 15 * anchoTerreno) * 3) + 1] + verticesTerreno[((indiceSupDer + 15 * anchoTerreno) * 3) + 1] +
                            verticesTerreno[(indiceInfIzq * 3) + 1] + verticesTerreno[(indiceInfDer * 3) + 1]) / 8.0;
    
    for (int i = limite1; i < limite1 + 50; i++)
    {
        for (int j = limite2; j < limite2 + 50; j++)
        {
            int indice = (i * anchoTerreno) + j;
            verticesTerreno[indice * 3 + 1] = alturaPromedio;
        }
    }
}
