//==============================================================================
#include "SpaceRaceCore.h"
#include "FreeType.h"

//==============================================================================
SpaceRaceCore::SpaceRaceCore() {
    Surf_Display = NULL;

    Running = true;
}

SpaceRaceCore::~SpaceRaceCore()
{
    //delete this->terreno;
    //our_font.clean();
}

//------------------------------------------------------------------------------
int SpaceRaceCore::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    SDL_EnableKeyRepeat(100, 100);
    while(Running) {
        currTime = SDL_GetTicks();
        timeElapsed = 0.001*(currTime - prevTime);
        if (velocidadJuego == 0)
            timeElapsed *= 0.5;
        else if (velocidadJuego == 2)
            timeElapsed *= 1.5;
        prevTime = currTime;

        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        
        OnLoop();
        OnRender();
          
        SDL_Delay(2);
    }

    OnCleanup();

    return 0;
}

//==============================================================================
int main(int argc, char* argv[]) {
    SpaceRaceCore theApp;

    return theApp.OnExecute();
}

//==============================================================================
