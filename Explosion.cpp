/* 
 * File:   Explosion.cpp
 * Author: bdi_usr
 * 
 * Created on 21 de mayo de 2012, 7:22
 */

#include "Explosion.h"

void Explosion::resetParticle(int num){
    /* Give the particles life */
    particles[num].life = 1.0f;
    /* Random Fade Speed */
    particles[num].fade = (float) (rand() % 100) / 1000.0f + 0.06f;
    /* Select Red Rainbow Color */
    
    particles[num].dt = 0;
    
    particles[num].r = 1.0f;
    /* Select Green Rainbow Color */
    particles[num].g = 1.0f;
    /* Select Blue Rainbow Color */
    particles[num].b = 0.8f;
    
     /* Set the position on the Y axis */
    particles[num].y =  0.1 + (float)( (rand() % 500) / 120.0f );
    
    /* Set the position on the X axis */
    particles[num].x = (float)( (rand() % 500) / (50.0f * (particles[num].y)) ) + (particles[num].y)/2.0;
   
    
    /* Set the position on the Z axis */
    particles[num].z = (float)( (rand() % 500)  / (50.0f *(particles[num].y)) );
    /* Random Speed On X Axis */
    particles[num].xi = 0;
    /* Random Speed On Y Axi */
    particles[num].yi = 0;
    /* Random Speed On Z Axis */
    particles[num].zi = 0;
    /* Set Horizontal Pull To Zero */
    particles[num].xg = (particles[num].x - 2.5)/(1 + rand() % 500);
    /* Set Vertical Pull Downward */
    particles[num].yg = (fabs(particles[num].xg) + particles[num].yi*10)*(rand() % 100 + 1);
    /* Set Pull On Z Axis To Zero */
    particles[num].zg = (particles[num].z - 2.5)/(1 + rand() % 500);
    
}


Explosion::Explosion() {
    
    ini_x       = 0.0;
    ini_y       = 0.0f;
    ini_z       = 0.0f;
    ladoPart    = 0.4f;
    
    TextureManager::Inst()->LoadTextureBMP("particle2.bmp",TEXT_PARTICULA_FIRE);
    //TextureManager::Inst()->LoadTextureBMP("particle.bmp",      TEXT_PARTICULA);
    
    for (int loop = 0; loop < MAX_PARTICLES_EX; loop++) {
        resetParticle(loop);
    }
}


void Explosion::Render(float timeElapsed,bool choque) {
    
    TextureManager::Inst()->BindTexture(TEXT_PARTICULA_FIRE);
    
    glTranslatef(-3 + ini_x , -2 + ini_y , -2 + ini_z );
    
    if (choque) {
        
        vector<Particle*> ps; 
        for(int i = 0; i < MAX_PARTICLES_EX; i++) {
                ps.push_back(particles + i);
        }
        sort(ps.begin(), ps.end(), compareParticles);
        
        for(unsigned int i = 0; i < ps.size(); i++) {
            Particle* p = ps[i]; 
            if (p->life > 0) {
                
                /* Draw The Particle Using Our RGB Values,
                 * Fade The Particle Based On It's Life
                 */
                
                glColor4f(p->r,p->g*(p->life),p->b*p->life/2.0f, p->life);
                
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
                if ( p->life < 0.0f ) {

                    /* Give the particles life */
                    p->life = 1.0f;
                    /* Random Fade Speed */
                    p->fade = (float) (rand() % 100) / 1000.0f + 0.006f;

                    p->dt = 0;

                    p->y = 0.1 + (float)( (rand() % 500) / 120.0f );
                    /* Set the position on the X axis */
                    p->x = (float)( (rand() % 500) / (50.0f * (p->y)) ) + (p->y)/2.0;
                    /* Set the position on the Y axis */
                    
                    /* Set the position on the Z axis */
                    p->z = (float)( (rand() % 500)  / (50.0f *(p->y)) );

                    /* Random Speed On X Axis */
                    p->xi = 0;
                    /* Random Speed On Y Axi */
                    p->yi = 0;
                    /* Random Speed On Z Axis */
                    p->zi = 0;

                    /* Set Horizontal Pull To Zero */
                    p->xg = (p->x - 2.5)/(1 + rand() % 500);
                    /* Set Vertical Pull Downward */
                    p->yg = (fabs(p->xg) + p->yi*10)*(rand() % 100 + 1);
                    /* Set Pull On Z Axis To Zero */
                    p->zg = (p->z - 2.5)/(1 + rand() % 500);
                   
                }
            }

        }
    }
}

Explosion::~Explosion() {
}

