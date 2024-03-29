/**
 * @file lib_screen.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Wrapper around screen, events and FPS-limitation logics 
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SCREEN_H
#define SCREEN_H
#include <string>

#include "lib_core.h"

using namespace std;

namespace sdl2_lib
{

#define FPS_CALC_REFRESH 30
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
    void name(const char *title);

    /**
     * @brief Set a upper FPS 'limit' (automaticly sleep if we have some spare times >=2ms)
     * @param FPS Targeted FPS :)
     */
    void setFPS(int FPS);
    /**
     * @brief Get the computed FPS (over few ms and few frames)
     * @return int
     */
    int getFps() const;
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
    double getLogicalMult() const;

    int Do();
    /**
     * @brief get the last pressed key, if it happened after the given ms
     *
     * @param From : the ms from which we accept the last input
     * @return int : the keycode
     */
    int lastPressed(unsigned int From = 0) const;
    /**
     * @brief check if the key state match the event type
     *
     * @param Key the keycode you want to query
     * @param eventType the type of the event
     * @return int
     */
    int key(int Key, int eventType = Pressed | OnPress | OnNewPress);
    bool hasWindowEventHappened() const;
    unsigned int getTime() const
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
    int hasDrop() const
    {
        return (DropsCount > 0);
    }
    const char * getDrop()
    {
        if (hasDrop())
            return DropsData[--DropsCount];
        else
            return 0;
    }
    /**
     * @brief Draw a line
     * 
     * @param x1 define first point
     * @param y1 define first point
     * @param x2 define second point
     * @param y2 define second point
     */
    void drawLine(int x1, int y1, int x2, int y2);
    /**
     * @brief Draw a point
     * 
     * @param x define point
     * @param y define point
     */
    void drawPoint(int x, int y);

    inline void countDraw() {
        this->drawCalls ++;
    };

    int Mousex, Mousey;
    int Mousevx, Mousevy;
    int Mouser, Mousel, Mousem; /// Touche lie aux cliques souris
    int Sx, Sy;

private:
    void init(int x, int y);
    void calc_fps();
    void event();
    void update();
    void destroy();

    int DropsCount;
    char DropsData[MAX_DROP][512];
    SDL_Window *Window;
    SDL_Renderer *Render;


    SDL_Event E;
    int Open, Frame, FPS;

    unsigned int LastPressed, MsLastPressed, Double;

    int Clavier[SDLK_COUNT]; /// Conteneur des touches clavier
    unsigned int Before;
    unsigned int TARGET_LAST;
    unsigned int TARGET_TIME;
    unsigned int TARGET_LOGICFPS;
    double TARGET_MULT;

    int Wdevent;
    int drawCalls;
};

} // namespace sdl2_lib

#endif // sdl_SCREEN_H
