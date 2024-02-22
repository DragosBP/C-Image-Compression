/* BOANTA Dragos-Petru - 315CD*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arbori.h"
#include "element.h"
#include "tasks.h"

// Structura pentru a retine toate detaliile care se primesc
// ca argument la executarea programului
typedef struct args {
    int c1, c2, d;
    int factor1, factor2;
    FILE *in, *out;
} Args;

// Functie care se ocupa de salvarea valorilor primite ca argument
Args citireArgs(char const *argv[])
{
    Args task;
    task.c1 = 0;
    task.c2 = 0;
    task.factor1 = 0;
    task.factor2 = 0;
    task.d = 0;
    int i;
    for (i = 1; i <= 5; i += 2) {
        if (strchr(argv[i], '-')) {
            if (strchr(argv[i], 'd')) {
                task.d = 1;
                i--;
            }
            if (strchr(argv[i], '1')) {
                task.c1 = 1;
                task.factor1 = atoi(argv[i + 1]);
            }
            if (strchr(argv[i], '2')) {
                task.c2 = 1;
                task.factor2 = atoi(argv[i + 1]);
            }
        } else {
            task.in = fopen(argv[i], "rb");
            task.out = fopen(argv[i + 1], "wb");
            return task;
        }
    }
    task.in = fopen(argv[i], "rb");
    if (task.c1 == 0)
        task.out = fopen(argv[i + 1], "wb");
    else
        task.out = fopen(argv[i + 1], "wt");
    return task;
}

int main(int argc, char const *argv[])
{
    Args task = citireArgs(argv);
    int dim, nrNod = 1;
    Pix **mat;
    if (task.c1 == 1) {
        mat = citireFisierMatrice(&dim, task.in);
        Tree root = initTree(dim);
        root = compresion(root, mat, task.factor1, &nrNod);
        destroyMatrice(mat, dim);
        int maxDim = 0, nrFull = 0, h;
        h = height(root, &maxDim, &nrFull) + 1;
        fprintf(task.out, "%d\n%d\n%d\n", h, nrFull, maxDim);
        destroyTree(&root);
    }

    if (task.c2 == 1) {
        mat = citireFisierMatrice(&dim, task.in);
        Tree root = initTree(dim);
        root = compresion(root, mat, task.factor2, &nrNod);
        destroyMatrice(mat, dim);
        CoadaPix coada = coadaMemAlloc();
        coada = coadaEnqueue(coada, root);
        fwrite(&dim, sizeof(unsigned int), 1, task.out);
        writeCompressed(task.out,  coada);
        coadaDestroy(coada);
        destroyTree(&root);
    }

    if (task.d == 1) {
        fread(&dim, sizeof(unsigned int), 1, task.in); // Citire dimensiune
        mat = alocMemMatrice(dim);
        Tree root = initTree(dim);
        CoadaPix coada = coadaMemAlloc();
        coada = coadaEnqueue(coada, root);
        root = decompresion(coada, task.in);
        mat = createDecompresionMat(root, mat);
        writeDecompressed(mat, dim, task.out);
        destroyMatrice(mat, dim);
        coadaDestroy(coada);
        destroyTree(&root);
    }
    // Inchidere fisiere
    fclose(task.in);
    fclose(task.out);
    return 0;
}

