/**
 * Une structure de donnée pour stocker des points
 */

#ifndef _LISTE_H_
#define _LISTE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * la structure pour les points
 * j'ai initialement choisi une liste simplement chainée car la fonction scanline a besoin d'une sorte de pile
 * de plus le programme étant interactif, il est peu probable qu'un utilisateur entre des centaines de milliers de points,
 * donc la performance n'est pas un critère pertinent
 * 
 * MAIS j'ai implémenté une liste doublement chainée histoire d'être plus clair ;)
 */
typedef struct S_liste {
    int x, y;
    struct S_liste* last;
    struct S_liste* next;
} Liste;

// alloue un nouveau point
Liste* liste_new();
// désalloue un point
void liste_destruct(Liste* p);

// supprime l'élement en haut de la liste
Liste* liste_rm(Liste* p);
// 
Liste* liste_add(Liste* old, int x, int y);
// recherche un élément de la liste en (x,y)
Liste* liste_find (Liste* p, int x, int y);

// retourne l'element suivant de la liste
Liste* liste_next(Liste* p);
// retourne l'element precedent de la liste
Liste* liste_last(Liste* p);
#endif
