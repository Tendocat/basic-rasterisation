#include "polygone.h"

/**
 * alloue un polygone vide
 */
Polygone* P_new_poly()
{
	Polygone* p = malloc(sizeof(Polygone));
	p->sommets = pile_new();
	p->selected = NULL;
	return p;
}

/**
 * ajoute un sommet en tête du polygone
 */
void P_append_sommet(Polygone* p, int x, int y)
{
	p->sommets = pile_add(p->sommets, x, y);
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
		Pile* s = pile_next(p->sommets, p->selected);
		p->selected = pile_add(p->selected, x, y);
		s->last = p->selected;
	}
}

/**
 * supprime le sommet sélectionné
 */
void P_rm_selected(Polygone* poly)
{
	if (poly->sommets->last == NULL)
		return;

	if (poly->selected == poly->sommets) {
		poly->sommets = pile_rm(poly->sommets);
		poly->selected = poly->sommets;
		return;
	}

	Pile* s = poly->sommets;
	while (s->last!=poly->selected) s = s->last;
	poly->selected = pile_rm(poly->selected);
	s->last = poly->selected;
	if (poly->selected == NULL || poly->selected->last == NULL)
		poly->selected = s;
}

/**
 * renvoie true si le sommet est tangent à une droite horizontale
 * false sinon
 **/
bool P_is_tangent(Polygone* p, int x, int y)
{
	Pile* s = pile_find(p->sommets, x, y);
	if (s == NULL)
		return false;
	int y_suiv, y_last;
	y_suiv = pile_next(p->sommets, s)->y;
	y_last = pile_last(p->sommets, s)->y;
	return (y_suiv <= y && y_last <= y) || (y_suiv >= y && y_last >= y);
}

/**
 * sélectionne le sommet suivant
 */
void P_focus_next(Polygone* poly)
{
	poly->selected = pile_next(poly->sommets, poly->selected);
}
/**
 * sélectionne le sommet précedant
 */
void P_focus_last(Polygone* poly)
{
	poly->selected = pile_last(poly->sommets, poly->selected);
}

/**
 * sélectionne le sommet le plus procche d'un point donné
 */
void P_select_closest_vertex(Polygone* poly, int x, int y)
{
	if (poly->sommets == NULL || poly->sommets->last == NULL)
		return;

	poly->selected = poly->sommets;
	float tmp = sqrt((poly->selected->x-x) * (poly->selected->x-x) +
					 (poly->selected->y-y) * (poly->selected->y-y));
	float distance = tmp;
	for (Pile* i=poly->sommets->last; i->last!=NULL; i=i->last)
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
	if (poly->sommets == NULL || poly->sommets->last == NULL)
		return;
	
	P_select_closest_vertex(poly, x, y);

	if (poly->sommets->last->last == NULL)
		return;

	Pile* last = pile_last(poly->sommets, poly->selected);
	Pile* next = pile_next(poly->sommets, poly->selected);

	float anglelast = _angle2vec(last->x, last->y, poly->selected->x, poly->selected->y, x, y);
	float anglenext = _angle2vec(next->x, next->y, poly->selected->x, poly->selected->y, x, y);
	if (anglelast > anglenext)
		poly->selected = last;
}