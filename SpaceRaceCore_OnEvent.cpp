//==============================================================================
#include "SpaceRaceCore.h"

//==============================================================================

void SpaceRaceCore::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}

//==============================================================================

void SpaceRaceCore::OnExit() {
    Running = false;
}

//==============================================================================

void SpaceRaceCore::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch (sym) {
        case SDLK_q:
        {
            Running = false;
            break;
        }
        case SDLK_ESCAPE:
        {
            if (!aterrizaje)
            {
                if (MenuAjustes)
                {
                    MenuAjustes = false;
                    posicionMenu = 0;
                    cantPosicionesMenu = 4;
                }
                else if (Creditos)
                    Creditos = false;
                else if (!nuevoJuego)
                {
                    if (Menu)
                    {
                        Pause = false;
                        Menu = false;
                    }
                    else if (!Pause)
                    {
                        Pause = true;
                        Menu = true;  
                    }
                }
            }
            break;
        }
        case SDLK_F11:
        {
            //handle_changeScreen();
            break;
        }
        case SDLK_SPACE:
        {
            if (!Pause)
            {
                barraEspaciadora = true;
                if(combustible>0 && !crash){
                    propulsor = true;
                    modeloPropulsor->Activar();
                }
            }
            break;
        }
        case SDLK_w:
        {
            if (!Pause)
                abajo ? adelante = false : adelante = true;
            break;
        }
        case SDLK_s:
        {
            if (!Pause)
                adelante ? abajo = false : abajo = true;
            break;
        }
        case SDLK_d:
        {
            if (!Pause)
                izquierda ? derecha = false : derecha = true;
            break;
        }
        case SDLK_a:
        {
            if (!Pause)
                derecha ? izquierda = false : izquierda = true;
            break;
        }
        case SDLK_p:
        {
            if (!Menu && !crash)
                Pause ? Pause = false : Pause = true;
            break;
        }
        case SDLK_UP:
        {
            if (Menu)
            {
                if (posicionMenu == 0)
                       posicionMenu = cantPosicionesMenu;
                 posicionMenu = (posicionMenu - 1) % cantPosicionesMenu;
            }          
            break;
        }
        case SDLK_DOWN:
        {           
            if (Menu)
                posicionMenu = (posicionMenu + 1) % cantPosicionesMenu;
            break;
        }
        case SDLK_RIGHT:
        {
            if (MenuAjustes)
            {
                if (posicionMenu == 0)
                {
                    if (velocidadJuego < 2)
                        velocidadJuego++;
                }
                else if (posicionMenu == 7)
                {
                    if (altura < 80)
                    {
                        altura += 30;
                        this->terreno->~Terreno();
                        this->terreno = new Terreno(altura);
                    }
                }
                else if (posicionMenu == 5)
                {
                    if (colorLuz < 4)
                    {
                        colorLuz++;
                        setColorLuzSatelite();
                    }
                }
                else if (posicionMenu == 6)
                {
                    if (posicionLuz < 3)
                    {
                        posicionLuz++;
                        setPosicionLuzSatelite();
                    }
                }
            }
            break;
        }
        case SDLK_LEFT:
        {
            if (MenuAjustes)
            {
                if (posicionMenu == 0)
                {
                    if (velocidadJuego > 0)
                        velocidadJuego--;
                }
                else if (posicionMenu == 7)
                {
                    if (altura > 20)
                    {
                        altura -= 30;
                        this->terreno->~Terreno();
                        this->terreno = new Terreno(altura);   
                    }
                }
                else if (posicionMenu == 5)
                {
                    if (colorLuz > 1)
                    {
                        colorLuz--;
                        setColorLuzSatelite();
                    }
                }
                else if (posicionMenu == 6)
                {
                    if (posicionLuz > 1)
                    {
                        posicionLuz--;
                        setPosicionLuzSatelite();
                    }
                }
            }
            break;
        }
        case SDLK_RETURN:
        {
            if (Menu)
            {   
                if (MenuAjustes)
                {
                    if (posicionMenu == 1) //Wireframe
                        wireframe ? wireframe = false : wireframe = true;
                    else if (posicionMenu == 2) //Facetado
                        facetado ? facetado = false : facetado = true;
                    else if (posicionMenu == 3) //Texturas
                        texturas ? texturas = false : texturas = true;
                    else if (posicionMenu == 4) //Luces
                        lighting ? lighting = false : lighting = true;
                }
                else if (!Creditos) //Menu principal
                {
                    if (posicionMenu == 3) //Salir
                        Running = false;
                    else
                    {
                        if (posicionMenu == 0) //Nuevo Juego
                        {
                            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                            glLoadIdentity(); 
                            SDL_GL_SwapBuffers();
                            SDL_Delay(500);
                            nuevoJuego = false;
                            nivel = 1;
                            puntos = puntosTotal = 0;
                            initVariablesJuego();
                            prevTime = SDL_GetTicks();
                            Menu = false;
                            Pause = false;
                        }
                        else if (posicionMenu == 1) //Ajustes
                        {
                            MenuAjustes = true;
                            cantPosicionesMenu = 8;
                        }    
                        else if (posicionMenu == 2) //Creditos
                            Creditos = true;
                        posicionMenu = 0;
                    }  
                } 
            }
            else if (aterrizaje)
            {
                if (nuevoJuego)
                {
                    Menu = true;
                    nivel = 1;
                    aterrizaje = crash = false;
                    initVariablesJuego(); 
                }
                else
                {
                    nivel++;
                    puntosTotal += puntos;
                    puntos = 0;
                    if (nivel == 5)
                    {
                        nuevoJuego = true;
                        nivel = 4;
                    }
                    else
                    {
                        aterrizaje = crash = false;
                        initVariablesJuego();
                        prevTime = SDL_GetTicks();
                    }
                }
            }
            break;
        }
        case SDLK_l:
        {
            if (!Pause)
            {
                if(luzSatelite){
                    glDisable(GL_LIGHT4);
                    luzSatelite = false;
                }
                else
                {
                    glEnable(GL_LIGHT4);
                    luzSatelite = true;
                }
            }
            break;
        }
        case SDLK_f:
        {
            if (!Pause)
            {
                if(lucesLunarLander){
                    glDisable(GL_LIGHT0);
                    glDisable(GL_LIGHT1);
                    glDisable(GL_LIGHT2);
                    glDisable(GL_LIGHT3);
                    lucesLunarLander = false;
                }
                else
                {
                    glEnable(GL_LIGHT0);
                    glEnable(GL_LIGHT1);
                    glEnable(GL_LIGHT2);
                    glEnable(GL_LIGHT3);
                    lucesLunarLander = true;
                }
            }
            break;
        }
        case SDLK_t:
        {
            if (!Pause)
            {
                if(!turboUsado &&  barraEspaciadora &&!crash){
                    turboUsado = true;
                    turbo = true;
                    if(!propulsor)
                            modeloPropulsor->Activar();
                }
            }
            break;
        }
        case SDLK_1:
        {
            if (!Pause)
                camara_delantera ? camara_delantera = false: camara_delantera = true;
        }
        case SDLK_n:
        {
            naveEnemiga->switchLuces();
        }

        default:
            break;
    }
}

void SpaceRaceCore::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    switch (sym) {
        case SDLK_SPACE:
        {
            if(!turbo){
                propulsor = false;
                modeloPropulsor->Desactivar();   
            }
            barraEspaciadora = false;
            break;

        }
        case SDLK_w:
        {
            adelante = false;
            break;
        }
        case SDLK_s:
        {
            abajo = false;
            break;
        }
        case SDLK_d:
        {
            derecha = false;
            break;
        }
        case SDLK_a:
        {
            izquierda = false;
            break;
        }
        default:
            break;
    }
}

void SpaceRaceCore::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    if (Right && !crash && !camara_delantera && !(turbo && Pause)) {
        xrot_cam += relX / 5;
        yrot_cam += relY / 5;
        if (yrot_cam <= 0)
            yrot_cam = 0;
        else if (yrot_cam >= maxyrot_cam)
            yrot_cam = maxyrot_cam;
    }
}

void SpaceRaceCore::OnMouseWheel(bool Up, bool Down) {
    if(!camara_delantera && !(turbo && Pause)){
        if (Up)
            ztra_cam -= 0.5;
        else
            ztra_cam += 0.5;
        if (ztra_cam > maxztra_cam)
            ztra_cam = maxztra_cam;
        else if (ztra_cam < minztra_cam)
            ztra_cam = minztra_cam;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(ztra_cam, (GLfloat) anchoPantalla / (GLfloat) altoPantalla, 0.1, 100);
        glMatrixMode(GL_MODELVIEW);
    }
}


bool SpaceRaceCore::handle_changeScreen()
{
    if (pantallaCompleta)
    {
        if ((Surf_Display = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE | SDL_OPENGL )) == NULL)
        {
                fprintf(stderr, "Error al setear el modo de video: %s\n", SDL_GetError());
                return false;
        }
        pantallaCompleta = false;
    }
    else
    {
        if ((Surf_Display = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE | SDL_OPENGL | SDL_FULLSCREEN)) == NULL)
        {
                printf("Error al setear el modo de video: %s\n", SDL_GetError());
                return false;
        }
        pantallaCompleta = true;
    }
    setup_OpenGl(anchoPantalla, altoPantalla); 
    return true;
}
