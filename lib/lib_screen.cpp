#include <iostream>

#include "../lib/lib_screen.h"
#include "../lib/lib_util.h"

void lib_Screen::init( int X, int Y )
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
    TARGET_FPS  = 0;
    TARGET_TIME = 0;
    TARGET_MULT = 0;
    TARGET_LOGIC= 0;
    TARGET_LOGICFPS=0;
    FPS = 0;
    Frame = 0;
    DropsCount = 0;

    Sx = X;
    Sy = Y;

    for (int i = 0 ; i < SDLK_COUNT ; i ++ )
        Clavier[i] = NotPressed;
}

void lib_Screen::setLogicalFPS( int A )
{
    TARGET_LOGICFPS = A;
    TARGET_LOGIC = ((double)1000/A);
}
void lib_Screen::setFPS( int A )
{
    TARGET_FPS = A;
    TARGET_TIME = (1000/A);
}
void lib_Screen::FPS_DELAY()
{
    int Remain = (TARGET_LAST + TARGET_TIME)-SDL_GetTicks();

    while ( Remain > 2 )
    {
        SDL_Delay(1);
        Remain = (TARGET_LAST + TARGET_TIME)-SDL_GetTicks();
    }

    while ( Remain > 0 )
    {
        Remain = (TARGET_LAST + TARGET_TIME)-SDL_GetTicks();
    }
}
void lib_Screen::update()
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
        TARGET_MULT = (double) (SDL_GetTicks()-TARGET_LAST)/TARGET_LOGIC;
    }
    TARGET_LAST = SDL_GetTicks();
}
double lib_Screen::getLogicalMult ()
{
    return TARGET_MULT;
}

void lib_Screen::create(string title, int x, int y)
{
    create(title,x,y,0,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    init(x,y);
}

void lib_Screen::create(string title, int x, int y, Uint32 Winflag, Uint32 Renflag)
{
    Window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x, y, Winflag);
    Render = SDL_CreateRenderer( Window, -1, Renflag);
    init(x,y);
}
lib_Screen::lib_Screen ()
{
    Window = 0;
    Render = 0;
}
lib_Screen::~lib_Screen ()
{
    destroy();
}

void lib_Screen::recreate( Uint32 Renflag )
{
    SDL_DestroyRenderer(Render);
    Render = SDL_CreateRenderer( Window, -1, Renflag);
}

void lib_Screen::destroy()
{
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Render);
}

void lib_Screen::name( char * name )
{
    SDL_SetWindowTitle(Window,name);
}
void lib_Screen::name( string name )
{
    SDL_SetWindowTitle(Window,name.c_str());
}

int lib_Screen::key( int Key )
{
    int Id = (Key<128)?Key:Key-CONVERT;
    int lcl = Clavier[Id]; 
    if ( Clavier[ Id ] & OnPress )
    {
        Clavier[Id] = Pressed;
    }
    return lcl;
}
void lib_Screen::event()
{
    unsigned int Time = SDL_GetTicks();
    char * Fpath = 0;
    int key;
    while ( SDL_PollEvent(&E) )
    {
        switch(E.type)
        {
        case SDL_DROPFILE:  // Drag & Drop
            Fpath = E.drop.file;
            if ( DropsCount < MAX_DROP )
                strcpy(DropsData[DropsCount++],Fpath);
            delete Fpath;
            break;
        case SDL_KEYUP:
            key = (E.key.keysym.sym<128)?E.key.keysym.sym:E.key.keysym.sym-CONVERT;
            Clavier[key] = NotPressed;
            break;
        case SDL_KEYDOWN:
            key = (E.key.keysym.sym<128)?E.key.keysym.sym:E.key.keysym.sym-CONVERT;
            if ( Clavier[key] == NotPressed )
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
                Mousel  = 0;
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
                if ( !Mousel )
                    Mousel  = OnPress;
                else
                    Mousel = Pressed;
                break;
            case SDL_BUTTON_RIGHT:
                if ( !Mouser )
                    Mouser  = OnPress;
                else
                    Mouser = Pressed;
                break;
            case SDL_BUTTON_MIDDLE:
                if ( !Mousem )
                    Mousem  = OnPress;
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
            default :
                break;
            }
            break;
        default:
            break;
        }
    }
}

int lib_Screen::HasWindowEventHappened()
{
    if (Wdevent)
    {
        Wdevent = 0;
        return 1;
    }
    return 0;
}

int lib_Screen::isOpen()
{
    return (Open==1);
}

void lib_Screen::calc_fps ()
{
    Frame++;
    if (Frame < FPS_CALC_REFRESH)
        return;
    unsigned int now = SDL_GetTicks();
    if (now == Before) {
        SDL_Delay(1);
    }
    now = SDL_GetTicks();
    FPS = (1000*Frame)/(now-Before);
    Before = now;
    Frame = 0;
}

int lib_Screen::getFps()
{
    return FPS;
}

int lib_Screen::Do()
{
    int Mouselx=Mousex,Mousely=Mousey;
    if ( Mousel )
        Mousel = Pressed;
    if ( Mouser )
        Mouser = Pressed;
    calc_fps();
    SDL_SetRenderDrawColor(Render,0,0,0,255);
    update();
    event();
    Mousevx = Mousex - Mouselx;
    Mousevy = Mousey - Mousely;
    return isOpen();
}

int lib_Screen::lastPressed()
{
    return LastPressed;
}
int lib_Screen::lastPressed( unsigned int Bef )
{
    return (Bef<=MsLastPressed)*LastPressed;
}