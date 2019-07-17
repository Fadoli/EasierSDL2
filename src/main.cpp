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
	//Display.setFPS(100);

	// Image that fails to load are just 'pink' (255,128,128)
	Image test(&Display, "no_image");
	// test.setPos(0,0);
	// test.setSize(SX,SY);

	double x = 0, y = 0;
	int lastFPS = 0;
	double base = 0;
	SDL_RenderSetLogicalSize(Display.getRender(), SX, SY);

	while (Display.Do())
	{
		double mult = Display.getLogicalMult();
		base += mult / 100;
		// cout << "Mult : " << mult << endl;
		auto space = Display.key(SDLK_SPACE);
		if (space & OnNewPress)
			cout << "OnNewPress" << endl;
		if (space & OnPress)
			cout << "OnPress" << endl;
		x += (Display.key(SDLK_RIGHT, Pressed) - Display.key(SDLK_LEFT, Pressed)) * mult;
		y += (Display.key(SDLK_DOWN, Pressed) - Display.key(SDLK_UP, Pressed)) * mult;
		test.setPos(x, y);
		test.draw();

		/*
		for ( int i = 0; i < SX; i+=30) {
			int next = i + 30;
			Display.drawLine(i, SY/2 + SY/3 * cos(base+(double)i/360),next,SY/2 + SY/3 * cos(base+(double)(next)/360));
			Display.drawLine(i, SY/2 + SY/3 * cos(3*(base+(double)i/360)),next,SY/2 + SY/3 * cos(3*(base+(double)(next)/360)));
			Display.drawLine(i, SY/2 + SY/3 * cos(5*(base+(double)i/360)),next,SY/2 + SY/3 * cos(5*(base+(double)(next)/360)));
		}
		*/

		SDL_SetRenderDrawColor(Display.getRender(),255,255,255,255);
		for (int size = 2; size < 20 ; size ++)
		{
			double ratio = 2*PI/size;
			double from = 0;
			double to = base;
			double sizeMult = 5+15*size;
			for (int cur = 0; cur<size; cur++ )
			{
				from = ratio*(cur+1) + base;
				Display.drawLine(SX/2+sizeMult*cos(from),SY/2+sizeMult*sin(from),SX/2+sizeMult*cos(to),SY/2+sizeMult*sin(to));
				to = from;
			}
		}

		int fps = Display.getFps();
		if (lastFPS != fps)
		{
			lastFPS = fps;
			cout << "Avg fps : " << lastFPS << endl;
		}
	}
	// sdl_MessageError("toto");

	return 0;
}
