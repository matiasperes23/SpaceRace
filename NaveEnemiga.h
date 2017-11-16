/* 
 * File:   NaveEnemiga.h
 * Author: bdi_usr
 *
 * Created on 22 de mayo de 2012, 9:54
 */

#ifndef NAVEENEMIGA_H
#define	NAVEENEMIGA_H

#include "Claves.h"
#include "math_3d.h"
#include "ModelManager.h"
#include "math.h"

class NaveEnemiga {
public:
    NaveEnemiga(Vector3f &pos_lunar_lander);
    
    void OnLoop(float timeElapsed,Vector3f &pos_lunar_lander,Vector3f &vel_lunar_lander);
    
    void switchLuces();
    
    void OnRender(bool inicio);
    
    virtual ~NaveEnemiga();
private:
    void setearPosicionYVelocidad(int i,Vector3f &pos_lunar_lander);
    
    void copiarArray(GLfloat* orig,GLfloat* &dest,int n);
    
    bool lucesOn;
    int i;
    Vector3f pos_enemy_inicio; 
    Vector3f pos_enemy[3];
    Vector3f vel_enemy[3];
    float altura_enemy[3];
    bool subiendo[3];
    float vel_subida[3];
    float anguloInterno,anguloExterno;
    
    GLfloat  spot_cutoff5, spot_exponent5;
    GLfloat *position5, *ambientLight5, *diffuseLight5, *specularLight5, *spot_direction5;
    
    GLfloat  spot_cutoff6, spot_exponent6;
    GLfloat *position6, *ambientLight6, *diffuseLight6, *specularLight6, *spot_direction6;
    
    GLfloat  spot_cutoff7, spot_exponent7;
    GLfloat *position7, *ambientLight7, *diffuseLight7, *specularLight7, *spot_direction7;
};

#endif	/* NAVEENEMIGA_H */

