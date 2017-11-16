//==============================================================================
#include "SpaceRaceCore.h"
#include "math_3d.h"

//==============================================================================

void SpaceRaceCore::OnLoop() {
    
    /***********************************************/
    if (!Pause || crash)
    {
        naveEnemiga->OnLoop(timeElapsed,pos_lander,vel_lander);
        // Calculo de Rotacion del satelite
        anguloSatelite += 10.0f * timeElapsed;
    }
    
    /***********************************************/
    if(!crash && !Pause){
        if((propulsor && (combustible > 0)) || (turbo)) // propulsor activado
        {
            
            // al apretar el propulsor la nave gira sola.
            if(yrot_lander >= 0)
                yrot_lander += vel_giro_lander*timeElapsed/5;
            else
                yrot_lander -= vel_giro_lander*timeElapsed/5;

            if(xrot_lander >= 0)
                xrot_lander += vel_giro_lander*timeElapsed/5;
            else
                xrot_lander -= vel_giro_lander*timeElapsed/5;
             
            if(!turbo)
                combustible -= timeElapsed;

            if((combustible < 0) && (!turbo)){
                propulsor = false;
                modeloPropulsor->Desactivar();
            }

            // Calculo de la nueva posicion del Lunar Lander



            Fx = fuerzaPropulsor*(sinf(xrot_lander*M_PI/180));
            Fz = -fuerzaPropulsor*(sinf(yrot_lander*M_PI/180)); 
            //Hip = sqrt(Fx*Fx + Fz*Fz);
            Fy = sqrt(fuerzaPropulsor*fuerzaPropulsor - (Fx*Fx) - (Fz*Fz));
            //Fy = fuerzaPropulsor*cos(asin(Hip/fuerzaPropulsor));


            if(turbo){

                tiempoTurbo += timeElapsed;

                Fx = Fx*3;
                Fy = Fy*3;
                Fz = Fz+3;

                if(tiempoTurbo > 2.0){
                    turbo = false;
                    if(!barraEspaciadora){
                        propulsor = false;
                        modeloPropulsor->Desactivar();
                    }
                }
            }

            acl_lander.Set(Fx/masaLunarLander,(-fuerzaPeso + Fy)/masaLunarLander, Fz/masaLunarLander);

            pos_lander += vel_lander*timeElapsed + acl_lander*(0.5*timeElapsed*timeElapsed);
            
            vel_lander += acl_lander*(timeElapsed);

            

            //fprintf (flog, "x %f \n",pos_lander.x);
            //fprintf (flog, "x %f \n",pos_lander.y);
            //fprintf (flog, "x %f \n",pos_lander.z);


        }
        else
        {
            acl_lander.Set(0,-gLuna,0); // solo fuerza de gravedad

            pos_lander += vel_lander*timeElapsed + acl_lander*(0.5*timeElapsed*timeElapsed);

            vel_lander += acl_lander*timeElapsed;
        }

        if(adelante)
        {
            // Giro la nave hacia adelante

            yrot_lander += vel_giro_lander*timeElapsed;

            if(yrot_lander > maxyrot_lander)
                yrot_lander = maxyrot_lander;
        }
        else if(abajo)
        {
            // Giro la nave hacia atras

            yrot_lander -= vel_giro_lander*timeElapsed;

            if(yrot_lander < -maxyrot_lander)
                yrot_lander = -maxyrot_lander;
        }
        else if(!turbo)// ajuste automatico de la nave
        {
            if(yrot_lander > 0)
            {
                yrot_lander -= vel_giro_lander*timeElapsed/10;

                if(yrot_lander < 0)
                    yrot_lander += vel_giro_lander*timeElapsed/5;
            }
            else
            {
                yrot_lander += vel_giro_lander*timeElapsed/10;

                if(yrot_lander > 0)
                    yrot_lander -= vel_giro_lander*timeElapsed/5;;
            }
        }

        if(izquierda)
        {
            xrot_lander -= vel_giro_lander*timeElapsed;

            if(xrot_lander  < -maxxrot_lander)
                xrot_lander = -maxxrot_lander;
        }
        else if(derecha)
        {
            xrot_lander += vel_giro_lander*timeElapsed;

            if(xrot_lander  > maxxrot_lander)
                xrot_lander = maxxrot_lander;
        }
        else if(!turbo)// ajuste automatico de la nave
        {
            if(xrot_lander > 0)
            {
                xrot_lander -= vel_giro_lander*timeElapsed/10;
                if(xrot_lander < 0)
                    xrot_lander += vel_giro_lander*timeElapsed/5;
            }
            else
            {
                xrot_lander += vel_giro_lander*timeElapsed/10;
                if(xrot_lander > 0)
                    xrot_lander -= vel_giro_lander*timeElapsed/5;
            }
        }
    }
    else if (crash)
    {       
        modeloPropulsor->Desactivar();
        propulsor = false;
        combustible = 0;
        turboUsado = true;
        turbo = false;
        vel_choque = vel_giro_lander*timeElapsed*((fabs(vel_lander.y) + 1)/2) + acl_lander.y*(timeElapsed*timeElapsed)*5;
        if(xrot_lander >= 0)
        {
            xrot_lander += vel_choque;
            if(xrot_lander > 30)
                xrot_lander = 30;
        }
        else
        {
            xrot_lander -= vel_choque;
            if(xrot_lander < -30)
                xrot_lander = -30;
        }
        if(yrot_lander < 0)
        {
            yrot_lander -= vel_choque;
            if(yrot_lander < -90)
                yrot_lander = -90;
        }
        else
        {
            yrot_lander += vel_choque;
            if(yrot_lander > 90)
                yrot_lander = 90;
        }
         
    }
    
    if (!Pause)
    {
        aterrizajeLuna();
        if (aterrizaje)
        {
            Pause = true;
            if (!crash)
            {
                yrot_lander = 0;
                xrot_lander = 0;
                puntos += (int) (combustible * 1000 * nivel / combustibleInicial);
            }    
        }
    }
}

void SpaceRaceCore::aterrizajeLuna()
{
    //Esta altura corresponde al centro de la nave
    GLfloat alturaTerrenoC = terreno->getAlturaTerreno(pos_lander.x, pos_lander.z, nivel);
    
    //El -0.1 para que no pase la superficie
    float yLLcentral = pos_lander.y + min_Y_LunarLander - 0.1;
    float yLL1, yLL2, yLL3, yLL4;
    yLL1 = yLL2 = yLL3 = yLL4 = yLLcentral;
    const double pi = 3.14159265358979323846;
    
    float yMas1 = sinf(yrot_lander * pi / 180) * (min_Z_LunarLander);
    float yMas2 = sinf(xrot_lander * pi / 180) * (min_X_LunarLander);
    float zMas = cosf(yrot_lander * pi / 180) * (min_Z_LunarLander);
    float xMas = cosf(xrot_lander * pi / 180) * (min_X_LunarLander);
    
    float Vertice1X = pos_lander.x;
    float Vertice1Z = pos_lander.z - zMas;
    float Vertice2X = pos_lander.x + xMas;
    float Vertice2Z = pos_lander.z;
    float Vertice3X = pos_lander.x;
    float Vertice3Z = pos_lander.z + zMas;
    float Vertice4X = pos_lander.x - xMas;
    float Vertice4Z = pos_lander.z;
    
    
    GLfloat alturaTerreno1 = terreno->getAlturaTerreno(Vertice1X, Vertice1Z, nivel);
    GLfloat alturaTerreno2 = terreno->getAlturaTerreno(Vertice2X, Vertice2Z, nivel);
    GLfloat alturaTerreno3 = terreno->getAlturaTerreno(Vertice3X, Vertice3Z, nivel);
    GLfloat alturaTerreno4 = terreno->getAlturaTerreno(Vertice4X, Vertice4Z, nivel);
    
    //Verifico para todas las alturas q tengo teniendo en cuenta tambien si la nave esta rotada
    yLL1 -= yMas1;
    yLL3 += yMas1;
    yLL2 -= yMas2;
    yLL4 += yMas2;
    
    aterrizaje =  ((alturaTerrenoC >= yLLcentral) || (alturaTerreno1 >= yLL1) || (alturaTerreno2 >= yLL2)
             || (alturaTerreno3 >= yLL3) || (alturaTerreno4 >= yLL4));
    
    
    //Ahora hay que ver si es con exito o no
    if (aterrizaje)
    {
        if (yrot_lander > 7 || yrot_lander < -7)
            crash = true;
        else if (xrot_lander > 6 || xrot_lander < -6)
            crash = true;
        else
        {
            //Si las 5 alturas calculadas son aproximadamente las mismas entonces esta sobre la plataforma
            // tambien se podria hacer con las normales
            int alt1 = (int) alturaTerrenoC;
            int alt2 = (int) alturaTerreno1;
            int alt3 = (int) alturaTerreno2;
            int alt4 = (int) alturaTerreno3;
            int alt5 = (int) alturaTerreno4;
            
            //fprintf(flog, "Alturas: %i %i %i %i %i\n", alt1, alt2, alt3, alt4, alt5);

            if (alt1 != alt2 || alt1 != alt3 || alt1 != alt4 || alt1 != alt5 || 
                alt2 != alt3 || alt2 != alt4 || alt2 != alt5 || 
                alt3 != alt4 || alt3 != alt5 || 
                alt4 != alt5)
            {    
                crash = true;
            }
            
            //Controlar las velocidades
            if (vel_lander.x > 3 || vel_lander.x < -3)
                crash = true;
            else if (vel_lander.y > 4 || vel_lander.y < -4)
                crash = true;
            else if (vel_lander.z > 3 || vel_lander.z < -3)
                crash = true;
        }  
    }
}

//==============================================================================
