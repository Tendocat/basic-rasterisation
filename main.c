
/*===============================================================*\

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/
#include <stdbool.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "Image.h"
#include "polygone.h"

enum Mode {append, vertex, edge};
Color C_blanc = {255,255,255};
Color C_rouge = {255,0,0};

int mode = append;
bool ouvert = true, fill = false;
Polygone* poly;
Image *img;

void draw_poly(Polygone* p)
{
	for (Pile* pile=p->sommets; pile->last!=NULL && pile->last->last!=NULL; pile=pile->last) {
    	I_bresenham(img, pile->last->x, pile->last->y, pile->x, pile->y, C_blanc);
	}
	if (!ouvert) {
		Pile* fermeture = pile_next(p->sommets, p->sommets);
		I_bresenham(img, p->sommets->x, p->sommets->y, fermeture->x, fermeture->y, C_blanc);
		if (fill)
			I_fill_scan_line(img, poly, C_blanc);
	}

	if (mode == edge)
		I_bresenham(img, poly->selected->x, poly->selected->y,
					pile_next(poly->sommets, poly->selected)->x,
					pile_next(poly->sommets, poly->selected)->y,
					C_rouge);

	if (p->selected == NULL || p->selected->last == NULL || mode == edge)
		return;
	
	for (int i=p->selected->x-5; i<p->selected->x+6;i++)
		if (i > 0 && i < img->_width)
			I_plotColor(img, i, p->selected->y, C_new(255, 0, 0));
	for (int i=p->selected->y-5; i<p->selected->y+6;i++)
		if (i > 0 && i < img->_height)
			I_plotColor(img, p->selected->x, i, C_new(255, 0, 0));
}

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

	I_fill(img, C_new(0,0,0));
	draw_poly(poly);

	I_draw(img);

    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN)) {
		I_focusPoint(img,x,img->_height-y);
		if(ouvert && mode == append)
			P_append_sommet(poly, x, y);
		else if (mode == vertex)
			P_select_closest_vertex(poly, x, y);
		else if (mode == edge)
			P_select_closest_edge(poly, x, y);
	}
	if((button==GLUT_RIGHT_BUTTON)&&(state==GLUT_DOWN) && mode == edge) {
		P_insert_sommet(poly, x, y);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch(key)
	{
	case 27 : exit(1); break;
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
	case 'a' : I_zoomInit(img); break;
	case 'c' : ouvert = !ouvert;break;
	case 'f' : fill = !fill;	break;
	case 'i' : mode = append;	break;
	case 'v' : mode = vertex;	break;
	case 'e' : mode = edge;		break;
	case 127 :
		if (mode == vertex)
			P_rm_selected(poly);
	break;
	default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch (mode)
	{
	case append:
		switch(key)
		{
		case GLUT_KEY_UP    : I_move(img,0,d); break;
		case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
		case GLUT_KEY_LEFT  : I_move(img,d,0); break;
		case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
		default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
		}
	break;
	case vertex:
		switch(key)
		{
		case 104    		: P_focus_last(poly); break;
		case 105    		: P_focus_next(poly); break;
		case GLUT_KEY_UP    :
			if (poly->selected->y>0) 
				poly->selected->y--; 
		break;
		case GLUT_KEY_DOWN  : 
			if (poly->selected->y < img->_height-1)
				poly->selected->y++; 
		break;
		case GLUT_KEY_LEFT  : 
			if (poly->selected->x>0)
				poly->selected->x--;
		break;
		case GLUT_KEY_RIGHT : 
			if (poly->selected->x < img->_width-1)
				poly->selected->x++;
		break;
		default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
		}
	break;
	case edge:
		switch(key)
		{
		case 104    		: P_focus_last(poly); break;
		case 105    		: P_focus_next(poly); break;
		default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
		}
	break;
	default : fprintf(stderr,"special_CB : %d : unknown mode.\n",mode);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	poly = P_new_poly();
	if(argc!=3)
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur = atoi(argv[1]), hauteur = atoi(argv[2]);
		img = I_new(largeur,hauteur);
		int windowPosX = 100, windowPosY = 100;

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,hauteur,0,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
