/*====================================================*\
  Arash HABIBI, Antoine DUMOULIN
  Image.c
\*====================================================*/

#include "Image.h"

//------------------------------------------------------------------------

Color C_new(float red, float green, float blue)
{
	Color c;
	c._red = red;
	c._green = green;
	c._blue = blue;
	return c;
}

//------------------------------------------------------------------------

void C_check(Color c, char *message)
{
	fprintf(stderr,"%s : %f %f %f\n",message,c._red,c._green,c._blue);
}

// test si deux couleurs sont égales
bool C_eq(Color ca, Color cb)
{
	return (ca._blue==cb._blue && ca._green==cb._green && ca._red==cb._red);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

Image* I_new(int width, int height)
{
	Image *img_new = (Image*)malloc(sizeof(Image));
	img_new->_width = width;
	img_new->_height = height;
	img_new->_xzoom = 0;
	img_new->_yzoom = 0;
	img_new->_zoom = 1.0;

	img_new->_xoffset=0;
	img_new->_yoffset=0;

	img_new->_current_color = C_new(255,255,255);

	img_new->_buffer = (Color**)calloc(width,sizeof(Color*));

	int x;
	for(x=0;x<width;x++)
		img_new->_buffer[x] = (Color*)calloc(height,sizeof(Color));

	return img_new;
}

//-----

void I_fill(Image *img, Color c)
{
	int x,y;
	for(x=0;x<img->_width;x++)
		for(y=0;y<img->_height;y++)
			img->_buffer[x][y]=c;
}

//------------------------------------------------------------------------

void I_changeColor(Image *img, Color c)
{
	img->_current_color = c;
}

//------------------------------------------------------------------------

void I_plot(Image *img, int x, int y)
{
	if((x>=0)&&(x<img->_width)&&
	   (y>=0)&&(y<img->_height))
		img->_buffer[x][y] = img->_current_color;
	else
	{
		fprintf(stderr,"I_plot : ERROR !!!\n");
		fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, img->_width);
		fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, img->_height);
	}
}

//------------------------------------------------------------------------

void I_plotColor(Image *img, int x, int y, Color c)
{
	if((x>=0)&&(x<img->_width)&&
	   (y>=0)&&(y<img->_height))
		img->_buffer[x][y] = c;
	else
	{
		fprintf(stderr,"I_plotColor : ERROR !!!\n");
		fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, img->_width);
		fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, img->_height);
	}
}

//------------------------------------------------------------------------
// Changement de repère

static void _windowToImage(Image *img, int xwin, int ywin, int *ximg, int *yimg)
{

	*ximg = img->_xoffset + img->_xzoom + (xwin-img->_xzoom) / img->_zoom;
	*yimg = img->_yoffset + img->_yzoom + (ywin-img->_yzoom) / img->_zoom;
}

//-----

void I_focusPoint(Image *img, int xwin, int ywin)
{
	int dx = xwin - img->_xzoom;
	int dy = ywin - img->_yzoom;
	img->_xoffset -= dx*(1-1.0/img->_zoom);
	img->_yoffset -= dy*(1-1.0/img->_zoom);
	img->_xzoom = xwin;
	img->_yzoom = ywin;
}

//------------------------------------------------------------------------

void I_zoomInit(Image *img)
{
	img->_xoffset = 0;
	img->_yoffset = 0;
	img->_zoom = 1.0;
}

//------------------------------------------------------------------------

void I_zoom(Image *img, double zoom_coef)
{
	img->_zoom = img->_zoom * zoom_coef;
}

//------------------------------------------------------------------------

void I_move(Image *img, int x, int y)
{
	img->_xoffset += x;
	img->_yoffset += y;
}

//------------------------------------------------------------------------

void I_draw(Image *img)
{
	glBegin(GL_POINTS);
	int xwin, ywin, ximg, yimg;
	for(xwin=0;xwin<img->_width;xwin++)
		for(ywin=0;ywin<img->_height;ywin++)
		{
			_windowToImage(img, xwin, ywin, &ximg, &yimg);
			Color c;
			if((ximg>=0)&&(ximg<img->_width)&&
			   (yimg>=0)&&(yimg<img->_height))
				c = img->_buffer[ximg][yimg];
			else
				c = C_new(0,0,0);

			glColor3f(c._red,c._green,c._blue);
			glVertex2i(xwin,ywin);
		}
	glEnd();
}

//------------------------------------------------------------------------

/*
 * Fonction du tracé de Bresenham comme vu en TD
 */
void Z2to1octant (int xA, int yA, int xB, int yB, 
				  int* xA10, int* yA10, int* xB10, int* yB10)
{
	int xAo, xBo, yAo, yBo;
	if (xB > xA) {
		xAo = xA;
		xBo = xB;
	} else {
		xAo = -xA;
		xBo = -xB;
	}
	if (yB > yA) {
		yAo = yA;
		yBo = yB;
	} else {
		yAo = -yA;
		yBo = -yB;
	}
	if (abs(xB-xA) < abs(yB-yA)) {
		*xA10 = yAo;
		*xB10 = yBo;
		*yA10 = xAo;
		*yB10 = xBo;
	} else {
		*xA10 = xAo;
		*xB10 = xBo;
		*yA10 = yAo;
		*yB10 = yBo;
	}
}
void octanttoZ2 (int xA, int yA, int xB, int yB, 
				  int a, int b,
				  int* xZ, int* yZ)
{
	int x, y;
	x = abs(a);
	y = abs(b);
	if (abs(xB-xA) < abs(yB-yA)) {
		*xZ = y;
		*yZ = x;
	} else {
		*xZ = x;
		*yZ = y;
	}
}
void I_bresenham(Image *img, int xA, int yA, int xB, int yB, Color c)
{
	int xA10, xB10, yA10, yB10;
	Z2to1octant (xA, yA, xB, yB, &xA10, &yA10, &xB10, &yB10);
	int dx = xB10-xA10;
	int dy = yB10-yA10;
	int incr1 = 2 * dy;
	int incr2 = 2 * (dy-dx);
	int d = 2*dy-dx;
	int aZ = 0, bZ = 0;
	int b = yA10;
	int a;
	for (a = xA10; a < xB10; a++)
	{
		octanttoZ2 (xA, yA, xB, yB, a, b, &aZ, &bZ);
		I_plotColor(img, aZ, bZ, c);
		if (d < 0) {
			d += incr1;
		} else {
			b++;
			d += incr2;
		}
	}
	octanttoZ2 (xA, yA, xB, yB, a, b, &aZ, &bZ);
	I_plotColor(img, aZ, bZ, c);
}

/*
 * Remplissage scanline
 */
void I_fill_scan_line(Image* img, Polygone* poly, Color cline)
{
	Liste* liste = NULL;
	bool alterne = true;
	bool tangent = false;

    // parcours en hauteur de l'écran
	for(int y=0; y<img->_height; y++)
	{
	    // parcours en largeur de l'écran
		for (int x=0; x<img->_width; x++)
		{
			if (C_eq(img->_buffer[x][y], cline))
				liste = liste_add(liste, x, y);
		}
		alterne = true;
		while (liste!=NULL && liste != liste->last)
		{
			tangent = false;
			// on regarde si le point est tangent, si oui on alterne pas
			if (P_is_tangent(poly, liste->x, y))
				tangent = true;

			// tant que le prochain point de la liste est adjacent en X on parcours la liste
			while (liste->last != liste && liste->x == liste->last->x + 1)
			{
			    // on vérifie encore si un des points parcouru est tangent
				if (P_is_tangent(poly, liste->x, y))
					tangent = true;
				Liste* tmp = liste_last(liste);
				liste_rm(liste);
				liste = tmp;
			}
			
			if (tangent) alterne = !alterne; // on réinverse alterne car le un point est tangent

			// une fois sur deux on va remplir du point courant jusqu'au prochain de la liste
			if (liste != NULL && liste->last != liste && alterne) {
				for (int i=liste->x-1; i > liste->last->x; i--)
					I_plotColor(img, i, y, cline);
			}
			liste = liste_rm(liste);
			alterne = !alterne;
		}
	}
	liste_destruct(liste);
}
