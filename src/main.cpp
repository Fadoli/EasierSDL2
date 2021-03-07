#include <stdio.h>
#include <math.h>
#include <time.h>
#include "lib_core.h"
#include "lib_screen.h"
#include "map.h"
#include "coz.h"

#define SX 1024
#define SY 768
using namespace sdl2_lib;

int main ( int argc, char ** argv )
{
	auto var = core();
    Map game;
    Screen Display;
    
    Display.create("Digger",SX,SY,SDL_WINDOW_RESIZABLE,SDL_RENDERER_ACCELERATED);
    Display.setFPS(144); // Sets an upper limit
    Display.setLogicalFPS(50);
    SDL_RenderSetLogicalSize(Display.getRender(),SX,SY);

    game.setDisplay(&Display);
    PRINT("Display set");
    game.tile_def("data/bloc.txt");
    PRINT("Data read");
    //game.generator(time(NULL),7500,500);
    game.generator(time(NULL),4000,250);
    PRINT("Map generated");

    double Size = 0.1;
    int Tsize = 64;
    game.setTileSize(Tsize,Tsize);
    game.setZoom(Size);
    Display.hasWindowEventHappened();

    double x=0,y=0,X=0,Y=0;
    double vx=0,vy=0,ax=0,ay=0;
    double t;
	int lastFPS = 0;

    while( Display.Do() )
    {
        COZ_PROGRESS;
		int fps = Display.getFps();
		if (lastFPS != fps)
		{
			lastFPS = fps;
			cout << "Avg fps : " << lastFPS << endl;
		}

        if (Display.key(SDLK_KP_PLUS) & OnPress)
        {
            Size += 0.1;
            if ( Size > 2 )
                Size = 2;
            game.setZoom(Size);
        }
        if (Display.key(SDLK_KP_MINUS) & OnPress)
        {
            Size -= 0.1;
            if ( Size < 0.1 )
                Size = 0.1;
            game.setZoom(Size);
        }
        if ( Display.hasWindowEventHappened() )
        {
            //Display.recreate(SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            //SDL_RenderSetLogicalSize(Display.Render,SX,SY);
            //game.setDisplay(&Display);
            //game.freeTile();
            //game.tile_def("data/bloc.txt");
            game.setZoom(Size);
        }
        t = Display.getLogicalMult();
        if ( t >= 1.2 )
        {
            //printf("lag ?\n");
        }
        /*
        if ( Joy_Enabled(Joy_Get(E,0)) )
        {
            x += E->Joy.J[0].Joy[0] / 2000;
            y += E->Joy.J[0].Joy[1] / 2000;
        }
        else
        {
            x += 1;
        }
        if (x < 0)
            x=0;
        else if ( x >= SX )
            x = SX-1;

        if (y < 0)
            y=0;
        else if ( y >= SY )
            y = SY-1;
        */


        ax = ((Display.key(SDLK_RIGHT)>0) - (Display.key(SDLK_LEFT)>0));
        ay = ((Display.key(SDLK_DOWN)>0) - (Display.key(SDLK_UP)>0));
        vx += (ax - vx/64)*t;
        vy += (ay - vy/64)*t;
        x += vx;
        y += vy;

        while ( x >= game.Sizex )
        {
            X++;
            x -= game.Sizex;
        }
        while ( x < 0 )
        {
            X--;
            x += game.Sizex;
        }
        while ( y >= game.Sizey )
        {
            Y++;
            y -= game.Sizey;
        }
        while ( y < 0 )
        {
            Y--;
            y += game.Sizey;
        }
        if ( X < 0 )
            X+=game.Mx;
        if ( X >= game.Mx )
            X-=game.Mx;

        if ( Y > 0 )
        {
            game.changeTile(X,Y,'v');
        }
        game.draw(X,x-Tsize/2,Y,y-Tsize/2);
    }

    return 0;
}
