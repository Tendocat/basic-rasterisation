#include "pile.h"

/**
 * alloue une nouvelle pile
 * (une pile contenant NULL comme prochain élement est vide)
 */
Pile* pile_new()
{
    Pile* p = malloc(sizeof(struct S_pile));
    p->last = NULL;
    p->x = -1;
    p->y = -1;
    return p;
}

/**
 * désalloue une pile
 */
void pile_destruct(Pile* p)
{
    while (p->last != NULL)
        p = pile_rm(p);
    free(p);
}

/**
 * supprime l'élement en haut de la pile
 */
Pile* pile_rm(Pile* p)
{
    Pile* f = p;
    p = p->last;
    free(f);
    return p;
}

/**
 * empile une nouvelle pile
 */
Pile* pile_add(Pile* old, int x, int y)
{
    Pile* new = pile_new();
    new->x = x;
    new->y = y;
    new->last = old;
    return new;
}

/**
 * recherche un element d'une pile
 * (on considère les coordonnées x,y uniques)
 */
Pile* pile_find (Pile* p, int x, int y)
{
    while (p->last != NULL && (p->x != x || p->y != y))
        p = p->last;

    if (p->last == NULL)
        return NULL;
    else
        return p;
}

/**
 * renvoie l'element au dessus d'une pile
 */
Pile* pile_next(Pile* base, Pile* p)
{
    if (base->last == NULL)
        return base;

    if (p == base) {
        while (base->last->last != NULL) base = base->last;
        return base;
    }

    while (base->last != p) base = base->last;
    return base;
}

/**
 * renvoie l'element en dessous d'une pile
 */
Pile* pile_last(Pile* base, Pile* p)
{
    if (base->last == NULL)
        return base;

    if (p->last->last == NULL)
        return base;

    return p->last;
}