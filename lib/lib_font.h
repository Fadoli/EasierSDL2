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

    Surface *text(const char *text, int R = 0, int G = 0, int B = 0);
    Surface *utf8(const char *text, int R = 0, int G = 0, int B = 0);
    Surface *textShadow(const char *text, int Deca, int R = 0, int G = 0, int B = 0);
    Surface *textNewLine(const char *text, int Scale, int lineSize, int R = 0, int G = 0, int B = 0);

private:
    TTF_Font *self;
};
} // namespace sdl2_lib

#endif // font_H
