/*!
 *\file util.h
 *\brief different outils utilitaire generaliste
 *\author Franck Mourre
 *\version 1
 *\date Octobre 2016
 */

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <math.h>
#include <string.h>

#define OWNER 1
/// Should be set to 1 when compiling and creating then to 0 before uploading ( with crypt files )
#define CRYPT 1
/// This should always be 1

/// DATAMAX is the number of data to stock for sin & cos, it also change the frequence
#define DATAMAX 360

/// Internal Data
#define PI 3.1416
#define MAXSUM 137863


///Define qui permet d'afficher des messages d'erreur utile pour les dev :D.
#define ERROR(X) cout << "Error \"%s\" occured in function %s at line %d file %s !\n",X,__FUNCTION__,__LINE__,__FILE__)

/// Fonction generant un entier aleatoire entre Min et Max [Min;Max[
int randint ( int Min, int Max );

int Strlen ( char * I );

unsigned long int converti(char* input);
/** Converti permet la conversion d'une chaine de caractere en entier, la fonction n'est pas protegee contre les overflows
  * @param input : la chaine de caractere a convertir en entier.
  * @return : retourne le resultat de la conversion
  */

int random (int Min, int Max);
/** Permet de tirer un nombre aleatoire compris entre min et max
  * Il faut utiliser srand(time(0)); au lancement du programme.
  */

int hachage ( const char* str,int Max);

int crypt (char *cryptit, int force);
/// Return -1 if the file isn't safe, 1 if it is.


double DegToRad (int Deg);

int sin100 (int Deg);
int cos100 (int Deg);
int ang_2d ( int x1, int y1, int x2, int y2 );
int dist_2d ( int x1, int y1, int x2, int y2 );

unsigned int FileSize ( char * file );
int fileExist ( char * filename );
char * FileExtension ( char * path );
char * FileName ( char * path );

int isIn ( char * Data, int DataLen, char * Look, int LookLen);

/// Version 'Optimise' de la fonction qui ne recalcul pas tout.
double DegToRadOpt (int Deg);

/// Fonction optimisee des cosinus et sinus ( mais necessitant plus de ram )
void InitData ();
void deleteData ();
int D_sin ( int Ang );
int D_cos ( int Ang );


#endif // UTIL_H_INCLUDED
