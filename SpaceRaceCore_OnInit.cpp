//==============================================================================
#include "SpaceRaceCore.h"
#include "math_3d.h"
#include "ModelManager.h"
#include <SDL/SDL_image.h>
//==============================================================================

bool SpaceRaceCore::OnInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    flog = fopen ("log.txt","w");
    pantallaCompleta = false;
    aterrizaje = crash = false;
    //Cambiar esos valores para la resolucion de la pantalla
    anchoPantalla = 800;
    altoPantalla = 600;
    
    Pause = true;
    Menu = true;
    nuevoJuego = true;
    posicionMenu = 0;
    cantPosicionesMenu = 5;
    MenuAjustes = false;
    Creditos = false;
    
    altura = 50;
    displayListSkyBox = -1;
    
    velocidadJuego = 1;
    wireframe = false;
    facetado = false;
    texturas = true;
    lighting = true;
    colorLuz = 1;
    posicionLuz = 1;
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // basically anti-aliasing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2); // how much to anti-alias
    
    if ((Surf_Display = SDL_SetVideoMode(anchoPantalla, altoPantalla, 32, SDL_HWSURFACE | SDL_OPENGL)) == NULL) {
        return false;
    }

    setup_OpenGl(anchoPantalla, altoPantalla);
    
    //Se crea el terreno
    terreno = new Terreno(altura);
    
    //Carga de texturas
    loadTextures();
    
    //Se crea la freetype fuente a usar
    our_font.init("StarJedi.TTF", 16);
    
    // XXX docs say all polygons are emitted CCW, but tests show that some aren't.
    if(getenv("MODEL_IS_BROKEN"))  
           glFrontFace(GL_CW);
    
    //CARGADO DE MODELOS 3D
    
    ModelManager::Inst()->LoadMesh("apolloLM_full.3ds",MODEL_LUNAR_LANDER);
    //Guardo el x,y,z minimo del modelo para poder usarlas para las colisiones
    min_Y_LunarLander = ModelManager::Inst()->GetModelo(MODEL_LUNAR_LANDER)->getModelo_minY();
    min_Z_LunarLander = ModelManager::Inst()->GetModelo(MODEL_LUNAR_LANDER)->getModelo_minZ();
    min_X_LunarLander = ModelManager::Inst()->GetModelo(MODEL_LUNAR_LANDER)->getModelo_minX();
    
    ModelManager::Inst()->LoadMesh("mariner2.3ds",MODEL_SATELITE);
    
    ModelManager::Inst()->LoadMesh("t.3ds",MODEL_T);
    
    // Sistemas de Particulas
    modeloPropulsor = new Propulsor();
    polvoLunar = new PolvoLunar();
    explosion = new Explosion();
    
    initVariablesJuego();
    
    naveEnemiga = new NaveEnemiga(pos_lander);
    
    initLuces();
    
    return true;
}

void SpaceRaceCore::setup_OpenGl(int ancho, int alto)
{
    
    glClearColor(0.0, 0.0, 0.0, 0);
    glClearDepth(1.0f); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ztra_cam = 55;
    gluPerspective(ztra_cam, (GLfloat)ancho / (GLfloat) alto, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //GLfloat global_ambient[] = { 0.5f, 0.4f, 0.5f, 1.0f };
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); // a la hora de pintarlos por pantalla, hace una comprobación de que no haya ninguna primitiva que esté por delante tapando a lo que se va a pintar en ese lugar.
    glDepthFunc(GL_LEQUAL);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    glEnable(GL_NORMALIZE);
 
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    
    glFogi (GL_FOG_MODE, GL_EXP2);
    GLfloat fogColor[4] = {0.01, 0.01, 0.01, 1.0}; 
    glFogfv (GL_FOG_COLOR, fogColor);
    GLfloat density = 0.02;
    glFogf (GL_FOG_DENSITY, density);
    
    glHint (GL_FOG_HINT, GL_NICEST);
}

void SpaceRaceCore::loadTextures()
{
    TextureManager::Inst()->LoadTexture("moon.png",             TEXT_LUNA,      0, 0, 0, 0);
    TextureManager::Inst()->LoadTexture("skybox_BK.png",        TEXT_FONDO_BK,  0, 0, 0, 1);
    TextureManager::Inst()->LoadTexture("skybox_DN.png",        TEXT_FONDO_DN,  0, 0, 0, 1);
    TextureManager::Inst()->LoadTexture("skybox_FT.png",        TEXT_FONDO_FT,  0, 0, 0, 1);
    TextureManager::Inst()->LoadTexture("skybox_LF.png",        TEXT_FONDO_LF,  0, 0, 0, 1);
    TextureManager::Inst()->LoadTexture("skybox_RT.png",        TEXT_FONDO_RT,  0, 0, 0, 1);
    TextureManager::Inst()->LoadTexture("skybox_UP.png",        TEXT_FONDO_UP,  0, 0, 0, 1);
}

void SpaceRaceCore::initVariablesJuego()
{
    // TIEMPO
    
    startTime = 0;
    prevTime = startTime;
    
    // COORDENADAS CAMARA
    camara_delantera = false;
    xrot_cam = 0.0f; yrot_cam = 15.0f; 
    xtra_cam = 0.0f; ytra_cam = 0.0f; 
    
    ztra_cam = 55;
    minztra_cam = 30.0f;
    maxztra_cam = 60.0f;
    
    maxyrot_cam = 30.0f; 
    
    /***********************************************/
    
    // COORDENADAS LUNAR LANDER
    
    xrot_lander = 0.0f; yrot_lander = 0.0f;
    maxxrot_lander = 20.0f; maxyrot_lander = 30.0f;
    propulsor = false; combustibleInicial = combustible = 20;
    turbo = false; turboUsado = false; tiempoTurbo = 0; barraEspaciadora = false;
    adelante = false; derecha = false; izquierda = false; abajo = false;
    pos_lander.Set(0.0f,30.0f,0.0f); vel_lander.Set(0.0f,0.0f,-7.0f);
    vel_giro_lander = 20;
    
    modeloPropulsor->ReiniciarParticulas();
    
    /***********************************************/
    
    // COORDENADAS DEL SATELITE QUE SIGUE AL LUNAR LANDER
    
    anguloSatelite = 0.0f;
    
    gLuna = 1.62; // M/S^2
    masaLunarLander = 15061; // KG
    fuerzaPeso = gLuna*masaLunarLander;
    fuerzaPropulsor = 2.0*fuerzaPeso; // Newtons
    
    
}

void copiarArray(GLfloat* orig,GLfloat* &dest,int n)
{
    dest = new GLfloat[n];
    for(int i = 0; i < n; i++)
    {
        dest[i] = orig[i];
    }
}

void SpaceRaceCore::initLuces()
{
     //LUCES
    lucesLunarLander = true;
    luzSatelite = true;
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    
    
    // Luz Amarilla
    spot_cutoff3        = 30.0;
    spot_exponent3      = 5.0;
    GLfloat light_diffuse3[4]   = { 1.0, 1.0, 0.0, 1.0 };
    copiarArray(light_diffuse3,this->light_diffuse3,4);
    
    GLfloat light_specular3[4]  = { 0.1, 0.1, 0.3, 1.0 };
    copiarArray(light_specular3,this->light_specular3,4);
    
    GLfloat light_position3[4]  = { 0.0, 2.6, 2.5, 1.0 };
    copiarArray(light_position3,this->light_position3,4);
    
    GLfloat spot_direction3[3]  = { 0.0, -1.0, 0.2 };
    copiarArray(spot_direction3,this->spot_direction3,3);
    
    //Luz Propulsor
    spot_cutoff2        = 15.0;
    spot_exponent2      = 20.0;
    GLfloat light_diffuse2[4]   = { 0.1, 0.1, 0.3, 1.0 };
    copiarArray(light_diffuse2,this->light_diffuse2,4);
    GLfloat light_specular2[4]  = { 0.1, 0.1, 0.3, 1.0 };
    copiarArray(light_specular2,this->light_specular2,4);
    GLfloat light_position2[4]  = { -0.05, -0.9, -0.2, 1.0 };
    copiarArray(light_position2,this->light_position2,4);
    GLfloat spot_direction2[3]  = { 0.0, -1.0, 0.0 };
    copiarArray(spot_direction2,this->spot_direction2,3);

    
    // Luces Rojas de Adelante
    GLfloat light_diffuse0[4]   = { 0.7, 0.0, 0.0, 1.0 };
    copiarArray(light_diffuse0,this->light_diffuse0,4);
    GLfloat light_ambient0[4]   = { 0.2, 0.2, 0.2, 1.0 };
    copiarArray(light_ambient0,this->light_ambient0,4);
    GLfloat light_specular0[4]  = { 1.0, 1.0, 1.0, 1.0 };
    copiarArray(light_specular0,this->light_specular0,4);
    GLfloat light_position0[4]  = { 1.7, -0.2, -1.7, 1.0 };
    copiarArray(light_position0,this->light_position0,4);
    GLfloat spot_direction0[3]  = { 0.5, -1.0, -0.3 };
    copiarArray(spot_direction0,this->spot_direction0,3);
    spot_cutoff0        = 45.0;
    spot_exponent0      = 1.0;
    
    GLfloat light_position1[4]  = { -1.7, -0.2, -1.7, 1.0 };
    copiarArray(light_position1,this->light_position1,4);
    GLfloat spot_direction1[3]  = { -0.5, -1.0, -0.3 };
    copiarArray(spot_direction1,this->spot_direction1,3);
    
    // Luz Satelital
    setPosicionLuzSatelite();
    setColorLuzSatelite();
}

void SpaceRaceCore::setColorLuzSatelite()
{
    GLfloat* ambientLight4 = new GLfloat[4];
    GLfloat* diffuseLight4 = new GLfloat[4];
    GLfloat* specularLight4 = new GLfloat[4];
    ambientLight4[3]  = 1.0f;
    diffuseLight4[3]  = 1.0f;
    specularLight4[3] = 1.0f;
    
    switch (colorLuz)
    {
        case 1:
        {
            ambientLight4[0]  = 1.0f; ambientLight4[1]  = 1.0f; ambientLight4[2]  = 1.0f; 
            diffuseLight4[0]  = 1.0f; diffuseLight4[1]  = 1.0f; diffuseLight4[2]  = 1.0f; 
            specularLight4[0] = 1.0f; specularLight4[1] = 1.0f; specularLight4[2] = 1.0f;
            break;  
        }
        case 2:
        {
            ambientLight4[0]  = 1.0f; ambientLight4[1]  = 0.0f; ambientLight4[2]  = 0.0f;
            diffuseLight4[0]  = 1.0f; diffuseLight4[1]  = 0.0f; diffuseLight4[2]  = 0.0f;
            specularLight4[0] = 1.0f; specularLight4[1] = 0.0f; specularLight4[2] = 0.0f;
            break;  
        }
        case 3:
        {
            ambientLight4[0]  = 0.0f; ambientLight4[1]  = 1.0f; ambientLight4[2]  = 0.0f; 
            diffuseLight4[0]  = 0.0f; diffuseLight4[1]  = 1.0f; diffuseLight4[2]  = 0.0f; 
            specularLight4[0] = 0.0f; specularLight4[1] = 1.0f; specularLight4[2] = 0.0f;
            break;  
        }
        case 4:
        {
            ambientLight4[0]  = 0.0f; ambientLight4[1]  = 0.0f; ambientLight4[2]  = 1.0f; 
            diffuseLight4[0]  = 0.0f; diffuseLight4[1]  = 0.0f; diffuseLight4[2]  = 1.0f; 
            specularLight4[0] = 0.0f; specularLight4[1] = 0.0f; specularLight4[2] = 1.0f;
            break;  
        }
    }
    
    copiarArray(ambientLight4,this->light_ambient4,4);
    copiarArray(diffuseLight4,this->light_diffuse4,4);
    copiarArray(specularLight4,this->light_specular4,4); 
}

void SpaceRaceCore::setPosicionLuzSatelite()
{
    GLfloat* position4 = new GLfloat[4];
    position4[0] = 0.0f;
    position4[2] = 0.0f;
    position4[3] = 0.0f;
    
    switch (posicionLuz)
    {
        case 1:
        {
            position4[1] = -1.0f;
            break;
        }
        case 2:
        {
            position4[1] = 1.0f;
            break;
        }
        case 3:
        {
            position4[1] = 0.0f;
            position4[2] = -1.0f;
            break;
        }
    }
    
    copiarArray(position4,this->light_position4,4);
}


//==============================================================================

