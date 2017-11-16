/* 
 * File:   Particle.h
 * Author: bdi_usr
 *
 * Created on 20 de mayo de 2012, 0:10
 */

#ifndef PARTICLE_H
#define	PARTICLE_H

typedef struct {
    int active; /* Active (Yes/No) */
    float life; /* Particle Life   */
    float fade; /* Fade Speed      */
    float dt;

    float r; /* Red Value       */
    float g; /* Green Value     */
    float b; /* Blue Value      */

    float x; /* X Position      */
    float y; /* Y Position      */
    float z; /* Z Position      */

    float xi; /* X Direction     */
    float yi; /* Y Direction     */
    float zi; /* Z Direction     */

    float xg; /* X Gravity       */
    float yg; /* Y Gravity       */
    float zg; /* Z Gravity       */
    
} Particle;



inline bool compareParticles(Particle* particle1, Particle* particle2) 
{
    return ((particle1->z) < (particle2->z));
}


#endif	/* PARTICLE_H */

