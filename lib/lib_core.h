/*!
 *\file lib_core.h
 *\brief basic functionnality
 *\author Franck
 *\version 1
 *\date June 2019
 *
 * This part is really linked into SDL, in the goal to make other more independant,
 * Allows generating / modifying Surfaces as well as some TTF related function
 */

#ifndef LIB_CORE_H_INCLUDED
#define LIB_CORE_H_INCLUDED

#include <SDL2/SDL.h> 
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define UNDEFINED 0
#define undefined 0

using namespace std;
typedef SDL_Surface lib_Surface;

namespace lib {

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

}

lib_Surface * lib_SurfaceRGB ( int x, int y, int r, int g, int b );
lib_Surface * lib_SurfaceRGBA ( int x, int y, int r, int g, int b, int a);
#define sdl_MessageError(MESS) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR",MESS,0)

/// Les textes
lib_Surface * sdl_TextCreate (TTF_Font* Police,const char* text, int R, int G, int B);
lib_Surface * sdl_TextShadow (TTF_Font* Police,const char* text, int R, int G, int B, int Deca);
lib_Surface * sdl_UTF8Create (TTF_Font* Police,const char* UTF8, int R, int G, int B);
/// Permet de creer un texte sur plusieurs lignes si besoin.
lib_Surface * sdl_TextCreateMaxX (TTF_Font* Police,const char* text,int Scale, int R, int G, int B, int MX);

void putpixel(lib_Surface * surface, int x, int y, Uint32 pixel);
void sdlChangeSCKUNO (lib_Surface * In,int R,int G,int B);

#endif // lib__H_INCLUDED
