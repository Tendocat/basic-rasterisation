/*====================================================*\
  Arash HABIBI, ANtoine DUMOULIN
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "liste.h"
#include "polygone.h"

//-----------------------------------------------------

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
void C_check(Color c, char *message);
bool C_eq(Color ca, Color cb);

//-----------------------------------------------------

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

Image* I_new(int _width, int _height);

void I_fill       (Image *img, Color c);

void I_changeColor(Image *img, Color c);
void I_plot       (Image *img, int x, int y);
void I_plotColor  (Image *img, int x, int y, Color c);

void I_focusPoint (Image *img, int x, int y);
void I_zoomInit   (Image *img);
void I_zoom       (Image *img, double zoom_coef);
void I_move       (Image *img, int x, int y);

void I_draw       (Image *img);

/*
 * les deux fonctions implémentées
 */
void I_bresenham(Image *img, int xA, int yA, int xB, int yB, Color c);
void I_fill_scan_line(Image* img, Polygone* poly, Color cline);
#endif



