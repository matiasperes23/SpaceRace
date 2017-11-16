/* 
 * File:   Terreno.h
 * Author: Germán_Ruiz
 *
 * Created on 11 de mayo de 2012, 14:32
 */

#ifndef TERRENO_H
#define	TERRENO_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include "math_3d.h"

class Terreno 
{
private:
    GLsizei anchoTerreno, largoTerreno;
    
    //Vertex Arrrays, texturas, normales y los indices para formar las primitivas
    GLfloat* verticesTerreno;
    GLfloat* normalesTerreno;
    GLfloat* texturasTerreno;
    GLuint* indicesTerreno;
    
    GLfloat** alturasVertices;
    
    int displayListID;
    
    Vector3f calcularNormal(int i, int j);
    Vector3f calcularNormalIndice(int indice);
    
    
    
public:
    Terreno(float altura);
    virtual ~Terreno();
    
    inline int getDisplayListID() { return this->displayListID; };
    GLfloat getAlturaTerreno(float x, float z, int nivel);
    int getAlturaAlTerreno(Vector3f pos_lander, GLfloat minY, int nivel);
    void colocarZonaDeAterrizaje(int,int);
    
    
};

#endif	/* TERRENO_H */

