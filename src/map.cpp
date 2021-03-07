/*!
 *\file Map.cpp
 *\brief code lie a la map ( lumiere / generation ).
 *\author Franck Mourre
 *\version 1
 *\date Fevrier 2016
 */
#include "map.h"
#include "lib_util.h"
#include "lib_core.h"
#include "lib_image.h"
#include "lib_screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(A,B)((A>B)?A:B)


void copyBlock (block * Dst, block * From)
{
    /// Basic content
    Dst->damage = From->damage;
    Dst->value = From->value;
    Dst->weight = From->weight;
    Dst->solidity = From->solidity;
    Dst->diggable = From->diggable;

    /// Map lightening
    Dst->Lr = From->Lr;
    Dst->Ls = From->Ls;
    Dst->Lsr = From->Lsr;
    Dst->Lsg = From->Lsg;
    Dst->Lsb = From->Lsb;

    /// Map generator
    Dst->gBegin = From->gBegin;
    Dst->gCenter = From->gCenter;
    Dst->gCoef1 = From->gCoef1;
    Dst->gCoef2 = From->gCoef2;
    Dst->gRare = From->gRare;
    Dst->gAble = From->gAble;
}

void initBlock ( block * In )
{
    In->Im      = 0;
    In->damage  = 0;
    In->diggable= 0;
    In->gAble   = 0;
    In->gBegin  = 0;
    In->gCoef1  = 0;
    In->gCoef2  = 0;
    In->gRare   = 0;
    In->gCenter = 0;
    In->Lr      = 0;
    In->Ls      = 0;
    In->Lsr     = 0;
    In->Lsg     = 0;
    In->Lsb     = 0;
    In->solidity= 0;
    In->value   = 0;
    In->weight  = 0;
}

Map::Map()
{
    MAP = NULL;
    My = 0;
    Mx = 0;

    Dx = 20;
    Dy = 15;
    //ctor
}

void Map::setDisplay ( Screen * E )
{
    M = E;
}

Map::~Map()
{
    freeMap();

    freeTile();
    //dtor
}

void Map::freeTile()
{
    int i;
    for ( i=0 ; i<255 ; i++ )
    {
        if (tile_aloc[i])
        {
            tile_aloc[i] = 0;
            delete tile[i]->Im;
            free(tile[i]);
        }
    }
}

void Map::freeMap ()
{
    int x;
    if ( MAP != NULL )
    {
        for ( x = 0 ; x < My ; x++ )
        {
            free(MAP[x]);
        }
        free(MAP);
        MAP = NULL;
    }
}

void Map::tile_def( char * fname )
{
    Image * TMP;

    FILE * File = fopen(fname,"r+");
    if ( File == NULL )
    {
        PRINT("Error while loading tile-set data : file not found (" << fname << ")");
    }
    else
    {
        int i;
        block * New = NULL;
        char Buff[BUFSIZ];
        char Text[BUFSIZ];
        char Act = '\0';
        Text[0]='\0';
        Buff[0]='\0';


        for ( i = 0 ; i < 255 ; i ++ )
        {
            tile[i] = NULL;
            tile_aloc[i] = 0;
        }

        while ( fgets(Buff,BUFSIZ,File) != NULL )
        {
            //PRINT( Act << ":" << Buff );
            //printf("'%c' : %s\n",Act,Buff);
            if (Buff[0] != '\n')
            {
                // Changement de bloc
                if (Buff[1] == ' ' || Buff[1] == '\n' || Buff[1] == '\0')
                {
                    Act = Buff[0];
                    printf("New block : %c\n",Act);
                    New = (block*) malloc( sizeof(block) );
                    initBlock(New);
                    tile_aloc[Act] = 1;
                    tile[Act] = New;

                }
                else if ( Buff[0] == 'd' )
                {
                    if (Buff[1] == 'a') // damage
                    {
                        New->damage = atoi(Buff+6);
                    }
                    else if (Buff[1] == 'e') // default
                    {
                        for ( i = 0 ; i < 255 ; i ++ )
                        {
                            if ( tile[i] == NULL )
                            {
                                tile[i] = New;
                            }
                        }
                    }
                    else if (Buff[1] == 'i') // diggable
                    {
                        New->diggable = atoi(Buff+8);
                    }
                }
                else if ( Buff[0] == 'f' ) // Heritage interblock (from)
                {
                    copyBlock(New,tile[Buff[5]]);
                }
                else if ( Buff[0] == 'g' ) // Map Generator datas:
                {
                    New->gAble = 1;
                    if (Buff[2] == 'b') // Map generator : Begin
                    {
                        sscanf(Buff,"%*s %d",&New->gBegin);
                    }
                    else if (Buff[2] == 'c')
                    {
                        if (Buff[3] == 'o') // Map generator : Coefficient
                            sscanf(Buff,"%*s %lf %lf",&New->gCoef1,&New->gCoef2);
                        else if (Buff[3] == 'e') // Map generator : Center
                            sscanf(Buff,"%*s %d",&New->gCenter);
                    }
                    else if (Buff[2] == 'r') // Map generator : rarity
                    {
                        sscanf(Buff,"%*s %lf",&New->gRare);
                    }
                    else if (Buff[2] == 'd') // Map generator : Default
                    {
                        gen_Default = Act;
                    }
                }
                else if ( Buff[0] == 'l' ) // Light Data
                {
                    if (Buff[1] == 'r')
                    {
                        New->Lr = atoi(Buff+3);
                    }
                    else if (Buff[2] == 'e')
                    {
                        New->Ls = atoi(Buff+7);
                    }
                    else if (Buff[2] == 'r')
                    {
                        New->Lsr = atoi(Buff+3);
                    }
                    else if (Buff[2] == 'g')
                    {
                        New->Lsg = atoi(Buff+3);
                    }
                    else if (Buff[2] == 'b')
                    {
                        New->Lsb = atoi(Buff+3);
                    }
                }
                else if ( Buff[0] == 's' ) // solidity
                {
                    New->value = atoi(Buff+8);
                }
                else if ( Buff[0] == 't' ) // texture
                {
                    sscanf(Buff,"%*s %s",Text);
                    TMP = new Image(M,Text);
                    auto* Render = M->getRender();
                    
                    if ( New->Im == NULL )
                    {
                        New->Im = TMP;
                    }
                    else
                    {
                        SDL_Texture * texture = SDL_CreateTexture(M->getRender(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,TMP->texture->w, TMP->texture->h);

                        //SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_NONE);
                        SDL_SetRenderTarget(Render, texture);

                        SDL_RenderClear(Render);
                        SDL_RenderCopy(Render,New->Im->getTexture()->Orig,NULL,NULL);
                        SDL_RenderCopy(Render,TMP->getTexture()->Orig,NULL,NULL);

                        SDL_DestroyTexture(New->Im->texture->Orig);
                        New->Im->texture->Orig = texture;

                        SDL_SetRenderTarget(Render, NULL);
                        delete TMP;
                    }
                }
                else if ( Buff[0] == 'v' ) // value
                {
                    New->value = atoi(Buff+5);
                }
                else if ( Buff[0] == 'w' ) // weight
                {
                    New->value = atoi(Buff+6);
                }
                else if ( Buff[0] == '?' ) // Default block on unknow things
                {
                    til_Default = Act;
                }
            }
        }
        fclose(File);
    }
}


void Map::draw ( int X , int x , int Y , int y )
{
    int Px,Py;
    int dx,dy;

    Py = Y - Dy/2;
    dy = -(Sizey*(Dy/2+1)+y)*Z + M->Sy/2;
    /*
    if ( Py < 0 )
    {
        dy += -Sy*Py;
        Py = 0;
    }
    //*/
    //*
    for ( Py = Y - Dy/2 ; Py <= 0 ; Py ++ )
    {
        if ((dy-Sy) > M->Sy)
            break;
        for ( dx = -(Sizex*(Dx/2 + 1)+x)*Z + M->Sx/2 ; dx < M->Sx + Sx ; dx += Sx)
        {
            tile[til_Default]->Im->draw(dx,dy);
        }
        dy += Sy;
    }
    //*/

    while ( (dy-Sy) < M->Sy)
    {
        dx = -(Sizex*(Dx/2+1)+x)*Z + M->Sx/2;
        Px = X - Dx/2;

        if ( Px < 0 )
            Px+=Mx;

        while ( (dx - Sx) < M->Sx )
        {
            auto &D = MAP[Py][Px];
            if ( !D.hide )
            {
                tile[D.b]->Im->setColorFilter(D.Lr,D.Lg,D.Lb);
                tile[D.b]->Im->drawAt(dx,dy,0,(Flip) D.ang);
            }
            dx += Sx;
            Px = (Px+1) % Mx;
        }

        dy += Sy;
        Py++;
    }
}


void Map::load ( char * fname )
{
    int x;
    FILE * File = fopen(fname,"r");
    if ( File != NULL )
    {
        fscanf(File,"%d %d",&My,&Mx);
        MAP = (tile_data**) malloc( My * sizeof(tile_data*) );
        if (MAP == NULL) PRINT("malloc failed");
        for ( x=0 ; x<My ; x++ )
        {
            MAP[x] = (tile_data *) malloc( Mx * sizeof(tile_data) );
            if ( MAP[x] == NULL ) PRINT("malloc failed");
            fread(MAP[x],Mx,sizeof(tile_data),File);
        }
        fclose(File);
    }
}

void Map::save ( char * fname )
{
    int x;
    FILE * File = fopen(fname,"w");
    if (File != NULL)
    {
        fprintf(File,"%d %d\n",My, Mx);
        for ( x=0 ; x<My ; x++ )
        {
            fwrite(MAP[x],Mx,sizeof(tile_data),File);
            fprintf(File,"\n");
        }
        fclose(File);
    }
    else
    {
        PRINT("file not found");
    }
}

void Map::setTileSize( int sx , int sy )
{
    int i;
    Sizex = sx;
    Sizey = sy;
    Z = 1;
    Sx = Sizex * Z;
    Sy = Sizey * Z;
    for ( i = 0 ; i < 255 ; i ++ )
    {
        if (tile[i]->Im)
        {
            tile[i]->Im->setSize(Sx,Sy);
        }
    }
    Dx = 3 + M->Sx / Sx;
    Dy = 3 + M->Sy / Sy;
    setZoom(1);
    PRINT( Dx << " x " << Dy );
}


void Map::setZoom( float Zoom )
{
    int i;
    Z = Zoom;
    Sx = Sizex * Z;
    Sy = Sizey * Z;
    for ( i = 0 ; i < 255 ; i ++ )
    {
        if (tile[i]->Im)
        {
            tile[i]->Im->setSize(Sx,Sy);
        }
    }
    Dx = M->Sx / (Z * Sizex) +1;
    Dy = M->Sy / (Z * Sizey) +1;
    PRINT( Dx << " x " << Dy );
}

void Map::generator ( unsigned int RandKey , int mx , int my )
{
    srand(RandKey);
    int x,y,i;
    My = mx;
    Mx = my;
    double P;

    if ( MAP != NULL ) freeMap();

    MAP = (tile_data **) malloc(My * sizeof(tile_data *));
    if (MAP == NULL) PRINT("malloc failed");

    // Xi�me ligne.
    block * D;
    char Pblo[256];
    int Prob[256],Pcoeff = RAND_MAX;
    int T = 0;

    for ( i = 0 ; i < 255 ; i ++ )
    {
        Prob[i] = 0;
        Pblo[i] = 'V';
    }
    for ( x = 0 ; x < My ; x++ )
    {
        MAP[x] = (tile_data *) malloc(Mx * sizeof(tile_data));
        if (MAP[x] == NULL) PRINT("malloc failed");
        T = 1; // groar =3
        for ( i = 0 ; i < 255 ; i++ )
        {
            D = tile[i];
            if ( D->gAble && D->gBegin < x)
            {
                double Prof = (x-D->gCenter);
                P = (double) (D->gRare + (D->gCoef1 + D->gCoef2*Prof)*Prof)*Pcoeff;
                if ( P > 0 )
                {
                    //printf("%c %f:",i,P);
                    Pblo[T] = i;
                    Prob[T] = P + Prob[T-1];
                    //printf("%d\n",Prob[T]);
                    T++;
                }
            }
        }
        Prob[T] = Pcoeff;
        Pblo[T] = gen_Default;
        if ( x == 0 )
            for (y = 0 ; y < Mx ; y++ )
                MAP[x][y].b = til_Default;
        else
        {
            for (y = 0 ; y < Mx ; y++ )
            {
                int R = randint(0,Pcoeff);

                // Methode d'incr�mentation +1
                T=0;
                while (Prob[T]<R)
                    T++;

                MAP[x][y].b = Pblo[T];
                MAP[x][y].ang = random(0,3);
            }
        }

    }
    initLight();
    save("map.txt");
}

void Map::initLight ()
{
    int i,j;
    block * D;


    for ( i = 0 ; i < My ; i++ )
    {
        for ( j=0 ; j < Mx ; j++ )
        {
            MAP[i][j].Lr = 0;
            MAP[i][j].Lg = 0;
            MAP[i][j].Lb = 0;
            MAP[i][j].hide = 1;
        }
    }

    for ( i = 0 ; i < My ; i++ )
    {
        for ( j=0 ; j < Mx ; j++ )
        {
            D = tile[MAP[i][j].b];
            if ( D->Ls )
            {
                MAP[i][j].Lr = MAX(D->Lsr,MAP[i][j].Lr);
                MAP[i][j].Lg = MAX(D->Lsg,MAP[i][j].Lg);
                MAP[i][j].Lb = MAX(D->Lsb,MAP[i][j].Lb);
                calcLight(j,i);
            }
        }
    }
}
void Map::calcLight ( int x , int y )
{
    MAP[y][x].hide = (MAP[y][x].Lr < LIGHT_HIDE_LEVEL && MAP[y][x].Lg < LIGHT_HIDE_LEVEL && MAP[y][x].Lb < LIGHT_HIDE_LEVEL);
    int i;
    float q = (float)(1000-tile[MAP[y][x].b]->Lr)/1000;
    int Lr = MAP[y][x].Lr*q;
    int Lg = MAP[y][x].Lg*q;
    int Lb = MAP[y][x].Lb*q;

    int X[4],Y[4];

    Y[0] = y-1;
    Y[1] = y+1;
    Y[2] = y;
    Y[3] = y;

    X[0] = x;
    X[1] = x;
    X[2] = (x-1+Mx)%Mx;
    X[3] = (x+1+Mx)%Mx;

    for (i=0;i<4;i++)
    {
        if (Y[i] < 0)
            continue;
        auto &T = MAP[Y[i]][X[i]];
        if (T.Lr < Lr || T.Lg < Lg || T.Lb < Lb)
        {
            T.Lr = MAX(T.Lr, Lr);
            T.Lg = MAX(T.Lg, Lg);
            T.Lb = MAX(T.Lb, Lb);
            calcLight(X[i],Y[i]);
        }
    }
}
void Map::checkLight ( int x , int y )
{
    if ( y < 0 )
        return;
    if ( x > Mx )
        x -= Mx;
    block * B;
    tile_data * T;

    T = &MAP[y][x];
    B = tile[T->b];

    if ( T->Lr > B->Lsr || T->Lg > B->Lsg || T->Lb > B->Lsb )
    {
        int Lr = T->Lr;
        int Lg = T->Lg;
        int Lb = T->Lb;
        if ( B->Ls )
        {
            T->Lr = B->Lsr;
            T->Lg = B->Lsg;
            T->Lb = B->Lsb;
        }
        else
        {
            T->Lr = 0;
            T->Lg = 0;
            T->Lb = 0;
        }
        calcLight((x-1+Mx)%Mx,y);
        calcLight((x+1)%Mx,y);
        calcLight(x,y+1);
        if ( y > 0 )
            calcLight(x,y-1);

        if ( T->Lr != Lr || T->Lb!=Lb || T->Lg!=Lg)
        {
            checkLight((x-1+Mx)%Mx,y);
            checkLight((x+1)%Mx,y);
            if ( y > 1 )
                checkLight(x,y-1);
            checkLight(x,y+1);
        }
        calcLight(x,y);
    }
}

void Map::changeTile ( int x , int y , char tile )
{
    calcLight(x,y);
    checkLight(x,y);
    MAP[y][x].b = tile;
}
