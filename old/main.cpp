#include <iostream>
#include <math.h>
#include "lib_core.h"
#include "lib_screen.h"
#include "lib_image.h"

#define SX 1200
#define SY 600

using namespace std;
using namespace sdl2_lib;

int main(int, char **)
{
	auto var = core();

	Screen Display;
	Display.create("Demo", SX, SY, SDL_WINDOW_RESIZABLE, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Display.setLogicalFPS(100);
	//Display.setFPS(100);

	// Image that fails to load are just 'pink' (255,128,128)
	Image test(&Display, "no_image");
	test.setCenter();
	Image test2(&Display, "no_image");
	test2.setCenter();
	// test.setPos(0,0);
	// test.setSize(SX,SY);

	double x = 0, y = 0;
	int lastFPS = 0;
	double player_speed = 3;
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
		x += (Display.key(SDLK_RIGHT, Pressed) - Display.key(SDLK_LEFT, Pressed)) * mult * player_speed;
		y += (Display.key(SDLK_DOWN, Pressed) - Display.key(SDLK_UP, Pressed)) * mult * player_speed;
		test.setPos(x, y);
		test.setAng(base*180);
		test.draw();

		for(int i =0; i < SX + 64 ; i += 30)
		{
			test.drawAt(i,SY/2 + 200 * sin(base + (double) i / 100),180/PI*(base + (double) i / 100));
		}


		/*
		test2.setPos(x+200, y);
		test2.setAng(base*180);
		test2.draw(32,32);
		*/
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
			double sizeMult = 5+15*size*sin(2*base) + 40*cos(base*5);
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
