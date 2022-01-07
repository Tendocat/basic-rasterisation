/**
 * Une structure de donnée pour empiler des points
 */

#ifndef _PILE_H_
#define _PILE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct S_pile {
    int x, y;
    struct S_pile* last;
} Pile;

Pile* pile_new();
void pile_destruct(Pile* p);

Pile* pile_rm(Pile* p);
Pile* pile_add(Pile* old, int x, int y);

Pile* pile_find (Pile* p, int x, int y);

Pile* pile_next(Pile* base, Pile* p);
Pile* pile_last(Pile* base, Pile* p);
#endif