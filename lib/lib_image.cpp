#include "SDL2/SDL_image.h"
#include <iostream>
#include <string.h>

#include "lib_screen.h"
#include "lib_image.h"
#include "lib_util.h"

namespace sdl2_lib
{

Surface *Image::_innernLoadImg(const char *filename)
{
    Surface *out = IMG_Load(filename);
    if (out == NULL)
    {
        PRINT("Failed loading image : " << filename);
        out = SurfaceRGB(64, 64, 255, 128, 128);
    }
    return out;
}

void Image::Free()
{
    // If we don't have a texture we should do nothing
    if (!texture)
        return;

    // Else notify the texture we are not using it anymore
    texture->delRef();

    // And reinit this ;)
    Init();
}
void Image::Init()
{
    Name[0] = 0;
    texture = 0;
}
Image::~Image()
{
    Free();
}

Image::Image(Image *Img)
{
    Init();
    (*this) = (*Img);
}
Image::Image(Image &Img) : Object()
{
    Init();
    (*this) = Img;
}
Image::Image(Screen *S, Surface *From)
{
    Init();
    screen = S;
    Change(From);
}
Image::Image(Screen *S, const char *name)
{
    Init();
    screen = S;
    strcpy(Name, name);
    Change(_innernLoadImg(name));
}

void Image::Change(Surface *surf)
{
#if DEBUG >= 1
    /// Debugage Niveau 1.
    if (surf == 0)
    {
        cout << "Surface given is NULL !" << endl;
        return;
    }
#if DEBUG >= 2
    /// Debugage Niveau 2.
    if (screen->getRender() == 0)
    {
        cout << "Screen renderer is NULL !" << endl;
        return;
    }
#endif // DEBUG
#endif // DEBUG
    Free();
    texture = new Texture(screen->getRender(), surf);
    Reset(texture->w, texture->h, 0);
#if DEBUG >= 2
    if (texture == 0)
        cout << "Texture is NULL !" << endl;
#endif // DEBUG
    SDL_FreeSurface(surf);
}

Image &Image::operator=(Image &Orig)
{
    Free();
    texture = Orig.texture;
    texture->addRef();
    screen = Orig.screen;
    strcpy(Name, Orig.Name);
    return (*this);
}

void Image::internal_draw(int x, int y, int Ox, int Oy, double Ang, Flip FLIP)
{
    SDL_Point from;
    SDL_Rect drawTo;

    drawTo.x = x;
    drawTo.y = y;
    drawTo.w = w;
    drawTo.h = h;

    from.x = -Ox - deltax;
    from.y = -Oy - deltay;

    if (needsRGB)
        SDL_SetTextureColorMod(texture->Orig, R, G, B);
    SDL_RenderCopyEx(screen->getRender(), texture->Orig, 0, &drawTo, Ang, &from, (SDL_RendererFlip)FLIP);
}

void Image::internal_draw(int x, int y, double Ang, Flip FLIP)
{
    SDL_Rect drawTo;
    drawTo.x = x;
    drawTo.y = y;
    drawTo.w = w;
    drawTo.h = h;
    if (needsRGB)
        SDL_SetTextureColorMod(texture->Orig, R, G, B);
    SDL_RenderCopyEx(screen->getRender(), texture->Orig, 0, &drawTo, Ang, 0, (SDL_RendererFlip)FLIP);
}
} // namespace sdl2_lib
