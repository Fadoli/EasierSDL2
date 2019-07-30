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

} // namespace sdl2_lib