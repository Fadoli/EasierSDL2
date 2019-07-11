#include <iostream>

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
    TARGET_FPS = 0;
    TARGET_TIME = 0;
    TARGET_MULT = 0;
    TARGET_LOGIC = 0;
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
    TARGET_LOGIC = ((double)1000 / A);
}
void Screen::setFPS(int A)
{
    TARGET_FPS = A;
    TARGET_TIME = (1000 / A);
}
void Screen::FPS_DELAY()
{
    int Remain = (TARGET_LAST + TARGET_TIME) - SDL_GetTicks();

    while (Remain > 1)
    {
        SDL_Delay(1);
        Remain = (TARGET_LAST + TARGET_TIME) - SDL_GetTicks();
    }
    /*
    Better have more FPS than actually locking it doing nothing ;)
    while (Remain > 0)
    {
        Remain = (TARGET_LAST + TARGET_TIME) - SDL_GetTicks();
    }
    */
}
void Screen::update()
{
    // Just updating & cleaning the screen here.
    SDL_RenderPresent(Render);
    SDL_RenderClear(Render);

    // Do the fps related things
    if (TARGET_FPS)
    {
        FPS_DELAY();
    }
    if (TARGET_LOGICFPS)
    {
        TARGET_MULT = (double)(SDL_GetTicks() - TARGET_LAST) / TARGET_LOGIC;
    }
    TARGET_LAST = SDL_GetTicks();
}
double Screen::getLogicalMult()
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

void Screen::name(char *name)
{
    SDL_SetWindowTitle(Window, name);
}
void Screen::name(string name)
{
    SDL_SetWindowTitle(Window, name.c_str());
}

int Screen::key(int Key)
{
    int Id = (Key < 128) ? Key : Key - CONVERT;
    int lcl = Clavier[Id];
    if (Clavier[Id] & OnPress)
    {
        Clavier[Id] = Pressed;
    }
    return lcl;
}
void Screen::event()
{
    unsigned int Time = SDL_GetTicks();
    char *Fpath = 0;
    int key;
    while (SDL_PollEvent(&E))
    {
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
    }
}

int Screen::HasWindowEventHappened()
{
    if (Wdevent)
    {
        Wdevent = 0;
        return 1;
    }
    return 0;
}

int Screen::isOpen()
{
    return (Open == 1);
}

void Screen::calc_fps()
{
    Frame++;
    unsigned int now = SDL_GetTicks();
    unsigned int dif = now - Before;
    if (Frame < FPS_CALC_REFRESH || dif < (10*FPS_CALC_REFRESH))
        return;
    //cout << "had " << Frame << " in " << dif << "ms" << endl;
    FPS = (1000 * Frame) / dif;
    Before = now;
    Frame = 0;
}

int Screen::getFps()
{
    return FPS;
}

int Screen::Do()
{
    int Mouselx = Mousex, Mousely = Mousey;
    Mousel = !!Mousel;
    Mouser = !!Mouser;

    calc_fps();
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
    update();
    event();

    Mousevx = Mousex - Mouselx;
    Mousevy = Mousey - Mousely;
    return isOpen();
}

int Screen::lastPressed()
{
    return LastPressed;
}
int Screen::lastPressed(unsigned int Bef)
{
    return (Bef <= MsLastPressed) * LastPressed;
}
} // namespace sdl2_lib
