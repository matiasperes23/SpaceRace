/* 
 * File:   NaveEnemiga.cpp
 * Author: bdi_usr
 * 
 * Created on 22 de mayo de 2012, 9:54
 */

#include "NaveEnemiga.h"


void NaveEnemiga::copiarArray(GLfloat* orig,GLfloat* &dest,int n)
{
    dest = new GLfloat[n];
    for(int i = 0; i < n; i++)
    {
        dest[i] = orig[i];
    }
}

void NaveEnemiga::switchLuces()
{
    if(lucesOn)
    {
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        glDisable(GL_LIGHT7);
        lucesOn = false;
    }
    else
    {
        glEnable(GL_LIGHT5);
        glEnable(GL_LIGHT6);
        glEnable(GL_LIGHT7);
        lucesOn = true;
    }
    
}

void NaveEnemiga::setearPosicionYVelocidad(int i,Vector3f &pos_lunar_lander)
{
    pos_enemy[i].Set(-50 + (rand() % 100) + pos_lunar_lander.x,30 + (rand() % 6),-50 + (rand() % 100)  + pos_lunar_lander.z);
        
    while(fabs(pos_enemy[i].x) < 10 && fabs(pos_enemy[i].z) < 10){
        pos_enemy[i].x = -50 + rand() % 100;
        pos_enemy[i].z = -50 + rand() % 100;
    }
    vel_enemy[i].Set(0,0,-20 + rand() % 40);

}

NaveEnemiga::NaveEnemiga(Vector3f &pos_lunar_lander) {
    int i;
    ModelManager::Inst()->LoadMesh("luna9.3ds",MODEL_LUNA9);
    
    pos_enemy_inicio.Set(0.0f,30.0f,-7.0f);
    
    for(i = 0; i < 3; i++){    
        setearPosicionYVelocidad(i,pos_lunar_lander);
        subiendo[i] = false;
        vel_subida[i] = 1 + rand() % 5;
    }

    altura_enemy[0] = 2;
    altura_enemy[1] = 0;
    altura_enemy[2] = -2;
    
    lucesOn = true;
    
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);
    glEnable(GL_LIGHT7);
    
    GLfloat position5[]      = {0,     4,    0,    1};
    copiarArray(position5,this->position5,4);
    GLfloat ambientLight5[]  = { 0.2f, 0.5f, 0.2f, 1.0f };
    copiarArray(ambientLight5,this->ambientLight5,4);
    GLfloat diffuseLight5[]  = { 0.0f, 1.0f, 0.0f, 1.0f };
    copiarArray(diffuseLight5,this->diffuseLight5,4);
    GLfloat specularLight5[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    copiarArray(specularLight5,this->specularLight5,4);
    GLfloat spot_direction5[]  = {0.0, -1.0, 0.0 };
    copiarArray(spot_direction5,this->spot_direction5,3);
    spot_cutoff5        = 18.0;
    spot_exponent5      = 5.0;
    
    GLfloat position6[]      = {0,     4,    0,    1};
    copiarArray(position6,this->position6,4);
    GLfloat ambientLight6[]  = { 0.5f, 0.2f, 0.2f, 1.0f };
    copiarArray(ambientLight6,this->ambientLight6,4);
    GLfloat diffuseLight6[]  = { 1.0f, 0.5f, 0.0f, 1.0f };
    copiarArray(diffuseLight6,this->diffuseLight6,4);
    GLfloat specularLight6[] = { 1.0f, 0.5f, 0.0f, 1.0f };
    copiarArray(specularLight6,this->specularLight6,4);
    GLfloat spot_direction6[]  = { 0.0, -1.0, 0.0 };
    copiarArray(spot_direction6,this->spot_direction6,3);
    spot_cutoff6        = 18.0;
    spot_exponent6      = 5.0;
    
    GLfloat position7[]      = {0,     4,    0,    1};
    copiarArray(position7,this->position7,4);
    GLfloat ambientLight7[]  = { 0.5f, 0.2f, 0.2f, 1.0f };
    copiarArray(ambientLight7,this->ambientLight7,4);
    GLfloat diffuseLight7[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
    copiarArray(diffuseLight7,this->diffuseLight7,4);
    GLfloat specularLight7[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    copiarArray(specularLight7,this->specularLight7,4);
    GLfloat spot_direction7[]  = { 0.0, -1.0, 0.0 };
    copiarArray(spot_direction7,this->spot_direction7,3);
    spot_cutoff7        = 18.0;
    spot_exponent7      = 5.0;
}

void NaveEnemiga::OnLoop(float timeElapsed,Vector3f &pos_lunar_lander,Vector3f &vel_lunar_lander)
{
    anguloInterno += 90.0f * timeElapsed;
    
    //anguloExterno += 30.0f * timeElapsed;
    
    for(i = 0; i < 3; i++){
        
        
        if(fabs(pos_enemy[i].x - pos_lunar_lander.x) > 100 || fabs(pos_enemy[i].z - pos_lunar_lander.z) > 100)
        {
            setearPosicionYVelocidad(i,pos_lunar_lander);
        }else if(       sqrt((pos_enemy[i].x - pos_lunar_lander.x)*(pos_enemy[i].x - pos_lunar_lander.x) + (pos_enemy[i].y + altura_enemy[i] - pos_lunar_lander.y)*(pos_enemy[i].y + altura_enemy[i] - pos_lunar_lander.y)
                        + (pos_enemy[i].z - pos_lunar_lander.z)*(pos_enemy[i].z - pos_lunar_lander.z)) < 4) //choque
        {
            vel_lunar_lander.x += vel_enemy[i].x;
            vel_lunar_lander.z += vel_enemy[i].z;
            vel_lunar_lander.y -= 30;
        }
        
        pos_enemy[i] += vel_enemy[i]*(timeElapsed);
        
        if(subiendo[i])
        {
            altura_enemy[i] += vel_subida[i]*timeElapsed;
            
            if(altura_enemy[i] >= 2)
                subiendo[i] = false;
        }
        else
        {
            altura_enemy[i] -= vel_subida[i]*timeElapsed;
            
            if(altura_enemy[i] <= -2)
                subiendo[i] = true;
        }
 
    }
}

void NaveEnemiga::OnRender(bool inicio)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    GLfloat value = 64.0;
    glMaterialfv(GL_FRONT,GL_SHININESS,&value);
    if(inicio)
        glTranslatef(pos_enemy_inicio.x,pos_enemy_inicio.y,pos_enemy_inicio.z);
    
    glPushMatrix();
    
    if(!inicio){
        
        glTranslatef(pos_enemy[0].x,pos_enemy[0].y,pos_enemy[0].z);
        //glRotatef(anguloExterno,0,1,0);
        glTranslatef(0.0f,altura_enemy[0],0.0f);
    }
    else
        glTranslatef(0.0f,2,-6.0f);
        
    
    
    
    glRotatef(-anguloInterno,0,1,0);
    if(lucesOn){
        glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight5);
        glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight5);
        glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight5);
        glLightfv(GL_LIGHT5, GL_POSITION, position5);
        glLightfv( GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction5 );
        glLightf(  GL_LIGHT5, GL_SPOT_CUTOFF , spot_cutoff5 );
        glLightf(  GL_LIGHT5, GL_SPOT_EXPONENT , spot_exponent5 );
        glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.1);
    }
    
    ModelManager::Inst()->Render(MODEL_LUNA9);
    glPopMatrix();
    
    glPushMatrix();
    
    if(!inicio){
        
        glTranslatef(pos_enemy[1].x,pos_enemy[1].y,pos_enemy[1].z);
        //glRotatef(anguloExterno,0,1,0);
        glTranslatef(0.0f,altura_enemy[1],0.0f);
    
    }
    else
        glTranslatef(-4.0f,0,0.0f);    
    
    
    glRotatef(anguloInterno,0,1,0);
    
    if(lucesOn){
        glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuseLight6);
        glLightfv(GL_LIGHT6, GL_AMBIENT, ambientLight6);
        glLightfv(GL_LIGHT6, GL_SPECULAR, specularLight6);
        glLightfv(GL_LIGHT6, GL_POSITION, position6);
        glLightfv( GL_LIGHT6, GL_SPOT_DIRECTION, spot_direction6 );
        glLightf(  GL_LIGHT6, GL_SPOT_CUTOFF , spot_cutoff6 );
        glLightf(  GL_LIGHT6, GL_SPOT_EXPONENT , spot_exponent6 );
        glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.1);
    }
    ModelManager::Inst()->Render(MODEL_LUNA9);
    glPopMatrix();
    
    if(!inicio){
        
        glTranslatef(pos_enemy[2].x,pos_enemy[2].y,pos_enemy[2].z);
        //glRotatef(anguloExterno,0,1,0);
        glTranslatef(0.0f,altura_enemy[2],0.0f);
    }
    else
        glTranslatef(4.0f,-2,0.0f);
    
    glRotatef(anguloInterno,0,1,0);
    if(lucesOn){
        glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuseLight7);
        glLightfv(GL_LIGHT7, GL_AMBIENT, ambientLight7);
        glLightfv(GL_LIGHT7, GL_SPECULAR, specularLight7);
        glLightfv(GL_LIGHT7, GL_POSITION, position7);
        glLightfv( GL_LIGHT7, GL_SPOT_DIRECTION, spot_direction7 );
        glLightf(  GL_LIGHT7, GL_SPOT_CUTOFF , spot_cutoff7 );
        glLightf(  GL_LIGHT7, GL_SPOT_EXPONENT , spot_exponent7 );
        glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.1);
    }
    ModelManager::Inst()->Render(MODEL_LUNA9);
}

NaveEnemiga::~NaveEnemiga() {
    
    delete[] position5; delete[] ambientLight5; delete[] diffuseLight5; delete[] specularLight5; delete[] spot_direction5;
    
    delete[] position6; delete[] ambientLight6; delete[] diffuseLight6; delete[] specularLight6; delete[] spot_direction6;
    
    delete[] position7; delete[] ambientLight7; delete[] diffuseLight7; delete[] specularLight7; delete[] spot_direction7;
    
}

