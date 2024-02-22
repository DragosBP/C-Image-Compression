/* BOANTA Dragos-Petru - 315CD*/

// Aici se afla functiile pentru prelucrearea matriciei de 
// pixeli si structura elementelor

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <stdio.h>
#include <stdlib.h>

// Structura pentru un pixel
typedef struct pix {
    unsigned char red, green, blue;
} Pix;

// Structura pentru un element din arbore
typedef struct item {
    unsigned char poz;
    Pix RGB;
} Item;

// Functie care returneaza un element cu toate valorile nule
Item makeEmptyElem(void)
{
    Item elem;
    elem.poz = 0;
    elem.RGB.red = 0;
    elem.RGB.green = 0;
    elem.RGB.blue = 0;
    return elem;
}

// Functie care aloca memorie pentru o matrice de pixeli
Pix **alocMemMatrice(int dim)
{
    Pix **mat = (Pix **)(malloc(dim * sizeof(Pix *)));
    for (int i = 0; i < dim; i++)
        mat[i] = (Pix *)(malloc(dim * sizeof(Pix)));
    return mat;
}

// Functie care citeste din fisier matricea de pixeli
Pix **citireMatrice(int dim, FILE *in)
{
    Pix **mat = alocMemMatrice(dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fread(&mat[i][j].red, sizeof(unsigned char), 1, in);
            fread(&mat[i][j].green, sizeof(unsigned char), 1, in);
            fread(&mat[i][j].blue, sizeof(unsigned char), 1, in);
        }
    }
    return mat;
}

// Functie care se ocupa de citirea matricei de pixeli si dimensiunii acesteia
Pix **citireFisierMatrice(int *dimension, FILE *in)
{
    Pix **mat;
    unsigned char n;
    int dim = 0, nr = 0;
    fread(&n, sizeof(unsigned char), 1, in);
    fread(&n, sizeof(unsigned char), 1, in);
    fread(&n, sizeof(unsigned char), 1, in);
    fread(&n, sizeof(unsigned char), 1, in);
    while (n != ' ') {
        dim = dim * 10 + ((int)(n) - 48);
        nr++;
        fread(&n, sizeof(unsigned char), 1, in);
    }
    for (int i = 0; i <= nr; i++)
        fread(&n, sizeof(unsigned char), 1, in);
    fread(&n, sizeof(unsigned char), 1, in);
    while (n != '\n')
        fread(&n, sizeof(unsigned char), 1, in);
    mat = citireMatrice(dim, in);
    (*dimension) = dim;
    return mat;
}

// PENTRU TESTARE
// Functie care scrie in fisier ce se afla in matricea de pixeli
void afisareMatrice(int dim, Pix **mat)
{
    FILE *out = fopen("ceva.txt", "wt");
    if (!out) {
        printf("What happened?\n");
        return;
    }
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(out, "%u ", (unsigned int)mat[i][j].red);
            fprintf(out, "%u ", (unsigned int)mat[i][j].green);
            fprintf(out, "%u ", (unsigned int)mat[i][j].blue);
            fprintf(out, "\t");
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

// Functie care scrie in matricea de pixeli
Pix **scriereMatrice(Pix **mat, int dim, int x, int y, Pix elem)
{
    for (int i = x; i < x + dim; i++)
        for (int j = y; j < y + dim; j++)
            mat[i][j] = elem;
    return mat;
}

// Functie care dezaloca memoria matricei de pixeli
void destroyMatrice(Pix **mat, int dim)
{
    for (int i = 0; i < dim; i++)
        free(mat[i]);
    free(mat);
}

#endif
