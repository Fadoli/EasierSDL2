#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include "core.h"
#include "screen.h"
#include "image.h"

#define SX 1200
#define SY 600

using namespace std;
using namespace sdl2_lib;

int main(int, char **)
{
	auto var = core();

	Screen Display;
	Display.create("Demo", SX, SY, SDL_WINDOW_RESIZABLE, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Display.setLogicalFPS(60);
	
	// Image that fails to load are just 'pink' (255,128,128)
	Image test(&Display, "no_image");
	// test.setPos(0,0);
	// test.setSize(SX,SY);

	SDL_RenderSetLogicalSize(Display.getRender(), SX, SY);

	while (Display.Do())
	{
		auto space = Display.key(SDLK_SPACE);
		if (space & OnNewPress)
			cout << "OnNewPress" << endl;
		if (space & OnPress)
			cout << "OnPress" << endl;
		
		test.draw();
		// affichage ?
	}
	// sdl_MessageError("toto");

	return 0;
}
