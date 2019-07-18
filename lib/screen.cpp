#include <iostream>
#include <string.h>

#include "screen.h"
#include "util.h"
#include "image.h"

namespace sdl2_lib
{
void Screen::init(int X, int Y)
{
    Open = 1;
    Wdevent = 0;
    Before = SDL_GetTicks();
    LastPressed = 0;
    MsLastPressed = Before;
    Double = 0;
    Mousel = 0;
    Mouser = 0;
    Mousem = 0;
    Mousex = 0;
    Mousey = 0;

    TARGET_LAST = SDL_GetTicks();
    TARGET_TIME = 0;
    TARGET_MULT = 0;
    TARGET_LOGICFPS = 0;
    FPS = 0;
    Frame = 0;
    DropsCount = 0;

    Sx = X;
    Sy = Y;

    for (int i = 0; i < SDLK_COUNT; i++)
        Clavier[i] = NotPressed;
}

void Screen::setLogicalFPS(int A)
{
    TARGET_LOGICFPS = A;
}
void Screen::setFPS(int A)
{
    TARGET_TIME = (1000 / A);
}
void Screen::update()
{
    // Just updating & cleaning the screen here.
    PRINT_LVL("Screen::update : Render clear", 4);
    SDL_RenderPresent(Render);
    SDL_RenderClear(Render);

    // Limit FPS
    PRINT_LVL("Screen::update : Limit FPS", 4);
    unsigned int elapsed = SDL_GetTicks() - TARGET_LAST;

    int Remain = TARGET_TIME - elapsed;
    if (Remain >= 1)
        SDL_Delay(Remain);

    // Compute multiplier
    PRINT_LVL("Screen::update : Compute multiplier", 4);
    elapsed = SDL_GetTicks() - TARGET_LAST;
    if (TARGET_LOGICFPS)
    {
        TARGET_MULT = ((double)(elapsed * TARGET_LOGICFPS) / 1000);
    }
    TARGET_LAST = SDL_GetTicks();
}
double Screen::getLogicalMult() const
{
    return TARGET_MULT;
}

void Screen::create(string title, int x, int y)
{
    create(title, x, y, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    init(x, y);
}

void Screen::create(string title, int x, int y, Uint32 Winflag, Uint32 Renflag)
{
    Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x, y, Winflag);
    Render = SDL_CreateRenderer(Window, -1, Renflag);
    init(x, y);
}
Screen::Screen()
{
    Window = 0;
    Render = 0;
}
Screen::~Screen()
{
    destroy();
}

void Screen::recreate(Uint32 Renflag)
{
    SDL_DestroyRenderer(Render);
    Render = SDL_CreateRenderer(Window, -1, Renflag);
}

void Screen::destroy()
{
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Render);
}

void Screen::name(const char *name)
{
    SDL_SetWindowTitle(Window, name);
}
void Screen::name(string name)
{
    SDL_SetWindowTitle(Window, name.c_str());
}

int Screen::key(int Key, int eventType)
{
    int Id = (Key < 128) ? Key : Key - CONVERT;
    int lcl = Clavier[Id];
    if (Clavier[Id] & OnPress)
    {
        Clavier[Id] = Pressed;
    }
    return lcl & eventType;
}
bool Screen::hasWindowEventHappened() const
{
    return !!Wdevent;
}

void Screen::event()
{
    unsigned int Time = SDL_GetTicks();
    char *Fpath = 0;
    int key;
    while (SDL_PollEvent(&E))
    {
        PRINT_LVL("In event" << E.type, 5);
        switch (E.type)
        {
        case SDL_DROPFILE: // Drag & Drop
            Fpath = E.drop.file;
            if (DropsCount < MAX_DROP)
                strcpy(DropsData[DropsCount++], Fpath);
            delete Fpath;
            break;
        case SDL_KEYUP:
            key = (E.key.keysym.sym < 128) ? E.key.keysym.sym : E.key.keysym.sym - CONVERT;
            Clavier[key] = NotPressed;
            break;
        case SDL_KEYDOWN:
            key = (E.key.keysym.sym < 128) ? E.key.keysym.sym : E.key.keysym.sym - CONVERT;
            if (Clavier[key] == NotPressed)
                Clavier[key] = OnNewPress + OnPress + Pressed;
            else
                Clavier[key] = OnPress + Pressed;
            break;

        case SDL_MOUSEMOTION:
            Mousex = E.motion.x;
            Mousey = E.motion.y;
            break;
        case SDL_MOUSEBUTTONUP:
            switch (E.button.button)
            {
            case SDL_BUTTON_LEFT:
                Mousel = 0;
                break;
            case SDL_BUTTON_RIGHT:
                Mouser = 0;
                break;
            case SDL_BUTTON_MIDDLE:
                Mousem = 0;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            LastPressed = E.button.button;
            MsLastPressed = Time;
            switch (E.button.button)
            {
            case SDL_BUTTON_LEFT:
                if (!Mousel)
                    Mousel = OnPress;
                else
                    Mousel = Pressed;
                break;
            case SDL_BUTTON_RIGHT:
                if (!Mouser)
                    Mouser = OnPress;
                else
                    Mouser = Pressed;
                break;
            case SDL_BUTTON_MIDDLE:
                if (!Mousem)
                    Mousem = OnPress;
                else
                    Mousem = Pressed;
                break;
            default:
                break;
            }
            break;
        case SDL_QUIT:
            Open = 0;
            break;

        case SDL_WINDOWEVENT:
            switch (E.window.event)
            {
            case SDL_WINDOWEVENT_MOVED:
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_MAXIMIZED:
                Wdevent = 1;
            default:
                break;
            }
            break;
        default:
            break;
        }
        PRINT_LVL("Out event" << E.type, 5);
    }
}

void Screen::calc_fps()
{
    Frame++;
    unsigned int now = SDL_GetTicks();
    unsigned int dif = now - Before;
    if (Frame < FPS_CALC_REFRESH || dif < (10 * FPS_CALC_REFRESH))
        return;
    FPS = (1000 * Frame) / dif;
    Before = now;
    Frame = 0;
}

int Screen::getFps() const
{
    return FPS;
}

int Screen::Do()
{
    int Mouselx = Mousex, Mousely = Mousey;
    Mousel = !!Mousel;
    Mouser = !!Mouser;
    Wdevent = 0;

    PRINT_LVL("Screen::Do - calc_fps", 4);
    calc_fps();
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
    PRINT_LVL("Screen::Do - update", 4);
    update();
    PRINT_LVL("Screen::Do - event", 4);
    event();

    Mousevx = Mousex - Mouselx;
    Mousevy = Mousey - Mousely;
    return (Open == 1);
}

int Screen::lastPressed(unsigned int Bef) const
{
    return (Bef <= MsLastPressed) * LastPressed;
}

// Let's do some rendering things :)
void Screen::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(Render, x1, y1, x2, y2);
}
void Screen::drawPoint(int x, int y)
{
    SDL_RenderDrawPoint(Render, x, y);
}

} // namespace sdl2_lib
