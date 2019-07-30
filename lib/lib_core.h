/**
 *\file core.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Wrapper around SDL2 initialisation 
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 * 
 * This part is really linked into SDL, in the goal to make other more independant,
 * Allows generating / modifying Surfaces as well as some TTF related function
 */

#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include "lib_util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

namespace sdl2_lib
{
typedef SDL_Surface Surface;

class core
{
public:
    /**
     * @brief Destroy the core object : Deinit SDL components
     */
    ~core();
    /**
     * @brief Construct a new core object : Initialise SDL components
     */
    core(int sdlFlags = undefined, int imgFlags = undefined);
};

Surface *SurfaceRGB(int x, int y, int r, int g, int b);
Surface *SurfaceRGBA(int x, int y, int r, int g, int b, int a);

void putpixel(Surface *surface, int x, int y, Uint32 pixel);
void sdlChangeSCKUNO(Surface *In, int R, int G, int B);
void sdl_MessageError(const char *msg);
} // namespace sdl2_lib

#endif // _H_INCLUDED
