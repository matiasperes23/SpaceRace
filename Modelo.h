/* 
 * File:   Modelo.h
 * Author: bdi_usr
 *
 * Created on 10 de mayo de 2012, 4:26
 */

#ifndef MODELO_H
#define	MODELO_H

#include <GL/gl.h>

#include <Assimp/assimp.h> 
#include <Assimp/aiScene.h>
#include <Assimp/aiPostProcess.h>

class Modelo {
public:
    Modelo(const aiScene* scene);
    virtual ~Modelo();
    void SetAiVector3d(aiVector3D* scene_min,aiVector3D* scene_max,aiVector3D* scene_center);
    void SetFactorEscalamiento(GLfloat);
    void SetDisplayList(GLuint list); 
    void Render();
    GLuint GetDisplayList(); 
    
    inline GLfloat getModelo_minY() { return this->scene_min->y * factorDeEscalamiento; }
    inline GLfloat getModelo_minZ() { return this->scene_min->z * factorDeEscalamiento; }
    inline GLfloat getModelo_minX() { return this->scene_min->x * factorDeEscalamiento; }
private:
    
    // the global Assimp scene object
    const struct aiScene* scene;
    GLuint scene_list; 
    aiVector3D *scene_min, *scene_max, *scene_center;
    
    GLfloat factorDeEscalamiento;
    
};

#endif	/* MODELO_H */

