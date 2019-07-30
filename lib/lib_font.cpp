#include "lib_font.h"

namespace sdl2_lib
{

font::font(Screen *, const char *FontFile, int FontSize)
{
    self = TTF_OpenFont(FontFile, FontSize);
}

font::~font()
{
    if (self)
        TTF_CloseFont(self);
    self = NULL;
}

/**
 * @brief get internal font
 * @return const TTF_Font* 
 */
const TTF_Font *font::getFont()
{
    return self;
}

/// Le texte
Surface *font::text(const char *text, int R, int G, int B)
{
    if (text[0])
    {
        SDL_Color Col;
        Col.r = R;
        Col.b = B;
        Col.g = G;
        Col.a = 255;
        Surface *loc;
        if ((loc = TTF_RenderText_Blended(self, text, Col)) == 0)
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
Surface *font::utf8(const char *text, int R, int G, int B)
{
    if (text[0])
    {
        SDL_Color Col;
        Col.r = R;
        Col.b = B;
        Col.g = G;
        Col.a = 255;
        Surface *loc;
        if ((loc = TTF_RenderUTF8_Blended(self, text, Col)) == 0)
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
Surface *font::textShadow(const char *text, int Deca, int R, int G, int B)
{
    Surface *Text = this->text(text, R, G, B);
    Surface *Out = this->text(text, 255 - R, 255 - G, 255 - B);
    SDL_Rect pos;
    pos.x = -Deca;
    pos.y = -Deca;

    SDL_SetSurfaceBlendMode(Text, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(Text, 0, Out, &pos);

    SDL_FreeSurface(Text);
    return Out;
}
Surface *font::textNewLine(const char *text, int Scale, int MX, int R, int G, int B)
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
            lcl = this->text(word, R, G, B);
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

} // namespace sdl2_lib