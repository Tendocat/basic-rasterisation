#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "liste.h"

/*
 * la structure de donnée d'un polygone
 */
typedef struct
{
	Liste* sommets;	// la liste des sommets
	Liste* selected;// le sommet sélectionné
} Polygone;

Polygone* P_new_poly();
void P_append_sommet(Polygone* p, int x, int y);
void P_insert_sommet(Polygone* p, int x, int y);
void P_rm_selected(Polygone* poly);
bool P_is_tangent(Polygone* p, int x, int y);
void P_focus_next(Polygone* poly);
void P_focus_last(Polygone* poly);
void P_select_closest_vertex(Polygone* poly, int x, int y);
void P_select_closest_edge(Polygone* poly, int x, int y);

#endif