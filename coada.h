/* BOANTA Dragos-Petru - 315CD*/

// Aici se afla structura pentru o coada si functiile necesare
// prelucrarii acesteia
// Acestea sunt folosite pentru coada care salveaza nodurile
// arborului pentru parcugerea arborelui in latime

#ifndef COADA_H_
#define COADA_H_

#include <stdlib.h>
#include <stdio.h>
#include "arbori.h"

// Structura unui nod din coada
typedef struct nodecoada {
    Tree elem;
    struct nodecoada *next;
} *CoadaNodes;

// Structura cozii
typedef struct coada {
    CoadaNodes tail, head;
} *CoadaPix;

// Functie care aloca memorie unui nod din coada
CoadaNodes coadaNodMemAlloc(Tree elem)
{
    CoadaNodes nod = (CoadaNodes)(malloc(sizeof(struct nodecoada)));
    nod->elem = elem;
    nod->next = NULL;
    return nod;
}

// Functie care aloca memorie cozii
CoadaPix coadaMemAlloc(void)
{
    CoadaPix coada = (CoadaPix)(malloc(sizeof(struct coada)));
    coada->head = NULL;
    coada->tail = NULL;
    return coada;
}

// Functie care adauga un element in coada
CoadaPix coadaEnqueue(CoadaPix coada, Tree elem)
{
    CoadaNodes nod = coadaNodMemAlloc(elem);
    if (coada == NULL || coada->head == NULL || coada->tail == NULL) {
        coada->head = nod;
        coada->tail = nod;
        return coada;
    }
    coada->tail->next = nod;
    coada->tail = nod;
    return coada;
}

// Functie care sterge un element din coada
CoadaPix coadaDequeue(CoadaPix coada)
{
    if (coada == NULL || coada->head == NULL || coada->tail == NULL) {
        printf("HE NEEDS HELP\n");
        return coada;
    }
    CoadaNodes tmp = coada->head;
    if (coada->head == coada->tail) {
        coada->head = NULL;
		coada->tail = NULL;
        free(tmp);
        return coada;
    }
    coada->head = coada->head->next;
    free(tmp);
    return coada;
}

// Functie care verifica daca coada este goala
int isCoadaEmpty(CoadaPix coada) {
    if (coada == NULL || coada->head == NULL || coada->tail == NULL)
        return 1;
    return 0;
}

// Functie care dezaloca memoria cozii
void coadaDestroy(CoadaPix coada)
{
    CoadaNodes tmp;
	while (coada->head) {
		tmp = coada->head;
		coada->head = coada->head->next;
		free(tmp);
	}
	free(coada);
}

#endif
