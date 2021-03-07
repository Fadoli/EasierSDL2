/**
 * @file lib_font.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Wrapper around text rendering
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 *
 * Allows handling font rendering, actual draw functions and cache are not in here
 */

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
