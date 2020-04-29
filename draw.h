
#ifndef DRAW_H
#define DRAW_H

#include "matrix.h"
#include "ml6.h"
#include "hashTable.h"


void draw_scanline(int x0, double z0, int x1, double z1, int y, screen s, zbuffer zb, color c0, color c1);
void draw_scanline_phong(int x0, double z0, int x1, double z1, int y, screen s, zbuffer zb, double* n0, double* n1,
  double *view, double light[2][3], color ambient,
  double *areflect, double *dreflect, double *sreflect);
void scanline_convert( struct matrix *points, int i, screen s, zbuffer zb, struct hashTable* h);
void scanline_convert_phong( struct matrix* points, int i, screen s, zbuffer zb, struct htElement** ht, 
  double *view, double light[2][3], color ambient,
  double *areflect, double *dreflect, double *sreflect);
void draw_line_horizontal(int x0, int x1, 
  int y, double z0, double z1, 
  screen s, zbuffer zb, color c);
//polygon organization
void add_polygon( struct matrix * polys,
                   double x0, double y0, double z0,
                   double x1, double y1, double z1,
                   double x2, double y2, double z2);
void draw_polygons( struct matrix * polys, screen s, zbuffer zb, color c,
                    double *view, double light[2][3], color ambient,
                    double *areflect, double *dreflect, double *sreflect);

//advanced shapes
//3d shapes
void add_box( struct matrix * edges,
              double x, double y, double z,
              double width, double height, double depth);
void add_sphere( struct matrix * edges,
                 double cx, double cy, double cz,
                 double r, int step);
struct matrix * generate_sphere(double cx, double cy, double cz,
                                double r, int step);
void add_torus( struct matrix * edges,
                double cx, double cy, double cz,
                double r1, double r2, int step);
struct matrix * generate_torus( double cx, double cy, double cz,
                                double r1, double r2, int step);

//2D Curves
void add_circle( struct matrix * points,
                 double cx, double cy, double cz,
                 double r, int step );
void add_curve( struct matrix *points,
                double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                int step, int type );

void add_point( struct matrix * points, double x, double y, double z);
void add_edge( struct matrix * points,
               double x0, double y0, double z0,
               double x1, double y1, double z1);
void draw_lines( struct matrix * points, screen s, zbuffer zb, color c);
void draw_line(int x0, int y0, double z0,
               int x1, int y1, double z1,
               screen s, zbuffer zb, color c);

#endif
