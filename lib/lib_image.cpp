#include "SDL2/SDL_image.h"
#include <iostream>

#include "lib_image.h"
#include "lib_screen.h"
#include "lib_util.h"

lib_Surface* lib_Image::_empty = lib_SurfaceRGB(64,64,255,128,128);

lib_Surface* lib_Image::_innernLoadImg ( const char * filename )
{
    lib_Surface * out = IMG_Load ( filename );
    if ( out == NULL )
    {
        if ( _empty == NULL )
            _empty = lib_SurfaceRGB(64,64,255,128,128);
        out = _empty;
    }
    return out;
}

void lib_Image::Free()
{
    // If we don't have a texture we should do nothing
    if ( !texture )
        return;

    // Else notify the texture we are not using it anymore
    texture->delRef();
    // And remove it from this object
    texture = NULL;
    Name[0] = 0;
}
void lib_Image::Init()
{
    Name[0] = 0;
    Zx = 1;
    Zy = 1;
    Fx = 0;
    Fy = 0;
    W = 0;
    H = 0;
    bx = 0;
    by = 0;

    D.x = 0;
    D.y = 0;
    D.w = 0;
    D.h = 0;

    Centered = 0;
    texture = 0;
}
lib_Image::~lib_Image ()
{
    Free();
}


lib_Image::lib_Image ( lib_Image * Img )
{
    (*this) = (*Img);
}
lib_Image::lib_Image ( lib_Image & Img )
{
    (*this) = Img;
}

lib_Image::lib_Image ( lib_Screen * S, lib_Surface * From )
{
    Init();
    screen = S;
    Change(From);
}
lib_Image::lib_Image (lib_Screen * S, const char * name )
{
    Init();
    screen = S;
    strcpy(Name,name);
    Change(_innernLoadImg(name));
}

void lib_Image::Change ( lib_Surface * surf )
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
    texture = new lib_Texture (screen->getRender(),surf);
#if DEBUG >= 2
    if (texture == 0)
        cout << "Texture is NULL !" << endl;
#endif // DEBUG
    SDL_FreeSurface(surf);
    setZoom(1);
}

lib_Image & lib_Image::operator= (lib_Image & Orig)
{
    Init();
    texture = Orig.texture;
    texture->addRef();
    screen = Orig.screen;
    setCenter(Orig.Centered);
    strcpy(Name,Orig.Name);
    //setZoom(100,100);
    /*
    cout << endl;
    cout << Zx << " x " << Zy << endl;
    cout << W << " x " << H << endl;
    cout << w << " x " << h << endl;
    cout << bx << " x " << by << endl;
    */
    return (*this);
}

void lib_Image::setZoomx( double Zoom )
{
    Zx = Zoom;
    W = Zx*w();
    if (Centered)
    {
        bx = -W/2;
    }
    D.w = W;
}
void lib_Image::setZoomy( double Zoom )
{
    Zy = Zoom;
    H = Zy*h();
    if (Centered)
    {
        by = -H/2;
    }
    D.h = H;
}
void lib_Image::setZoom( double Zoomx, double Zoomy )
{
    setZoomx( Zoomx );
    setZoomy( Zoomy );
}
void lib_Image::setZoom( double Zoom )
{
    Zx = Zoom;
    Zy = Zoom;

    H = Zy*h();
    W = Zx*w();
    if (Centered)
    {
        bx = -W/2;
        by = -H/2;
    }
    D.w = W;
    D.h = H;
}
void lib_Image::setSize ( int W, int H )
{
    this->W = W;
    this->H = H;
    Fx = W;
    Fy = H;
    D.w = W;
    D.h = H;
    if (Centered)
    {
        bx = -W/2;
        by = -H/2;
    }
}
void lib_Image::setCenter ( bool t )
{
    if (t)
        Centered = 1;
    else
        Centered = 0;
    setZoom(Zx);
    setZoom(Zy);
}



void lib_Image::drawOrigAng (int Sx, int Sy, int Dx, int Dy, int Angx, int Angy, int Angz, double Zoom)
{
    SDL_Point from;

    int Zx = D_cos(Angx)*Zoom/100,Zy = D_sin(Angy)*Zoom/100;
    Angz = (Angz*360) / DATAMAX;


    SDL_RendererFlip Flip = SDL_FLIP_NONE;


    D.x = Dx * Zx / 100;
    D.y = Dy * Zy / 100;

    D.w = w()*Zx/100;
    D.h = h()*Zy/100;
    if (D.w < 0)
    {
        D.w = -D.w;
        if (D.h < 0)
            Angz = (Angz+180)%360;
        else
            Flip = SDL_FLIP_HORIZONTAL;
    }
    else if (D.h < 0)
    {
        D.h = -D.h;
        Flip = SDL_FLIP_VERTICAL;
    }

    from.x = -D.x+D.w/2;
    from.y = -D.y+D.h/2;
    D.x += Sx - D.w/2;
    D.y += Sy - D.h/2;

    SDL_RenderCopyEx(screen->getRender(),texture->Orig,0,&D,Angz,&from,Flip);
}

void lib_Image::draw ( int x, int y, int ang, int R, int G, int B, int FLIP )
{
    D.x = x + bx;
    D.y = y + by;
    SDL_SetTextureColorMod(texture->Orig,R,G,B);
    SDL_RenderCopyEx(screen->getRender(),texture->Orig,0,&D,ang,0,(SDL_RendererFlip)FLIP);
}

void lib_Image::draw ( int x, int y, int ang )
{
    D.x = x + bx;
    D.y = y + by;
    //cout << D.x << ";" << D.y << " " << W << "x" << H << endl;
    SDL_RenderCopyEx(screen->getRender(),texture->Orig,0,&D,ang,0,SDL_FLIP_NONE);
}

void lib_Image::draw ( int x, int y )
{
    D.x = x + bx;
    D.y = y + by;
    //cout << D.x << ";" << D.y << " " << W << "x" << H << endl;
    SDL_RenderCopy(screen->getRender(),texture->Orig,0,&D);
}