#include "util.h"
#include "core.h"

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

/// Le texte
Surface *sdl_TextCreate(TTF_Font *Police, const char *text, int R, int G, int B)
{
    if (text[0])
    {
        SDL_Color Col;
        Col.r = R;
        Col.b = B;
        Col.g = G;
        Col.a = 255;
        Surface *loc;
        if ((loc = TTF_RenderText_Blended(Police, text, Col)) == 0)
        {
            sdl_MessageError("3 : impossible de charger un texte :( !");
        }
        return loc;
    }
    else
    {
        return SurfaceRGB(1, 1, 0, 0, 0);
    }
}
Surface *sdl_UTF8Create(TTF_Font *Police, const char *text, int R, int G, int B)
{
    if (text[0])
    {
        SDL_Color Col;
        Col.r = R;
        Col.b = B;
        Col.g = G;
        Col.a = 255;
        Surface *loc;
        if ((loc = TTF_RenderUTF8_Blended(Police, text, Col)) == 0)
        {
            sdl_MessageError("3 : impossible de charger un texte :( !");
        }
        return loc;
    }
    else
    {
        return SurfaceRGB(1, 1, 0, 0, 0);
    }
}
Surface *sdl_TextCreateShadow(TTF_Font *Police, const char *text, int R, int G, int B, int Deca)
{
    Surface *Text = sdl_TextCreate(Police, text, R, G, B);
    Surface *Out = sdl_TextCreate(Police, text, 0, 0, 0);
    SDL_Rect pos;
    pos.x = -Deca;
    pos.y = -Deca;

    SDL_SetSurfaceBlendMode(Text, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(Text, 0, Out, &pos);

    SDL_FreeSurface(Text);
    return Out;
}
Surface *sdl_TextCreateMaxX(TTF_Font *Police, const char *text, int Scale, int R, int G, int B, int MX)
{
    int i = 0, x = 0, y = 0;
    char word[64];
    int Ligne = 2 + (strlen(text) * Scale) / MX;
    Surface *Out = SurfaceRGBA(MX, Ligne * Scale, 0, 0, 0, 0);
    //SDL_SetSurfaceAlphaMod(Out,SDL_ALPHA_TRANSPARENT);
    SDL_SetSurfaceBlendMode(Out, SDL_BLENDMODE_ADD);
    Surface *lcl = 0;
    SDL_Rect pos;
    /// On initialise les variables.
    char car = text[0];
    int Size = -1;
    word[0] = '\0';

    while (car != '\0') /// tant qu'on est pas a la fin
    {
        car = text[i];
        /// Si le caractere n'est pas \ ce qui permet de mettre des \n dans le texte pour couper

        if (car == ' ' || car == '\\' || car == '\0')
        /// On coupe les mots aux espace aux \ et a la fin du texte
        {
            /// Create Texte pour afficher le texte qu'on vien de detacher du reste
            word[Size + 1] = ' ';
            word[Size + 2] = '\0';
            lcl = sdl_TextCreate(Police, word, R, G, B);
            Size = -1;
            word[0] = '\0';

            /// Si il n'y as pas assez de place ou qu'on souhaite force un retour a la ligne '\n'
            /// Ici on change la position du curseur de dessin du texte en fonction de si on a la place ou non
            if (x + lcl->w > MX || (car == '\\' && text[i + 1] == 'n'))
            {
                if (car == '\\' && text[i + 1] == 'n')
                {
                    i++;
                    word[Size + 1] = '\0';
                }
                y += Scale + 2;
                pos.x = 0;
                x = lcl->w;
            }
            else
            {
                pos.x = x;
                x += lcl->w;
            }
            pos.y = y;
            /// On affiche la partie du texte dans l'image.
            SDL_BlitSurface(lcl, 0, Out, &pos);
            SDL_FreeSurface(lcl);
        }
        else
        {
            Size++;
            word[Size] = car;
        }
        i++;
    }
    //SDL_SetSurfaceBlendMode(Out,SDL_BLENDMODE_ADD);
    return Out;
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