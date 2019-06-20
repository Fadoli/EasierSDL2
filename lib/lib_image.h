/*!
 *\file lib_image.h
 *\brief Grosse surcouche encapsulante des images
 *\author Franck Mourre
 *\version 1
 *\date Octobre 2016
 *
 * Permet de stocker de maniere 'simple' les donnees d'une image
 * Permet de gerer des agrandissments
 * Permet l'affichage avec filtre RGB
 * Permet l'affichage avec flip / angle
 *
 */

#ifndef LIB_IMAGE_H
#define LIB_IMAGE_H
#include <string>
#include <iostream>

#include "lib_core.h"
#include "lib_object.h"
#define PREC 10

class lib_Screen;

class lib_Texture
{
public:
    lib_Texture ( SDL_Renderer * Rend, SDL_Surface * O )
    {
        this->w = O->w;
        this->h = O->h;
        this->Orig = SDL_CreateTextureFromSurface(Rend,O);
        this->nbRef = 1;
    }
    ~lib_Texture ()
    {
        if ( nbRef > 0 )
        {
            cout << "Trying to delete a texture that is still being used ?" << endl;
        }
        SDL_DestroyTexture(this->Orig);
    }

    void addRef()
    {
        nbRef ++;
    }
    void delRef()
    {
        nbRef --;
        if (nbRef <= 0)
            delete this;
    }

    SDL_Texture * Orig;
    int h,w;
    int nbRef;
protected:
};

inline void lib_TextInit( lib_Texture & O )
{
    O.Orig = 0;
    O.h = O.w = O.nbRef = 0;
}

class lib_Image : public lib_Object
{
public :
    /// Fonction de liberation
    void Free ();
    ~lib_Image ();

    /// Attention le delete de la surface est integree
    void Change ( lib_Surface * In);
    lib_Image ( lib_Screen * S = 0, lib_Surface * From = 0 );
    lib_Image ( lib_Screen * S, const char * );
    /// Attention ne copie pas la vram, il ne faut pas liberer si il y a encore des enfants ...
    /// Securise par des fonctionnalitees internes.
    lib_Image & operator= ( lib_Image & Orig);
    lib_Image ( lib_Image * Img );
    lib_Image ( lib_Image & Img );

    void drawOrigAng ( int Sx, int Sy, int Dx, int Dy, int Angx, int Angy, int Angz, double Zoom);
    /// Permet de changer le % de zoom celon la precision souhaite en entree ( ensuite en 2^PREC ).

    virtual void setZoomx( double zoom );
    virtual void setZoomy( double zoom );
    virtual void setZoom( double zoomx, double zoomy );
    virtual void setZoom( double zoom );

    virtual void setSize( int w, int h );
    virtual void setCenter ( bool t = true );

    virtual void draw   ( int x, int y );
    virtual void draw   ( int x, int y, int Ang );
    virtual void draw   ( int x, int y, int ang, int R, int G, int B, int FLIP);

    virtual char * getName ()
    {
        return Name;
    }

    void affiche ()
    {
        cout << "Dimension " << W << ";" << H << endl;
    }

    int w ()
    {
        if ( !texture )
            return 0;
        return texture->w;
    }
    int h ()
    {
        if ( !texture )
            return 0;
        return texture->h;
    }
    int getX()
    {
        //cout << "getX : " << D.x << endl;
        return D.x;
    }
    int getY()
    {
        //cout << "getY : " << D.y << endl;
        return D.y;
    }

    long int W,H;
    lib_Texture * getTexture()
    {
        return texture;
    }

    static lib_Surface * _empty;
protected :
    int bx,by,Fx,Fy;
    double Zx,Zy;
    int Centered;
    char Name [128];
    lib_Texture * texture;
    SDL_Rect D;
private :
    lib_Surface * _innernLoadImg (const char *);
    void Init();
    lib_Screen * screen;
};

#endif // lib_Image_H
