/* 
 * File:   PolvoLunar.h
 * Author: bdi_usr
 *
 * Created on 20 de mayo de 2012, 0:24
 */

#ifndef POLVOLUNAR_H
#define	POLVOLUNAR_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "TextureManager.h"
#include "Particle.h"
#include "Claves.h"
#include <SDL/SDL_opengl.h>

using namespace std;

#define MAX_PARTICLES_DUST 200


class PolvoLunar {
public:
    
    PolvoLunar();
    
    void Render(float timeElapsed,float x, float y, float z, bool pause);
    
    virtual ~PolvoLunar();
    
private:
    void resetParticle(int num);
    Particle particles[MAX_PARTICLES_DUST];
    float ini_x,ini_y,ini_z;
    float ladoPart;
};

#endif	/* POLVOLUNAR_H */

