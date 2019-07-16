#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "util.h"

namespace sdl2_lib
{
int *Dsin = 0, *Dcos = 0;

int randint(int Min, int Max)
{
    return (rand() % (Max - Min)) + Min;
}

int Strlen(char *I)
{
    char *O = I;
    while (*(I++) != '\0')
        ;
    return I - O;
}

uint64_t converti(char *input)
{
    int i = 0, out = 0;
    char c;
    while (input[i] != '\0' && input[i] != '\n')
    {
        c = input[i];
        i++;
        if (c >= '0' && c <= '9')
        {
            out = out * 10 + c - '0';
        }
        else
        {
            return 0;
        }
    }
    return out;
}

int random(int Min, int Max)
{
    if (Min == Max)
        return Min;
    int Out = (rand() % (Max - Min + 1)) + Min;
    return Out;
}

int hachage(const char *str, int Max)
{
    unsigned int i, out = 0;
    for (i = 0; str[i] != '\0'; i++)
        out = (out * 256 + str[i]) % Max;
    return (out);
}

int crypt(char *cryptit, int force)
{
#if CRYPT == 1
    FILE *lecture = 0;
    FILE *fichier = 0;
    lecture = fopen(cryptit, "r");

    if (lecture == 0)
    {
        PRINT_LVL("WARNING : looks like a file is missing !",1);
        return -1;
    }

    char Read[1024];
    uint64_t Hacher = 0;
    while (fgets(Read, 1024, lecture) != NULL)
    {
        Hacher += hachage(Read, MAXSUM);
    }
    Hacher = Hacher % MAXSUM;
    fclose(lecture);

    if (OWNER == 1 || force == 1)
    {
        sprintf(Read, "%s_crypt", cryptit);
        fichier = fopen(Read, "w");
        fprintf(fichier, "%llud", (long long unsigned int) Hacher);
        fclose(fichier);
    }
    else // OWNER
    {
        sprintf(Read, "%s_crypt", cryptit);
        fichier = fopen(Read, "r");
        if (fichier == 0)
        {
            PRINT_LVL("ERROR : checksum file is missing !",0);
            return -1;
        }
        else
        {
            if (!fgets(Read, 1024, fichier))
                PRINT_LVL("ERROR : checksum file is empty !",0);
            uint64_t lcl = converti(Read);
            fclose(fichier);
            if (lcl != Hacher)
            {
                PRINT_LVL("ERROR : checksum file doesn't match !",0);
                return -1;
            }
        }
    }
#endif // CRYPT
    return 1;
}

double DegToRad(int Deg)
{
    if (Deg > 0)
        return (double)Deg * PI / 180;
    else if (Deg < 0)
        return (double)(Deg + 360) * PI / 180;
    return 0;
}

double DegToRadOpt(int Deg)
{
    static int t = 0;
    static double Out[1080];
    if (t == 0)
    {
        for (t = 0; t < 1080; t++)
            Out[t] = (double)(t * PI / 180);
        t = 1;
    }
    return Out[Deg];
}

char *FileName(char *path)
{
    unsigned int i, c = 0;
    char lcl[512];
    for (i = 0; i < strlen(path); i++)
    {
        if (path[i] == '\\')
        {
            c = 0;
        }
        else
        {
            lcl[c] = path[i];
            c++;
        }
    }
    lcl[c] = '\0';
    char *Out = new char[c + 1];
    strcpy(Out, lcl);
    return Out;
}
char *FileExtension(char *path)
{
    unsigned int i, c = 0;
    char lcl[512];
    for (i = 0; i < strlen(path); i++)
    {
        if (path[i] == '.')
        {
            c = 0;
        }
        else
        {
            lcl[c] = path[i];
            c++;
        }
    }
    lcl[c] = '\0';
    char *Out = new char[c + 1];
    strcpy(Out, lcl);
    return Out;
}
int fileExist(char *filename)
{
    FILE *fichier = 0;
    fichier = fopen(filename, "r");
    if (fichier != 0)
    {
        fclose(fichier);
        return 1;
    }
    return 0;
}

int ang_2d(int x1, int y1, int x2, int y2)
{
    return ((double)180 * atan2(x1 - x2, y1 - y2)) / (PI);
}

int dist_2d(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int isIn(char *Data, int DataLen, char *Look, int LookLen)
{
    int i, i2, max, good = 0;
    max = DataLen - LookLen + 1;
    for (i = 0; i < max; i++)
    {
        good = 0;
        for (i2 = 0; i2 < LookLen; i2++)
        {
            if (Data[i + i2] == Look[i2])
            {
                good = 1;
            }
            else
            {
                good = 0;
                i2 = LookLen;
            }
        }
        if (good)
        {
            return i + LookLen;
        }
    }
    return 0;
}

unsigned int FileSize(char *file)
{
    int size = 0;
    FILE *fichier;
    fichier = fopen(file, "rb");
    if (fichier != 0)
    {
        fseek(fichier, (long)0, SEEK_END);
        size = ftell(fichier);
        fclose(fichier);
    }
    else
    {
        printf("failed to get size\n");
    }
    return size;
}
} // namespace sdl2_lib
