//==============================================================================
// SDL Tutorial 1
//==============================================================================
#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include "CEvent.h"
#include "TextureManager.h"
#include "Terreno.h"
#include "math.h"
#include "math_3d.h"
#include "Claves.h"
#include "Propulsor.h"
#include "FreeType.h"
#include "PolvoLunar.h"
#include "Explosion.h"
#include "NaveEnemiga.h"

//==============================================================================
class SpaceRaceCore : public CEvent {
    private:
        
        SDL_Surface* Surf_Display;
        
        bool    Running;
        bool    Pause;
        
        bool nuevoJuego;
        int nivel;
        int puntos;
        int puntosTotal;
        bool Menu;
        int  posicionMenu;
        int  cantPosicionesMenu;
        bool MenuAjustes;
        bool Creditos;
        
        // Ajustes
        int velocidadJuego;
        bool wireframe;
        bool facetado;
        bool texturas;
        bool lighting;
        int colorLuz;
        int posicionLuz;
        
        // CHOQUE Y ATERRIZAJE
        float vel_choque;
        bool    crash;
        bool    aterrizaje;
        
        // INIT SDL
        bool pantallaCompleta;
        int anchoPantalla;
        int altoPantalla;
        
        
        // ARCHIVO DE PRUEBAS
        FILE *flog;
        
        //Todo la info de la fuente que usamos
        freetype::font_data our_font;
        
        Terreno* terreno;
        float altura;       
        int displayListSkyBox;
        
        // Variables Tiempo
        int startTime;
        int prevTime;
        int currTime;
        float timeElapsed;
        
        GLfloat angulo;
       
        //Coordenadas de la camara        
        // Variables movimiento de la camara
        bool camara_delantera;
        GLfloat xrot_cam,yrot_cam,xtra_cam,ytra_cam,ztra_cam;
        GLfloat maxyrot_cam, maxztra_cam, minztra_cam;
        
        // Variables movimiento del Lunar Lander
        Vector3f pos_lander;
        GLfloat xrot_lander,yrot_lander;
        GLfloat maxxrot_lander, maxyrot_lander;
        Vector3f vel_lander;
        bool propulsor;
        float combustibleInicial, combustible;
        bool adelante, derecha, izquierda, abajo;
        bool turbo, turboUsado, barraEspaciadora;
        float tiempoTurbo;
        Vector3f acl_lander;
        float Fx,Fy,Fz,Hip;
        float vel_giro_lander;
        
         // Coordenadas del Satelite
        GLfloat anguloSatelite;
        // Constantes varias
        GLfloat gLuna; // M/S^2
        GLfloat masaLunarLander; // KG
        GLfloat fuerzaPeso;
        GLfloat fuerzaPropulsor; // Newtons
        GLfloat min_Y_LunarLander;
        GLfloat min_Z_LunarLander;
        GLfloat min_X_LunarLander;
        
        
        Propulsor* modeloPropulsor;
        PolvoLunar* polvoLunar;
        Explosion* explosion;
        NaveEnemiga* naveEnemiga;
        
        //Luces
        bool lucesLunarLander,luzSatelite;
        // Luz Amarilla
        GLfloat  spot_cutoff3, spot_exponent3;
        GLfloat *light_diffuse3, *light_specular3, *light_position3, *spot_direction3;
        
        //Luz Propulsor
        GLfloat  spot_cutoff2, spot_exponent2;
        GLfloat *light_diffuse2,*light_specular2,*light_position2,*spot_direction2;

        // Luces Rojas de Adelante
        GLfloat  spot_cutoff0, spot_exponent0;
        GLfloat *light_diffuse0, *light_ambient0, *light_specular0, *light_position0, *spot_direction0;
       
        GLfloat *spot_direction1, *light_position1;
        
        // Luz Satelital
        
        GLfloat *light_position4, *light_ambient4, *light_diffuse4, *light_specular4;

    public:
        SpaceRaceCore();
        virtual ~SpaceRaceCore();
        
        int OnExecute();

        bool OnInit();
        
        void OnEvent(SDL_Event* Event);
        
        void OnExit();
        
        void OnLoop();

        void OnRender(); 
        
        void OnCleanup();
        
    private:
        
        //Configuracion OpenGl
        
        void setup_OpenGl(int ancho, int alto);
        
        //Otros
        
        void setColorLuzSatelite();

        void setPosicionLuzSatelite();

        bool handle_changeScreen();
        
        void loadTextures();
        
        void initVariablesJuego();
        
        void aterrizajeLuna();
        
        void initLuces();
        
        
        // Renderizado
        
        void dibujarNaveEnemiga();
        
        void dibujarLunarLanderYSatelite();
        
        void dibujarLucesLunarLander();
        
        void dibujarPropulsor(bool pause);
        
        void dibujarPolvoLunar(bool pause);
        
        void dibujarExplosion();
        
        void dibujarSkyBox();
        
        void dibujarTerreno();
        
        void dibujarHud();
        void dibujarAterrizajeMensaje();
        void dibujarFinJuego();
        
        void dibujarMenu();
        void dibujarMenuAjustes();
        void dibujarCreditos();
        
        // Eventos Capturados
        
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
        
        void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
        
        void OnMouseWheel(bool Up, bool Down);

};

//==============================================================================

#endif
