/* 
 * File:   Propulsor.cpp
 * Author: bdi_usr
 * 
 * Created on 18 de mayo de 2012, 6:21
 */

#include "Propulsor.h"



void Propulsor::resetParticle(int num){
    /* Give the particles life */
    particles[num].life = 1.0f;
    /* Random Fade Speed */
    particles[num].fade = (float) (rand() % 100) / 1000.0f + 0.06f;
    /* Select Red Rainbow Color */
    
    particles[num].dt = 0;
    
    particles[num].r = 0.6f;
    /* Select Green Rainbow Color */
    particles[num].g = 0.733f;
    /* Select Blue Rainbow Color */
    particles[num].b = 0.815f;
   /* Set the position on the X axis */
    particles[num].x = ((float) (rand() % 500)) / 7000.0f;
    /* Set the position on the Y axis */
    particles[num].y = ((float) (rand() % 500)) / 7000.0f;
    /* Set the position on the Z axis */
    particles[num].z = ((float) (rand() % 500)) / 7000.0f;
    /* Random Speed On X Axis */
    particles[num].xi = 0;
    /* Random Speed On Y Axi */
    particles[num].yi = 0;
    /* Random Speed On Z Axis */
    particles[num].zi = 0;
    /* Set Horizontal Pull To Zero */
    particles[num].xg = (particles[num].x - 0.035)*3000;
    /* Set Vertical Pull Downward */
    particles[num].yg = -1000.0f + 0.01 * fabs(particles[num].xg) ;
    /* Set Pull On Z Axis To Zero */
    particles[num].zg = (particles[num].z -  0.035)*3000;
 
}


Propulsor::Propulsor() {
    
    activo      = false;
    apagado     = true;
    ini_x       = 0.95;
    ini_y       = 0.1f;
    ini_z       = 0.8f;
    ladoPart    = 0.2f;
    
    TextureManager::Inst()->LoadTextureAlphaTexture("circle.bmp", "circlealpha.bmp",TEXT_PARTICULA);
    //TextureManager::Inst()->LoadTextureBMP("particle.bmp",      TEXT_PARTICULA);
}

void Propulsor::Render(float timeElapsed,bool turbo,bool pause) {
    
    TextureManager::Inst()->BindTexture(TEXT_PARTICULA);
    
    glTranslatef(-1.0f + ini_x , -1.0f + ini_y , -1.0 + ini_z );
        
    particulasVisibles = false;

    if (activo || !apagado) {
        
        vector<Particle*> ps; 
        for(int i = 0; i < MAX_PARTICLES; i++) {
                ps.push_back(particles + i);
        }
        sort(ps.begin(), ps.end(), compareParticles);
        
        for(unsigned int i = 0; i < ps.size(); i++) {
            Particle* p = ps[i]; 
            if (p->life > 0) {
                
                
                
                particulasVisibles = true;

                /* Draw The Particle Using Our RGB Values,
                 * Fade The Particle Based On It's Life
                 */
                
               
                if( turbo )
                        glColor4f(1* (p->life), p->g * (p->life), 0,
                        p->life);
                else
                        glColor4f(p->r * (p->life), p->g * (p->life), p->b * p->life * 3,
                        p->life);

                
                /* Build Quad From A Triangle Strip */
                glBegin(GL_TRIANGLE_STRIP);
                /* Top Right */
                glTexCoord2d(1, 1);
                glVertex3f(p->x + ladoPart, p->y + ladoPart, p->z + ladoPart);
                /* Top Left */
                glTexCoord2d(0, 1);
                glVertex3f(p->x - ladoPart, p->y + ladoPart, p->z + ladoPart);
                /* Bottom Right */
                glTexCoord2d(1, 0);
                glVertex3f(p->x + ladoPart, p->y - ladoPart, p->z + ladoPart);
                /* Bottom Left */
                glTexCoord2d(0, 0);
                glVertex3f(p->x - ladoPart, p->y - ladoPart, p->z + ladoPart);
                glEnd();
                
                
                if(!pause){

                /* Move On The X Axis By X Speed */
                p->x += p->xi * p->dt + p->xg * (0.5f) * p->dt * p->dt;
                /* Move On The Y Axis By Y Speed */
                p->y += p->yi * p->dt + p->yg * (0.5f) * p->dt * p->dt;
                /* Move On The Z Axis By Z Speed */
                p->z += p->zi * p->dt + p->zg * (0.5f) * p->dt * p->dt;

                /* Take Pull On X Axis Into Account */
                p->xi += p->xg * p->dt;
                /* Take Pull On Y Axis Into Account */
                p->yi += p->yg * p->dt;
                /* Take Pull On Z Axis Into Account */
                p->zi += p->zg * p->dt;

                p->yg -= p->yg * (fabs(p->y) / 10);


                /* Reduce Particles Life By 'Fade' */
                p->life -= p->fade;

                p->dt += timeElapsed;
                /* If the particle dies, revive it */
                if ((p->life <= 0.0f) && (activo)) { // Reinicio las particulas solo si el propulsor esta activo

                    /* Give the particles life */
                    p->life = 1.0f;
                    /* Random Fade Speed */
                    p->fade = (float) (rand() % 100) / 1000.0f + 0.01f;

                    p->dt = 0;

                    /* Set the position on the X axis */
                    p->x = ((float) (rand() % 500)) / 7000.0f;
                    /* Set the position on the Y axis */
                    p->y = ((float) (rand() % 500)) / 7000.0f;
                    /* Set the position on the Z axis */
                    p->z = ((float) (rand() % 500)) / 7000.0f;

                    /* Random Speed On X Axis */
                    p->xi = 0;
                    /* Random Speed On Y Axi */
                    p->yi = 0;
                    /* Random Speed On Z Axis */
                    p->zi = 0;

                    /* Set Horizontal Pull To Zero */
                    p->xg = (p->x - 0.035)*3000;
                    /* Set Vertical Pull Downward */
                    p->yg = -1000.0f + 0.01 * fabs(p->xg) ;
                    /* Set Pull On Z Axis To Zero */
                    p->zg = (p->z -  0.035)*3000;
                    
                    if(turbo)
                    {
                        p->xg *= 30;
                        
                        p->yg *= 20;
                        
                        p->zg *= 20;
                    }
                }
                }
            }

        }
    }
    
        
    if (!particulasVisibles)
        apagado = true;
}

bool Propulsor::Expulsando(){
    return (activo || !apagado);
}

void Propulsor::Desactivar() {
    activo = false;
    apagado = false;
}

void Propulsor::Activar() {
    activo = true;
    
    for (int loop = 0; loop < MAX_PARTICLES; loop++) {
        if(particles[loop].life <= 0.0f)
                resetParticle(loop);
    }
}

void Propulsor::ReiniciarParticulas()
{
    for (int loop = 0; loop < MAX_PARTICLES; loop++) {
        resetParticle(loop);
    }
}

Propulsor::~Propulsor() {
}

