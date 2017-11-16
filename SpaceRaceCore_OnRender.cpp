//==============================================================================
#include "SpaceRaceCore.h"
#include "ModelManager.h"
#include "Terreno.h"
#include "Explosion.h"

//==============================================================================


GLfloat density = 0.3;
GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; 

void SpaceRaceCore::OnRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Then, we reset our GL_MODELVIEW matrix.
    
    if (facetado)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);
    
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if (lighting && !wireframe)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);
    
    if (texturas)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
    
    //Dibujo la skybox
    this->dibujarSkyBox();

    glEnable(GL_DEPTH_TEST);
    
    /*
    glPushAttrib(GL_TRANSFORM_BIT);
    GLint	viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
    glPopAttrib();
    
    // No tuve suerte
    if (!turboUsado && !Menu){
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(0,0,-10);
        //glRotatef(anguloSatelite*5,0,1,0);
        glScalef(0.1,0.1,0.1);
        ModelManager::Inst()->Render(MODEL_T);
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    }
    
    glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
     */
    
    //MOVIMIENTO DE LA CAMARA
    if(!crash)
    {
        if(camara_delantera)
        {
            glTranslatef(0.0f,3.0f,5.0f);
            glRotatef(yrot_lander, 1.0f, 0.0f, 0.0f);
            glRotatef(xrot_lander, 0.0f, 1.0f, 0.0f);
        }
        else
        {
            glTranslatef(0.0f,0.0f,-20.0f + yrot_lander/20);
            glRotatef(yrot_cam + yrot_lander/3, 1.0f, 0.0f, 0.0f);
            glRotatef(xrot_cam + xrot_lander/3, 0.0f, 1.0f, 0.0f);
        }
    }
    else
    {
        glTranslatef(0.0f,3.0f,-20.0f);
        glRotatef(yrot_cam, 1.0f, 0.0f, 0.0f);
        glRotatef(xrot_cam, 0.0f, 1.0f, 0.0f);
    }
    
    
    
    // LUNAR LANDER Y SATELITE
    this->dibujarLunarLanderYSatelite();
 
    glTranslatef(-pos_lander.x,-pos_lander.y,-pos_lander.z); // se translada el terreno dando la sensacion de movimiento del lunar lander
    
    glEnable (GL_FOG);
    glCullFace(GL_FRONT);
    
    // NAVE ENEMIGA
    this->dibujarNaveEnemiga();
    
    this->dibujarTerreno();
 
    if(!crash)
        this->dibujarPropulsor(Pause);
    
    if(modeloPropulsor->Expulsando())
       glEnable(GL_LIGHT2);
    else
       glDisable(GL_LIGHT2);
    
    if(crash)
        this->dibujarExplosion();
    
    this->dibujarPolvoLunar(Pause);
    
    glDisable(GL_FOG);
    glCullFace(GL_BACK);
    
    if(turbo){
        if(!Pause){
            glAccum(GL_MULT, 0.95);
            glAccum(GL_ACCUM,0.05);
        }
        glAccum(GL_RETURN,1.0);
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (aterrizaje)
    {
        if (nuevoJuego)
            this->dibujarFinJuego();
        else
            this->dibujarAterrizajeMensaje();
    }
    else if (Menu)
    {
        if (MenuAjustes)
            this->dibujarMenuAjustes();
        else if (Creditos)
            this->dibujarCreditos();
        else
            this->dibujarMenu();
    }
    else
        this->dibujarHud();
    
    SDL_GL_SwapBuffers();
}

void SpaceRaceCore::dibujarNaveEnemiga()
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    
    
    this->naveEnemiga->OnRender(Menu && nuevoJuego);
    
    
    
    glPopAttrib();
    glPopMatrix();
}

void SpaceRaceCore::dibujarLunarLanderYSatelite()
{
    
    glPushMatrix();
    glRotatef(anguloSatelite, 0.5f, 1.0f, -0.5f);
    glTranslatef(0.0f,15.0f,-5.0f);
    
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    
    ModelManager::Inst()->Render(MODEL_SATELITE);    // Renderizado del Satelite
    
    //Luz Satelite
    
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
    glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);
    //glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction );
    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
    glPopMatrix();
    
    
    
    glPushMatrix();
    glRotatef(yrot_lander, -1.0f, 0.0f, 0.0f);
    glRotatef(xrot_lander, 0.0f, 0.0f, -1.0f);
    
    this->dibujarLucesLunarLander();
    ModelManager::Inst()->Render(MODEL_LUNAR_LANDER); // Renderizado del Lunar Lander
    glPopAttrib();
    glPopMatrix();
}

void SpaceRaceCore::dibujarLucesLunarLander()
{
    
    //Luces rojas
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse0 );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position0 );
    glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction0 );
    glLightf(  GL_LIGHT0, GL_SPOT_CUTOFF , spot_cutoff0 );
    glLightf(  GL_LIGHT0, GL_SPOT_EXPONENT , spot_exponent0 );
    
    
    glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient0 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse0 );
    glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular0 );
    glLightfv( GL_LIGHT1, GL_POSITION, light_position1 );
    glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1 );
    glLightf(  GL_LIGHT1, GL_SPOT_CUTOFF , spot_cutoff0 );
    glLightf(  GL_LIGHT1, GL_SPOT_EXPONENT , spot_exponent0 );
    
    
    // Luz del propulsor
    glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse2 );
    glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular2 );
    glLightfv( GL_LIGHT2, GL_POSITION, light_position2 );
    glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2 );
    glLightf(  GL_LIGHT2, GL_SPOT_CUTOFF , spot_cutoff2 );
    glLightf(  GL_LIGHT2, GL_SPOT_EXPONENT , spot_exponent2 );
    
    
    // Luz Amarilla
    glLightfv( GL_LIGHT3, GL_DIFFUSE, light_diffuse3 );
    glLightfv( GL_LIGHT3, GL_SPECULAR, light_specular3 );
    glLightfv( GL_LIGHT3, GL_POSITION, light_position3 );
    glLightfv( GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction3 );
    glLightf(  GL_LIGHT3, GL_SPOT_CUTOFF , spot_cutoff3 );
    glLightf(  GL_LIGHT3, GL_SPOT_EXPONENT , spot_exponent3 );
    /*
    glBegin(GL_TRIANGLES);
    glVertex3f(light_position3[0]      ,light_position3[1]      ,light_position3[2]);
    glVertex3f(light_position3[0] + 0.5,light_position3[1] + 0.5,light_position3[2]);
    glVertex3f(light_position3[0] - 0.5,light_position3[1] + 0.5,light_position3[2]);
    glEnd();
    
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(6      ,-3      ,-6);
    glNormal3f(0,1,0);
    glVertex3f(-6,-3,-6);
    glNormal3f(0,1,0);
    glVertex3f(-6,-3,6);
    glNormal3f(0,1,0);
    glVertex3f(6,-3,6);
    glEnd();
    **/
}

void SpaceRaceCore::dibujarPropulsor(bool pause)
{
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    glCullFace(GL_BACK);

    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    glPushMatrix();
    
    // se evita que el propulsor se traslade con el terreno
    glTranslatef(pos_lander.x,pos_lander.y,pos_lander.z);
    // rota con el lunar lander
    glRotatef(yrot_lander, -1.0f, 0.0f, 0.0f);
    glRotatef(xrot_lander, 0.0f, 0.0f, -1.0f);
    // el propulsor gira con la camara
    glRotatef(-(xrot_cam + xrot_lander/3), 0.0f, 1.0f, 0.0f);
    
    // se debe dibujar por ultimo dado que se usa blend
    modeloPropulsor->Render(timeElapsed*0.005,turbo,pause);
    
    glPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

void SpaceRaceCore::dibujarPolvoLunar(bool pause)
{
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    glCullFace(GL_BACK);

    glEnable(GL_FOG);
    glDisable(GL_LIGHTING);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    glPushMatrix();
    
    polvoLunar->Render(timeElapsed,pos_lander.x,pos_lander.y,pos_lander.z,pause && !Menu);
    
    glPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}


void SpaceRaceCore::dibujarExplosion()
{
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    glCullFace(GL_BACK);

    glEnable(GL_FOG);
    glDisable(GL_LIGHTING);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    glPushMatrix();
    
    // se evita que el propulsor se traslade con el terreno
    glTranslatef(pos_lander.x,pos_lander.y,pos_lander.z);
    // rota con el lunar lander
    //glRotatef(yrot_lander, -1.0f, 0.0f, 0.0f);
    //glRotatef(xrot_lander, 0.0f, 0.0f, -1.0f);
    // el propulsor gira con la camara
    //glRotatef(-(xrot_cam + xrot_lander/3), 0.0f, 1.0f, 0.0f);
    //glRotatef(-yrot_cam, 1.0f, 0.0f, 0.0f);
    glRotatef(-xrot_cam, 0.0f, 1.0f, 0.0f);
    
    explosion->Render(timeElapsed,aterrizaje);
    
    glPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}


void SpaceRaceCore::dibujarSkyBox()
{ 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //Para ver de mas lejos las imagenes de la skybox
    gluPerspective(90, (GLfloat) anchoPantalla / (GLfloat) altoPantalla, 0.1, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(yrot_cam + yrot_lander/3, 1.0f, 0.0f, 0.0f);
    glRotatef(xrot_cam + xrot_lander/3, 0.0f, 1.0f, 0.0f); 

    
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
    if (displayListSkyBox == -1)
    {
        displayListSkyBox = glGenLists(1);
        glNewList(displayListSkyBox, GL_COMPILE);
        glCullFace(GL_FRONT);
        glColor4f(0,0,0.5,1);

        TextureManager::Inst()->BindTexture(TEXT_FONDO_FT);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
        // Dibujo el quad de adelante
        glBegin(GL_QUADS);
            glTexCoord2f(1, 1); glVertex3f( 1.5f, -1.5f, -1.5f );
            glTexCoord2f(0, 1); glVertex3f( -1.5f, -1.5f, -1.5f );
            glTexCoord2f(0, 0); glVertex3f( -1.5f, 1.5f, -1.5f );
            glTexCoord2f(1, 0); glVertex3f( 1.5f, 1.5f, -1.5f );
        glEnd();

        // Dibujo el de la izquierda
        TextureManager::Inst()->BindTexture(TEXT_FONDO_LF);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex3f(  1.5f, -1.5f,  1.5f );
            glTexCoord2f(1, 1); glVertex3f(  1.5f, -1.5f, -1.5f );
            glTexCoord2f(1, 0); glVertex3f(  1.5f,  1.5f, -1.5f );
            glTexCoord2f(0, 0); glVertex3f(  1.5f,  1.5f,  1.5f );
        glEnd();

        // El de atras
        TextureManager::Inst()->BindTexture(TEXT_FONDO_BK);
        glBegin(GL_QUADS);
            glTexCoord2f(1, 1); glVertex3f( -1.5f, -1.5f,  1.5f );
            glTexCoord2f(1, 0); glVertex3f(  1.5f, -1.5f,  1.5f );
            glTexCoord2f(0, 0); glVertex3f(  1.5f,  1.5f,  1.5f );
            glTexCoord2f(0, 1); glVertex3f( -1.5f,  1.5f,  1.5f );

        glEnd();

        // El de la derecha
        TextureManager::Inst()->BindTexture(TEXT_FONDO_RT);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex3f( -1.5f, -1.5f, -1.5f );
            glTexCoord2f(1, 1); glVertex3f( -1.5f, -1.5f,  1.5f );
            glTexCoord2f(1, 0); glVertex3f( -1.5f,  1.5f,  1.5f );
            glTexCoord2f(0, 0); glVertex3f( -1.5f,  1.5f, -1.5f );
        glEnd();

        // Arriba
        TextureManager::Inst()->BindTexture(TEXT_FONDO_UP);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex3f( -1.5f,  1.5f, -1.5f );
            glTexCoord2f(0, 0); glVertex3f( -1.5f,  1.5f,  1.5f );
            glTexCoord2f(1, 0); glVertex3f(  1.5f,  1.5f,  1.5f );
            glTexCoord2f(1, 1); glVertex3f(  1.5f,  1.5f, -1.5f );
        glEnd();

        // Abajo, no se si necesario
        TextureManager::Inst()->BindTexture(TEXT_FONDO_DN);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glNormal3f(0, 1, 0); glVertex3f( -1.5f, -1.5f, -1.5f );
            glTexCoord2f(0, 1); glVertex3f( -1.5f, -1.5f,  1.5f );
            glTexCoord2f(1, 1); glVertex3f(  1.5f, -1.5f,  1.5f );
            glTexCoord2f(1, 0); glVertex3f(  1.5f, -1.5f, -1.5f );
        glEnd();
        glCullFace(GL_BACK);
        glEndList();
    }
    
    glCallList(displayListSkyBox);
    
    // Restauro los bits de enable y la matriz
    glPopAttrib();
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void SpaceRaceCore::dibujarTerreno()
{
    //Dibujo el terreno llamando a la displayList
    glPushMatrix();
    TextureManager::Inst()->BindTexture(TEXT_LUNA);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    glRotatef(90 * (nivel - 1), 0, 1, 0);
    glColor4f(0.6,0.6,0.6,1);
    
    glCallList(terreno->getDisplayListID());
    
    // Lo replico, hay q ver esto
    glTranslatef(-153.6 * 2 + 1, 0, -153.6 * 2 + 1);
    glCallList(terreno->getDisplayListID());
    glTranslatef(153.6 * 2 - 1, 0, 0);
    glCallList(terreno->getDisplayListID());
    glTranslatef(153.6 * 2 - 1, 0, 0);
    glCallList(terreno->getDisplayListID());
    glTranslatef(0, 0, 153.6 * 2 - 1);
    glCallList(terreno->getDisplayListID());
    glTranslatef(-153.6 * 4 + 1, 0, 0);
    glCallList(terreno->getDisplayListID());
    glTranslatef(0, 0, 153.6 * 2 - 1);
    glCallList(terreno->getDisplayListID());
    glTranslatef(153.6 * 2 - 1, 0, 0);
    glCallList(terreno->getDisplayListID());
    glTranslatef(153.6 * 2 - 1, 0, 0);
    glCallList(terreno->getDisplayListID());
    glPopMatrix();

}

void SpaceRaceCore::dibujarHud()
{
    glPushMatrix();
    glLoadIdentity();
    int altura = terreno->getAlturaAlTerreno(pos_lander, min_Y_LunarLander, nivel);
    int fuel = (int) ((combustible * 100) / combustibleInicial);
    int velocidadX = (int) vel_lander.x;
    int velocidadY = (int) vel_lander.y;
    int velocidadZ = (int) vel_lander.z;
    
    glScalef(0.8,0.8,0.8);
    glColor4f(1,1,1,1);
    freetype::print(our_font, 5, altoPantalla -20, "altura: %i", altura);
    freetype::print(our_font, 5, altoPantalla -50, "combustible: %i%%", fuel);
    if (!turboUsado){
        freetype::print(our_font, 5, altoPantalla -80, "turbo disponible");
    }
    else
        freetype::print(our_font, 5, altoPantalla -80, "turbo no disponible");
    freetype::print(our_font, anchoPantalla/2 - 50, altoPantalla -20, "mision %i", nivel);
    freetype::print(our_font, anchoPantalla/2 - 50, altoPantalla -50, "score: %i", puntosTotal);
    
    freetype::print(our_font, anchoPantalla - 190, altoPantalla -20, "velocidad x: %i", velocidadX);
    freetype::print(our_font, anchoPantalla - 190, altoPantalla -50, "velocidad y: %i", velocidadY);
    freetype::print(our_font, anchoPantalla - 190, altoPantalla -80, "velocidad z: %i", -velocidadZ);
    
    glScalef(0.9,0.9,0.9);
    
    if (Pause)
        freetype::print(our_font, 5, 55, "p: reanudar");
    else
        freetype::print(our_font, 5, 55, "p: pausar");
    
    freetype::print(our_font, 5, 30, "esc: menu");
    freetype::print(our_font, 5, 5, "q: saliR");
    
    freetype::print(our_font, anchoPantalla - 180, 30, "space: propulsor");
    freetype::print(our_font, anchoPantalla - 180, 5, "space-t: turbo");
    freetype::print(our_font, anchoPantalla/2 - 70, 5, "l: luz satelite");
    freetype::print(our_font, anchoPantalla/2 - 70, 30, "f: luz nave");
    
    glPopMatrix();
}

void SpaceRaceCore::dibujarMenu()
{
    glPushMatrix();
    glLoadIdentity();
    
    glScalef(2,2,2);
    glColor4f(1,1,1,1);
    freetype::print(our_font, anchoPantalla/2 - 150, altoPantalla - 40, "SPACE RACE");
    
    glColor4f(0,0,1,1);
    freetype::print(our_font, 20, altoPantalla - 150, "menu");
    glScalef(0.5,0.5,0.5);
    
    if (posicionMenu == 0)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 220, "nuevo juego");
    if (posicionMenu == 1)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 270, "ajustes");
    if (posicionMenu == 2)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 320, "creditos");
    if (posicionMenu == 3)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 370, "salir");
    
    glPopMatrix();
}

void SpaceRaceCore::dibujarMenuAjustes()
{
    glPushMatrix();
    glLoadIdentity();
    
    glScalef(2,2,2);
    
    glColor4f(1,1,1,1);
    freetype::print(our_font, anchoPantalla/2 - 150, altoPantalla - 40, "SPACE RACE");
    
    glColor4f(0,0,1,1);
    freetype::print(our_font, 20, altoPantalla - 150, "ajustes");
    glScalef(0.5,0.5,0.5);
    
    if (posicionMenu == 0)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    switch (velocidadJuego)
    {
        case 0:
        {
            freetype::print(our_font, 30, altoPantalla - 210, "velocidad lento");
            break;
        }
        case 1:
        {
            freetype::print(our_font, 30, altoPantalla - 210, "velocidad normal");
            break;
        }
        case 2:
            freetype::print(our_font, 30, altoPantalla - 210, "velocidad rapido");
    }
    
    if (posicionMenu == 1)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    if (wireframe)
        freetype::print(our_font, 30, altoPantalla - 250, "wireframe on");
    else
        freetype::print(our_font, 30, altoPantalla - 250, "wireframe off");
    
    if (posicionMenu == 2)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    if (facetado)
        freetype::print(our_font, 30, altoPantalla - 290, "facetado");
    else
        freetype::print(our_font, 30, altoPantalla - 290, "interpolado");
    
    if (posicionMenu == 3)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    if (texturas)
        freetype::print(our_font, 30, altoPantalla - 330, "texturas on");
    else
        freetype::print(our_font, 30, altoPantalla - 330, "texturas off");
    
    if (posicionMenu == 4)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    if (lighting)
        freetype::print(our_font, 30, altoPantalla - 370, "luces on");
    else
        freetype::print(our_font, 30, altoPantalla - 370, "luces off");
    
    if (posicionMenu == 5)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 410, "color luz satelite %i", colorLuz);
    
    if (posicionMenu == 6)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 450, "posicion luz satelite %i", posicionLuz);
    
    if (posicionMenu == 7)
        glColor4f(1,0,0,1);
    else
        glColor4f(1,1,1,1);
    if (altura == 20)
        freetype::print(our_font, 30, altoPantalla - 490, "terreno plano");
    else if (altura == 50)
        freetype::print(our_font, 30, altoPantalla - 490, "terreno normal");
    else
        freetype::print(our_font, 30, altoPantalla - 490, "terreno escarpado");
    
    glPopMatrix();
}

void SpaceRaceCore::dibujarCreditos()
{
    glPushMatrix();
    glLoadIdentity();
    
    glScalef(2,2,2);
    
    glColor4f(1,1,1,1);
    freetype::print(our_font, anchoPantalla/2 - 150, altoPantalla - 40, "SPACE RACE");
    
    glScalef(0.5,0.5,0.5);
    glColor4f(0,0,1,1);
    freetype::print(our_font, 20, altoPantalla - 150, "desarrollado por");
    glScalef(0.9,0.9,0.9);
    glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 220, "german ruiz");
    freetype::print(our_font, 30, altoPantalla - 250, "y");
    freetype::print(our_font, 30, altoPantalla - 280, "matias fabrizio peres");
    
    glScalef(0.8,0.8,0.8);
    glColor4f(1,0,0,1);
    freetype::print(our_font, 30, altoPantalla - 320, "durante mayo del 2012");
    
    glScalef(2,2,2);
    glScalef(0.5,0.5,0.5);
    glColor4f(0,0,1,1);
    freetype::print(our_font, 20, altoPantalla - 380, "agradecemos a");
    glScalef(0.9,0.9,0.9);
    glColor4f(1,1,1,1);
    freetype::print(our_font, 30, altoPantalla - 420, "fing");
    freetype::print(our_font, 30, altoPantalla - 460, "club nacional de futbol");
    freetype::print(our_font, 30, altoPantalla - 500, "bdi");
    
    glPopMatrix();
}


void SpaceRaceCore::dibujarAterrizajeMensaje()
{
    glPushMatrix();
    glLoadIdentity();
    
    glColor4f(1,0,0,1);
    if (!crash)
         freetype::print(our_font, 20, altoPantalla - 80, "mision %i cumplida", nivel);
    else
         freetype::print(our_font, 20, altoPantalla - 80, "mision %i fallida", nivel);
        
    
    freetype::print(our_font, 20, altoPantalla - 120, "score: %i", puntos);
    
    glScalef(0.8,0.8,0.8);
    glColor4f(0,0,1,1);
    freetype::print(our_font, anchoPantalla/2 + 50, altoPantalla/2 - 280, "presione enter para continuar");
    
    glPopMatrix();
}

void SpaceRaceCore::dibujarFinJuego()
{
    glPushMatrix();
    glLoadIdentity();
    
    glColor4f(1,0,0,1);
    freetype::print(our_font, 20, altoPantalla - 80, "juego terminado");
        
    
    freetype::print(our_font, 20, altoPantalla - 120, "score final: %i", puntosTotal);
    
    glScalef(0.8,0.8,0.8);
    glColor4f(0,0,1,1);
    freetype::print(our_font, anchoPantalla/2 + 50, altoPantalla/2 - 280, "presione enter para continuar");
    
    glPopMatrix();
    
}


//==============================================================================
