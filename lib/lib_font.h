#ifndef font_H
#define font_H

#include <stdio.h>
#include "lib_core.h"
#include "lib_screen.h"

namespace sdl2_lib
{
class font
{
public:
    font(Screen *screen, const char *FontFile, int FontSize = 12);
    ~font();
    const TTF_Font *getFont();

private:
    TTF_Font *self;
};
} // namespace sdl2_lib

#endif // font_H
