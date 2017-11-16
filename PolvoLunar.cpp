/* 
 * File:   PolvoLunar.cpp
 * Author: bdi_usr
 * 
 * Created on 20 de mayo de 2012, 0:24
 */

#include "PolvoLunar.h"

void PolvoLunar::resetParticle(int num){
    
    /* Give the particles life */
    particles[num].life = 10.0f;
    /* Random Fade Speed */
    particles[num].fade = (float) (rand() % 100) / 10000.0f + 0.03f;
    
    particles[num].dt = 0;
    
    particles[num].r = 0.9f;
    /* Select Green Rainbow Color */
    particles[num].g = 0.9f;
    /* Select Blue Rainbow Color */
    particles[num].b = 0.9f;
   /* Set the position on the X axis */
    particles[num].x = ((float) (rand() % 500)) / 10.0f;
    /* Set the position on the Y axis */
    particles[num].y = ((float) (rand() % 500)) / 10.0f;
    /* Set the position on the Z axis */
    particles[num].z = ((float) (rand() % 500)) / 10.0f;
    
    /* Random Speed On X Axis */
    particles[num].xi = ((float) (rand() % 500)) / 8000.0f;
    /* Random Speed On Y Axi */
    particles[num].yi = ((float) (rand() % 500)) / 8000.0f;
    /* Random Speed On Z Axis */
    particles[num].zi = ((float) (rand() % 500)) / 8000.0f;
    
    
    // aceleración cero, velocidad constante
    particles[num].xg = 0;
    particles[num].yg = 0;
    particles[num].zg = 0;
    
}

PolvoLunar::PolvoLunar() {
    
    ini_x = 0.95;
    ini_y = 0.1;
    ini_z = 1.05;
    ladoPart = 0.11f;
    
    for (int loop = 0; loop < MAX_PARTICLES_DUST; loop++) {
        resetParticle(loop);
    }
    
}

void PolvoLunar::Render(float timeElapsed,float x, float y, float z,bool pause) {
    
    TextureManager::Inst()->BindTexture(TEXT_PARTICULA);
    
    glTranslatef(-25.0f + ini_x , -25.0f + ini_y , -50.0 + ini_z );

    
        vector<Particle*> ps;
        for(int i = 0; i < MAX_PARTICLES_DUST; i++) {
                ps.push_back(particles + i);
        }
        //sort(ps.begin(), ps.end(), compareParticles);
        
        for(unsigned int i = 0; i < ps.size(); i++) {
            Particle* p = ps[i];
            
                /* Draw The Particle Using Our RGB Values,
                 * Fade The Particle Based On It's Life
                 */
                glColor4f(p->r * p->life, p->g * p->life, p->b * p->life , p->life);
                
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
                
                glBegin(GL_TRIANGLE_STRIP);
                /* Top Right */
                glTexCoord2d(1, 1);
                glVertex3f(p->x - ladoPart, p->y + ladoPart, p->z + ladoPart);
                /* Top Left */
                glTexCoord2d(0, 1);
                glVertex3f(p->x + ladoPart, p->y + ladoPart, p->z + ladoPart);
                /* Bottom Right */
                glTexCoord2d(1, 0);
                glVertex3f(p->x - ladoPart, p->y - ladoPart, p->z + ladoPart);
                /* Bottom Left */
                glTexCoord2d(0, 0);
                glVertex3f(p->x + ladoPart, p->y - ladoPart, p->z + ladoPart);
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

                //p->yg -= p->yg * (fabs(p->y) / 10);
                
                p->life -= p->fade;
                
                p->dt += timeElapsed;
                /* If the particle dies, revive it */
                if ( p->life <= 0) { // Reinicio las particulas solo si el propulsor esta activo
                    
                    p->dt = 0;
                    p->life = 10.0;
                    
                    p->fade = (float) (rand() % 100) / 10000.0f + 0.03f;
                    
                    // Posicion Aleatoria
                    p->x = x + ((float) (rand() % 500)) / 10.0f ;
                    
                    p->y = ((float) (rand() % 500)) / 10.0f;
                    
                    p->z = z + ((float) (rand() % 500)) / 10.0f;
                    
                    /* Velocidad Aleatoria */
                    p->xi = ((float) (rand() % 500)) / 8000.0f;
                    
                    p->yi = ((float) (rand() % 500)) / 8000.0f;
                    
                    p->zi = ((float) (rand() % 500)) / 8000.0f;

                }
                }
            

        }
}

PolvoLunar::~PolvoLunar() {
}

