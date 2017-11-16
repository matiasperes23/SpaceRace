/* 
 * File:   Modelo.cpp
 * Author: bdi_usr
 * 
 * Created on 10 de mayo de 2012, 4:26
 */

#include "Modelo.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


Modelo::Modelo(const aiScene* scene) {
    this->scene = scene;
}

void Modelo::SetAiVector3d(aiVector3D* scene_min, aiVector3D* scene_max, aiVector3D* scene_center) {
    this->scene_min = scene_min;
    this->scene_max = scene_max;
    this->scene_center = scene_center;
}

void Modelo::SetFactorEscalamiento(GLfloat factor)
{
    float tmp = scene_max->x - scene_min->x;
    tmp = aisgl_max(scene_max->y - scene_min->y, tmp); 
    tmp = aisgl_max(scene_max->z - scene_min->z, tmp); 
    factorDeEscalamiento = factor / tmp;
}

void Modelo::SetDisplayList(GLuint list) {
    this->scene_list = list;
}

void Modelo::Render() 
{
    glPushMatrix();
    glScalef(factorDeEscalamiento, factorDeEscalamiento, factorDeEscalamiento);
  
    // center the model
    glTranslatef( -scene_center->x, -scene_center->y, -scene_center->z );

    glCallList(scene_list);
    glPopMatrix();
}
GLuint Modelo::GetDisplayList()
{
    return this->scene_list;
}

Modelo::~Modelo() {
    aiReleaseImport(scene);
}

