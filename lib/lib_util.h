/**
 * @file lib_util.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Some utility/basics function 
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
#include <cstdint>
#include <iostream>

namespace sdl2_lib
{

#define UNDEFINED 0
#define undefined 0

#define DEBUG 0
#define PRINT_LVL(A, B) ((B > DEBUG) ? std::cout : (std::cout << "File : " << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")" << "\n" \
                                                              << A << std::endl))
#if DEBUG > 0
#define PRINT(A) PRINT_LVL(A, 1)
#else
#define PRINT(A) PRINT_LVL(A, 0)
#endif

/// Should be set to 1 when compiling and creating then to 0 before uploading ( with crypt files )
#define OWNER 1

/// This should always be 1
#define CRYPT 1

/// Internal Data
#define PI 3.14159265359
#define MAXSUM 137863

/// Fonction generant un entier aleatoire entre Min et Max [Min;Max[
int randint(int Min, int Max);

int Strlen(char *I);

uint64_t converti(char *input);
/** Converti permet la conversion d'une chaine de caractere en entier, la fonction n'est pas protegee contre les overflows
  * @param input : la chaine de caractere a convertir en entier.
  * @return : retourne le resultat de la conversion
  */

int random(int Min, int Max);
/** Permet de tirer un nombre aleatoire compris entre min et max
  * Il faut utiliser srand(time(0)); au lancement du programme.
  */

int hachage(const char *str, int Max);

int crypt(char *cryptit, int force);
/// Return -1 if the file isn't safe, 1 if it is.

double DegToRad(int Deg);

int ang_2d(int x1, int y1, int x2, int y2);
int dist_2d(int x1, int y1, int x2, int y2);

unsigned int FileSize(char *file);
int fileExist(char *filename);
char *FileExtension(char *path);
char *FileName(char *path);

int isIn(char *Data, int DataLen, char *Look, int LookLen);
} // namespace sdl2_lib

#endif // UTIL_H_INCLUDED
