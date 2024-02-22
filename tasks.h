/* BOANTA Dragos-Petru - 315CD*/

// Aici se afla functiile care fac comprimarile si decomprimarile fisierelor si
// le scriu pe acestea in fisiere
int nr = 0;

#ifndef TASKS_H_
#define TASKS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arbori.h"
#include "element.h"
#include "coada.h"

// Functie care verifica daca submatricea mai trebuie divizata sau
Item divideCheck(Pix **mat, unsigned long long prag, unsigned long long dim, int x, int y)
{
    unsigned long long sumRed = 0, sumGreen = 0, sumBlue = 0, total = 0;
    unsigned long long mean, red, green, blue;
    unsigned long long i, j;
    Item elem;
    for (i = x; i < x + dim; i++)
        for (j = y; j < y + dim; j++) {
            sumRed += (unsigned long long)(mat[i][j].red);
            sumGreen += (unsigned long long)(mat[i][j].green);
            sumBlue += (unsigned long long)(mat[i][j].blue);
        }
    red = sumRed / (dim * dim);
    green = sumGreen / (dim * dim);
    blue = sumBlue / (dim * dim);
    elem.RGB.red = (unsigned char)red;
    elem.RGB.green = (unsigned char)green;
    elem.RGB.blue = (unsigned char)blue;
    for (i = x; i < x + dim; i++)
        for (j = y; j < y + dim; j++) {
            total +=
            (red - (unsigned long long)(mat[i][j].red)) * (red - (unsigned long long)(mat[i][j].red)) +
            (green - (unsigned long long)(mat[i][j].green)) * (green - (unsigned long long)(mat[i][j].green)) +
            (blue - (unsigned long long)(mat[i][j].blue)) * (blue - (unsigned long long)(mat[i][j].blue));
        }
    mean = total / (3 * dim * dim);
    if (mean <= prag)
        elem.poz = 1;
    else
        elem.poz = 0;
    return elem;
}

// Functie care creeaza arborele cuatrenar de comprimare a imaginii
Tree compresion(Tree root, Pix **mat, int prag, int *nrNod)
{
    Item elem = divideCheck(mat, prag, root->dim, root->x, root->y);
    if (elem.poz == 0 && root->dim > 1) {
        // Crearea fiilor radacinii
        *nrNod += 4;
        root->top_left = createTree(makeEmptyElem(), root->x, root->y, root->dim / 2);
        root->top_right = createTree(makeEmptyElem(), root->x, (root->y + root->dim / 2), root->dim / 2);
        root->bottom_right = createTree(makeEmptyElem(), (root->x + root->dim / 2), (root->y + root->dim / 2), root->dim / 2);
        root->bottom_left = createTree(makeEmptyElem(), (root->x + root->dim / 2), root->y, root->dim / 2);
        // Apelare pentru fiecare din nou separarea lor
        root->top_left = compresion(root->top_left, mat, prag, nrNod);
        root->top_right = compresion(root->top_right, mat, prag, nrNod);
        root->bottom_right = compresion(root->bottom_right, mat, prag, nrNod);
        root->bottom_left = compresion(root->bottom_left, mat, prag, nrNod);
    } else {
        root->elem = elem;
    }
    return root;
}

// FUnctie care scrie in fisier arborele conform cerintei prin parcugere in latime
void writeCompressed(FILE *out, CoadaPix coada)
{
    while (!isCoadaEmpty(coada))
    {
        if (coada->head->elem != NULL) {
            if (coada->head->elem->elem.poz == 0)
                fwrite(&coada->head->elem->elem.poz, sizeof(unsigned char), 1, out);
            else {
              fwrite(&coada->head->elem->elem.poz, sizeof(unsigned char), 1, out);
              fwrite(&coada->head->elem->elem.RGB.red, sizeof(unsigned char), 1, out);
              fwrite(&coada->head->elem->elem.RGB.green, sizeof(unsigned char), 1, out);
              fwrite(&coada->head->elem->elem.RGB.blue, sizeof(unsigned char), 1, out);
            } 
            coada = coadaEnqueue(coada, coada->head->elem->top_left);
            coada = coadaEnqueue(coada, coada->head->elem->top_right);
            coada = coadaEnqueue(coada, coada->head->elem->bottom_right);
            coada = coadaEnqueue(coada, coada->head->elem->bottom_left);
        }
        coada = coadaDequeue(coada);
    }
}

// Functie care creeaza arborele cuatrenar pentru decomprimarea imaginei
Tree decompresion(CoadaPix coada, FILE *in)
{
    Tree root = coada->head->elem;
    while (!isCoadaEmpty(coada))
    {
        fread(&coada->head->elem->elem.poz, sizeof(char), 1, in);
        if (coada->head->elem->elem.poz == 0) {
            coada->head->elem->top_left = createTree(makeEmptyElem(), coada->head->elem->x, coada->head->elem->y, coada->head->elem->dim / 2);
            coada->head->elem->top_right = createTree(makeEmptyElem(), coada->head->elem->x, (coada->head->elem->y + coada->head->elem->dim / 2), coada->head->elem->dim / 2);
            coada->head->elem->bottom_right = createTree(makeEmptyElem(), (coada->head->elem->x + coada->head->elem->dim / 2), (coada->head->elem->y + coada->head->elem->dim / 2), coada->head->elem->dim / 2);
            coada->head->elem->bottom_left = createTree(makeEmptyElem(), (coada->head->elem->x + coada->head->elem->dim / 2), coada->head->elem->y, coada->head->elem->dim / 2);
            coada = coadaEnqueue(coada, coada->head->elem->top_left);
            coada = coadaEnqueue(coada, coada->head->elem->top_right);
            coada = coadaEnqueue(coada, coada->head->elem->bottom_right);
            coada = coadaEnqueue(coada, coada->head->elem->bottom_left);
        } else {
            fread(&coada->head->elem->elem.RGB.red, sizeof(char), 1, in);
            fread(&coada->head->elem->elem.RGB.green, sizeof(char), 1, in);
            fread(&coada->head->elem->elem.RGB.blue, sizeof(char), 1, in);
        }
        coada = coadaDequeue(coada);
    }
    return root;
}

// Functie care scrie matricea de pixeli
Pix **createDecompresionMat(Tree root, Pix **mat)
{
    if (root->elem.poz == 0) {
        mat = createDecompresionMat(root->top_left, mat);
        mat = createDecompresionMat(root->top_right, mat);
        mat = createDecompresionMat(root->bottom_left, mat);
        mat = createDecompresionMat(root->bottom_right, mat);
    } else {
        mat = scriereMatrice(mat, root->dim, root->x, root->y, root->elem.RGB);
    }
    return mat;
}

// Functie care scrie fisierul de tip PPM
void writeDecompressed(Pix **mat, int dim, FILE *out)
{
    char init[50] = "P6\n", number[25];
    sprintf(number, "%d", dim);
    strcat(init, number);
    strcat(init, " ");
    strcat(init, number);
    strcat(init, "\n255\n");
    fwrite(init, sizeof(char), strlen(init), out);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fwrite(&mat[i][j].red, sizeof(char), 1, out);
            fwrite(&mat[i][j].green, sizeof(char), 1, out);
            fwrite(&mat[i][j].blue, sizeof(char), 1, out);
        }
    }
}

#endif
