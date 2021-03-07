/*!
 *\file Map.h
 *\brief code lie a la map ( lumiere / generation ).
 *\author Franck Mourre
 *\version 1
 *\date Fevrier 2016
 */

#ifndef MAP_H
#define MAP_H

#include "lib_core.h"
#include "lib_image.h"

#define TSIZE 64
using namespace sdl2_lib;

/*!
 *\struct block
 *\brief structure interne de la classe Map
 * Cette structure contient toute les informations lues depuis un fichier de configuration.
 */
typedef struct
{
    int value,damage,weight,solidity,diggable;
    int Lr,Ls,Lsr,Lsg,Lsb;

    Image * Im;

    int gBegin,gCenter,gAble;
    double gRare,gCoef1,gCoef2;
} block;
void copyBlock (block * Dst, block * From);

#define LIGHT_HIDE_LEVEL 8
/*!
 *\struct tile_data
 *\brief structure de stockage de la map
 * Structure qui contient toute les informations liee a la map.
 */
typedef struct
{
    unsigned char hide,b,ang,Lr,Lg,Lb;
} tile_data;


typedef struct
{
    int Life_len;
    int From_alpha,To_alpha;
    int From_size,To_size;

    int Mov_x,Move_y;
    int Mov_rand_x_min, Mov_rand_x_max;
    int Mov_rand_y_min, Mov_rand_y_max;
    int Mov_noise;

    Image Im;
} particle;
typedef struct
{
    unsigned int Spawn;
    int move_x,move_y;

    particle * Type;
} particle_data;

class Map
{
public:
    /** Default constructor */
    Map();
    /** Default destructor */
    virtual ~Map();
    void setDisplay ( Screen * E );

    void tile_def   ( char * filename );
    void generator  ( unsigned int Key, int Mx , int My );
    void setTileSize( int Sx , int Sy );
    void setZoom    ( float Zoom );
    void draw       ( int Col , int c , int Lin , int l );
    void load       ( char * filename );
    void save       ( char * filename );

    void checkLight ( int x , int y );
    void changeTile ( int x , int y , char tile );


    void initLight ();
    void calcLight ( int x , int y );

    int Sizex;
    int Sizey;
    int Sx,Sy;
    int Mx,My;
    float Z;
    tile_data ** MAP;
    void freeTile   ();
protected:
private:
    void freeMap    ();

    Screen * M;
    block * tile[256]; // On n'alloueras que les elements si besoin, ceux non alloues pointerons sur le block default.
    int tile_aloc[256];
    char til_Default;
    char gen_Default;
    int Dx,Dy;
};

#endif // MAP_H
