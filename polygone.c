#include "polygone.h"

/**
 * alloue un polygone vide
 */
Polygone* P_new_poly()
{
	Polygone* p = malloc(sizeof(Polygone));
	p->sommets = NULL;
	p->selected = NULL;
	return p;
}

/**
 * ajoute un sommet en tête du polygone
 */
void P_append_sommet(Polygone* p, int x, int y)
{
	p->sommets = liste_add(p->sommets, x, y);
	p->selected = p->sommets;
}

/**
 * insert un sommet avant le point selectionné
 */
void P_insert_sommet(Polygone* p, int x, int y)
{
	if (p->selected == p->sommets)
		P_append_sommet(p, x, y);
	else
	{
		Liste* s = liste_next(p->selected);
		p->selected = liste_add(p->selected, x, y);
		s->last = p->selected;
	}
}

/**
 * supprime le sommet sélectionné
 */
void P_rm_selected(Polygone* poly)
{
	if (poly->sommets == NULL)
		return;

	if (poly->selected == poly->sommets) {
		poly->sommets = liste_rm(poly->sommets);
		poly->selected = poly->sommets;
		return;
	}

	Liste* s = poly->sommets;
	while (s->last!=poly->selected) s = s->last;
	poly->selected = liste_rm(poly->selected);
	s->last = poly->selected;
	if (poly->selected == NULL || poly->selected->last == poly->selected)
		poly->selected = s;
}

/**
 * renvoie true si le point apartient au polygone et que ses deux voisins sont du même coté
 * false sinon
 * renvoie true uniquement si le point apartient au polygone et que ce poi
 **/
bool P_is_tangent(Polygone* p, int x, int y)
{
	if (p->sommets == NULL)
		return false;
	Liste* s = liste_find(p->sommets, x, y);
	if (s == NULL)
		return false;
	int y_suiv, y_last;
	y_suiv = liste_next(s)->y;
	y_last = liste_last(s)->y;
	return (y_suiv <= y && y_last <= y) || (y_suiv >= y && y_last >= y);
}

/**
 * sélectionne le sommet suivant
 */
void P_focus_next(Polygone* poly)
{
	poly->selected = liste_next(poly->selected);
}
/**
 * sélectionne le sommet précedant
 */
void P_focus_last(Polygone* poly)
{
	poly->selected = liste_last(poly->selected);
}

/**
 * sélectionne le sommet le plus procche d'un point donné
 */
void P_select_closest_vertex(Polygone* poly, int x, int y)
{
	if (poly->sommets == NULL)
		return;

	poly->selected = poly->sommets;

	// calcule la distance avec le premier sommet
	float tmp = sqrt((poly->selected->x-x) * (poly->selected->x-x) +
					 (poly->selected->y-y) * (poly->selected->y-y));
	float distance = tmp;

	// calcule la distance avec chaque sommet et met à jour s'il est plus proche
	for (Liste* i=poly->sommets->last; i!=poly->sommets; i=i->last)
	{
		if ((tmp=sqrt((i->x-x)*(i->x-x)+(i->y-y)*(i->y-y))) < distance) {
			poly->selected = i;
			distance = tmp;
		}
	}
}

/**
 * calcule l'angle GMD
 */
float _angle2vec(int xg, int yg, int xm, int ym, int xd, int yd)
{
	int xu = xg - xm;
	int yu = yg - ym;
	int xv = xd - xm;
	int yv = yd - ym;
	return (xu*xv+yu*yv) / (sqrt(xu*xu+yu*yu)*sqrt(xv*xv+yv*yv));
}

/**
 * sélectionne l'arrête le plus proche d'un point donné
 */
void P_select_closest_edge(Polygone* poly, int x, int y)
{
	if (poly->sommets == NULL)
		return;
	
	// selectionne le sommet le plus proche
	P_select_closest_vertex(poly, x, y);

	Liste* last = liste_last(poly->selected);
	Liste* next = liste_next(poly->selected);

	// calcul l'angle entre le point (x,y), le point sélectionné et le sommet suivant puis le précedent
	// l'angle le plus petit correspond à l'arrête, adjacente au sommet sélectionné, la plus proche de (x,y)
	float anglelast = _angle2vec(last->x, last->y, poly->selected->x, poly->selected->y, x, y);
	float anglenext = _angle2vec(next->x, next->y, poly->selected->x, poly->selected->y, x, y);
	if (anglelast > anglenext)
		poly->selected = last;
}