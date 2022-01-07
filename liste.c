#include "liste.h"

/**
 * alloue une nouvelle liste
 */
Liste* liste_new()
{
    Liste* p = malloc(sizeof(struct S_liste));
    p->last = p;
    p->next = p;
    p->x = -1;
    p->y = -1;
    return p;
}

/**
 * désalloue entièrement une liste
 */
void liste_destruct(Liste* p)
{
    if (p == NULL)
        return;
    while (p->last != p)
        p = liste_rm(p);
    free(p);
}

/**
 * supprime le dernier élement de la liste
 */
Liste* liste_rm(Liste* p)
{
    if (p == NULL)
        return NULL;
    if (p->last != p) {
        p->last->next = p->next;
        p->next->last = p->last;
        Liste* last = p->last;
        free(p);
        return last;
    } else {
        free (p);
        return NULL;
    }
}

/**
 * insert une nouvelle liste
 */
Liste* liste_add(Liste* old, int x, int y)
{
    Liste* new = liste_new();
    new->x = x;
    new->y = y;
    if (old != NULL) {
        new->last = old;
        new->next = old->next;
        old->next->last = new;
        old->next = new;
    }
    return new;
}

/**
 * recherche un element d'une liste
 * (on considère les coordonnées x,y uniques)
 */
Liste* liste_find (Liste* p, int x, int y)
{
    if (p == NULL)
        return NULL;
    
    Liste* debut = p;
    while (p != debut && (p->x != x || p->y != y))
        p = p->last;

    if (p->x != x || p->y != y)
        return NULL;
    else
        return p;
}

/**
 * renvoie l'element suivant d'une liste
 */
Liste* liste_next(Liste* p)
{  
    if (p == NULL)
        return NULL;

    return p->next;
}

/**
 * renvoie l'element précedent d'une liste
 */
Liste* liste_last(Liste* p)
{
    if (p == NULL)
        return NULL;

    return p->last;
}