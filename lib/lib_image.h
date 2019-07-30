/**
 * @file image.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Wrapper around Image/Texture
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 *
 * Allows storing image in a much simpler manner,
 * Extends object, and implement it's internal missing display function
 */

#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <iostream>

#include "lib_core.h"
#include "lib_object.h"
#define PREC 10

namespace sdl2_lib
{

class Screen;

/**
 * @brief Allow to duplicate references of the same texture and intelligently implement 
 * 
 */
class Texture
{
public:
    Texture(SDL_Renderer *Rend, SDL_Surface *O)
    {
        this->w = O->w;
        this->h = O->h;
        this->Orig = SDL_CreateTextureFromSurface(Rend, O);
        this->nbRef = 1;
    }
    ~Texture()
    {
        if (nbRef > 0)
        {
            PRINT_LVL("WARNING: Trying to delete a texture that is still being used !" , -1);
        }
        SDL_DestroyTexture(this->Orig);
    }

    void addRef()
    {
        nbRef++;
    }
    void delRef()
    {
        nbRef--;
        if (nbRef <= 0)
            delete this;
    }

    SDL_Texture *Orig;
    int h, w;
    int nbRef;

protected:
};

inline void TextInit(Texture &O)
{
    O.Orig = 0;
    O.h = O.w = O.nbRef = 0;
}

class Image : public Object
{
public:
    /// Fonction de liberation
    void Free();
    ~Image();

    /// Attention le delete de la surface est integree
    void Change(Surface *In);
    Image(Screen *S, Surface *From);
    Image(Screen *S, const char *);

    /// Attention ne copie pas la vram, il ne faut pas liberer si il y a encore des enfants ...
    /// Securise par des fonctionnalitees internes.
    Image &operator=(Image &Orig);
    Image(Image *Img);
    Image(Image &Img);

/*
    virtual void draw();
    virtual void draw(int x, int y, int Ang);
    virtual void draw(int x, int y, int ang, int R, int G, int B, int FLIP);
    void drawOrigAng(int Sx, int Sy, int Dx, int Dy, int Angx, int Angy, int Angz, double Zoom);
*/
    virtual void setColorFilter(int R, int G, int B)
    {
        this->R = R;
        this->G = G;
        this->B = B;

        needsRGB = (R != 255) || (G != 255) || (B != 255);
    }
    /**
     * @brief Get the Name of the 'image'
     * @return char* 
     */
    virtual const char *getName()
    {
        return Name;
    }
    /**
     * @brief Get the Texture of the 'image'
     * @return Texture* 
     */
    const Texture *getTexture()
    {
        return texture;
    }

protected:
    char Name[128];
    bool needsRGB = false;
    uint8_t R=255, G=255, B=255;
    Texture *texture;

    virtual void internal_draw(int x, int y, double Ang, Flip FLIP);
    virtual void internal_draw(int x, int y, int Ox, int Oy, double Ang, Flip FLIP);
private:
    static Surface *_innernLoadImg(const char *);
    void Init();
    Screen *screen;
};
} // namespace sdl2_lib

#endif // Image_H
