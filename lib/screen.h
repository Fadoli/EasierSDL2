/*!
 *\file screen.h
 *\brief surcouche de gestion de l'ecran et de ses evenements
 *\author Franck Mourre
 *\version 1
 *\date Octobre 2016
 */

#ifndef SCREEN_H
#define SCREEN_H
#include <string>

#include "core.h"

using namespace std;

namespace sdl2_lib
{

#define FPS_CALC_REFRESH 20
#define SDLK_COUNT 283
#define MAX_DROP 32
#define CONVERT SDLK_CAPSLOCK

// Permet de definir les evenements
#define OnNewPress 0b100
#define OnPress 0b10
#define Pressed 0b1
#define NotPressed 0

class Screen
{
public:
    ~Screen();
    Screen();
    void create(string title, int x, int y);
    void create(string title, int x, int y, Uint32 Winflag, Uint32 Renflag);
    void recreate(Uint32 Renflag);
    void name(string title);
    void name(char *title);
    void update();
    void destroy();
    int isOpen();

    /**
     * @brief Set a upper FPS 'limit' (automaticly sleep if we have some spare times >=2ms) 
     * @param FPS Targeted FPS :)
     */
    void setFPS(int FPS);
    /**
     * @brief Get the computed FPS (over few ms and few frames)
     * @return int 
     */
    int getFps();
    /**
     * @brief Set the 'logical' FPS : it can be used to make your game not frame dependant
     * @param FPS 
     */
    void setLogicalFPS(int FPS);
    /**
     * @brief Get the double corresponding to the % of the frame compared to your target :
     * If the frame gets rendered twice as fast, will output 0.5
     * If the frame is reduced by half, will output 2
     * @return double the 'frame-time' multiplier
     */
    double getLogicalMult();

    void event();
    int Do();
    int lastPressed();                  // retourne la derniere touche utilisee ( inutile ? )
    int lastPressed(unsigned int From); // Retourne la derniere touche utilise SI utilise apres From
    int key(int Key);
    int HasWindowEventHappened();
    unsigned int getTime()
    {
        return TARGET_LAST;
    }

    SDL_Window *getWindow()
    {
        return Window;
    }
    SDL_Renderer *getRender()
    {
        return Render;
    }
    int hasDrop()
    {
        return (DropsCount > 0);
    }
    char *getDrop()
    {
        if (hasDrop())
            return DropsData[--DropsCount];
        else
            return 0;
    }

    int Mousex, Mousey;
    int Mousevx, Mousevy;
    int Mouser, Mousel, Mousem; /// Touche lie aux cliques souris
    int Sx, Sy;

private:
    int DropsCount;
    char DropsData[MAX_DROP][256];
    SDL_Window *Window;
    SDL_Renderer *Render;

    void init(int x, int y);
    void calc_fps();

    SDL_Event E;
    int Open, Frame, FPS;

    unsigned int LastPressed, MsLastPressed, Double;

    int Clavier[SDLK_COUNT]; /// Conteneur des touches clavier
    unsigned int Before;

    unsigned int TARGET_LAST;
    unsigned int TARGET_FPS;
    unsigned int TARGET_TIME;

    unsigned int TARGET_LOGICFPS;
    double TARGET_LOGIC;
    double TARGET_MULT;
    int Wdevent;
};

} // namespace sdl2_lib

#endif // sdl_SCREEN_H
