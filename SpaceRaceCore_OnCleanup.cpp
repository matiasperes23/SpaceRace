//==============================================================================
#include "SpaceRaceCore.h"

//==============================================================================
void SpaceRaceCore::OnCleanup() {
    delete this->terreno;
    delete this->modeloPropulsor; 
    delete this->polvoLunar;
    delete this->explosion;
    delete this->naveEnemiga;
    delete[] light_diffuse3;
    delete[] light_specular3; delete[] light_position3; delete[] spot_direction3;

    delete[] light_diffuse2; delete[] light_specular2; delete[] light_position2; delete[] spot_direction2;

    delete[] light_diffuse0; delete[] light_ambient0; delete[] light_specular0; delete[] light_position0; delete[] spot_direction0;
       
    delete[] spot_direction1; delete[] light_position1;
    
    our_font.clean();
    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
    fclose(flog);
}

//==============================================================================
