#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../lib/lib_core.h"
#include "../lib/lib_screen.h"

#define SX 1200
#define SY 600
using namespace lib;

int main ( int, char ** )
{
    auto var = core();

    lib_Screen Display;
    Display.create("Demo",SX,SY,SDL_WINDOW_RESIZABLE,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Display.setLogicalFPS(60);
    SDL_RenderSetLogicalSize(Display.getRender(),SX,SY);

	while ( Display.Do() )
	{
        auto space = Display.key(SDLK_SPACE);
		if (space & OnNewPress)
			cout << "OnNewPress" << endl;
		if (space & OnPress)
			cout << "OnPress" << endl;
		
		// affichage ?
	}
    
    return 0;
}


