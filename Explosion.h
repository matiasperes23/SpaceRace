/* 
 * File:   Explosion.h
 * Author: bdi_usr
 *
 * Created on 21 de mayo de 2012, 7:22
 */

#ifndef EXPLOSION_H
#define	EXPLOSION_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "TextureManager.h"
#include "Claves.h"
#include "math.h"

#include "Particle.h"

#include <SDL/SDL_opengl.h>


using namespace std;

#define MAX_PARTICLES_EX 30000

class Explosion {
public:
    Explosion();
    
    void Render(float timeElapsed,bool choque);
    
    virtual ~Explosion();
private:
    Particle particles[MAX_PARTICLES_EX];
    float ini_x,ini_y,ini_z;
    float ladoPart;
    void resetParticle(int num);

};

#endif	/* EXPLOSION_H */

