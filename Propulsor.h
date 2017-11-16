/* 
 * File:   Propulsor.h
 * Author: bdi_usr
 *
 * Created on 18 de mayo de 2012, 6:21
 */

#ifndef PROPULSOR_H
#define	PROPULSOR_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "TextureManager.h"
#include "Claves.h"
#include "math.h"

#include "Particle.h"

#include <SDL/SDL_opengl.h>


using namespace std;

#define MAX_PARTICLES 10000

class Propulsor {
public:
    
    Propulsor();
    
    void Render(float timeElapsed,bool turbo,bool pause);
    
    virtual ~Propulsor();

    void Desactivar();
    
    void Activar();
    
    bool Expulsando();
    
    void ReiniciarParticulas();
    
private:
    
    Particle particles[MAX_PARTICLES];
    bool activo;
    bool apagado;
    bool particulasVisibles;
    float ini_x,ini_y,ini_z;
    float ladoPart;
    void resetParticle(int num);
};

#endif	/* PROPULSOR_H */

