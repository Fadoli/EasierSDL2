#include "lib_util.h"
#include "lib_core.h"
#include <string.h>

#define DEFAULT_SDL SDL_INIT_EVERYTHING
#define DEFAULT_IMG (IMG_INIT_JPG | IMG_INIT_PNG)

namespace sdl2_lib
{

core::core(int sdlFlags, int imgFlags)
{
    if (!sdlFlags)
        sdlFlags = DEFAULT_SDL;
    if (!imgFlags)
        imgFlags = DEFAULT_IMG;
    SDL_Init(sdlFlags);
    IMG_Init(imgFlags);
    //InitData();
    TTF_Init();
}
core::~core()
{
    //deleteData();
    IMG_Quit();
    SDL_Quit();
}

/**
     * @brief Create a fully colored Surface from the specified size & color
     *
     * @param w width
     * @param h height
     * @param R Red color
     * @param G Green color
     * @param B Blue color
     * @return Surface*
     */
Surface *SurfaceRGB(int w, int h, int R, int G, int B)
{
    Surface *tmp;
    if ((tmp = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0)) != 0)
    {
        SDL_FillRect(tmp, 0, SDL_MapRGBA(tmp->format, R, G, B, 255));
        return tmp;
    }
    return 0;
}
/**
     * @brief Create a fully colored Surface from the specified size & color with Alpha
     *
     * @param w width
     * @param h height
     * @param R Red color
     * @param G Green color
     * @param B Blue color
     * @param A Alpha canal
     * @return Surface*
     */
Surface *SurfaceRGBA(int w, int h, int R, int G, int B, int A)
{
    Surface *tmp;
    if ((tmp = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0)) != 0)
    {
        SDL_FillRect(tmp, 0, SDL_MapRGBA(tmp->format, R, G, B, A));
        SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_ADD);
        return tmp;
    }
    return 0;
}

/// Fonction des pixels
void putpixel(Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;

    default:
        return; /* shouldn't happen, but avoids warnings */
    }
}

void sdlChangeSCKUNO(Surface *In, int R, int G, int B)
{
    if (In != 0)
    {
        SDL_SetColorKey(In, SDL_TRUE, SDL_MapRGB(In->format, R, G, B));
        return;
    }
    return;
}

void sdl_MessageError(const char *msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", msg, 0);
}

} // namespace sdl2_lib
