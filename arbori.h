/* BOANTA Dragos-Petru - 315CD*/

// Aici se afla structurile si functiile pentru lucrarile
// cu arbori cuatrenari completi
#ifndef ARBORI_H_
#define ARBORI_H_

#include <stdio.h>
#include <stdlib.h>
#include "element.h"

// Structura pentru un nod din arbore
typedef struct tree {
    int x, y, dim;
    Item elem;
    struct tree *top_left, *top_right, *bottom_right, *bottom_left;
} *Tree;

// Functie care creeaza si aloca memorie pentru un nod din arbore
Tree createTree(Item elem, int x, int y, int dim)
{
    Tree root = (Tree)(malloc(sizeof(struct tree)));
    root->top_left = NULL;
    root->top_right = NULL;
    root->bottom_right = NULL;
    root->bottom_left = NULL;
    root->elem = elem;
    root->x = x;
    root->y = y;
    root->dim = dim;
    return root;
}

// Functia care aloca memorie pentru un arbore si atribuie primul noi in acesta
Tree initTree(int dim)
{
    Tree root = createTree(makeEmptyElem(), 0, 0, dim);
    return root;
}

// Functie care calculeaa valoare maxima dintre 4 numere intregi
int maxValue(int a, int b, int c, int d)
{
    int max = a;
    if (b > max)
        max = b;
    if (c > max)
        max = c;
    if (d > max)
        max = d;
    return max;
}

// Functie care calculeaza valorile cerute la cerinta 1
int height(Tree root, int *dim, int *nr)
{
    if (root->elem.poz == 1) {
        if (root->dim > *dim)
            *dim = root->dim;
        *nr += 1;
        return 0;
    }
    int TL, TR, BR, BL;
    TL = height(root->top_left, dim, nr);
    TR = height(root->top_right, dim, nr);
    BR = height(root->bottom_right, dim, nr);
    BL = height(root->bottom_left, dim, nr);
    return 1 + maxValue(TL, TR, BR, BL);
}

// Functie care dezaloca memoria alocata arborelui
void destroyTree(Tree *root)
{
	if ((*root) != NULL) {
		destroyTree(&(*root)->top_left);
		destroyTree(&(*root)->top_right);
        destroyTree(&(*root)->bottom_right);
        destroyTree(&(*root)->bottom_left);
		free(*root);
		(*root) = NULL;
	}
}


#endif
