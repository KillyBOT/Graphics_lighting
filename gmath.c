
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i, iA, iD, iS;

  normalize(normal);
  normalize(view);

  iA = calculate_ambient(alight, areflect);
  iD = calculate_diffuse(light, dreflect, normal);
  iS = calculate_specular(light, sreflect, view, normal);

  //printf("%d %d %d %d %d %d %d %d %d\n\n",iA.red,iA.green,iA.blue,iD.red,iD.green,iD.blue,iS.red,iS.green,iS.blue);

  i.red = iA.red + iD.red + iS.red;
  i.green = iA.green + iD.green + iS.green;
  i.blue = iA.blue + iD.blue + iS.blue;

  //printf("%d %d %d\n", i.red, i.green, i.blue);
  limit_color(&i);
  //printf("%d %d %d\n\n", i.red, i.green, i.blue);

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = alight.red * areflect[0];
  a.green = alight.green * areflect[1];
  a.blue = alight.blue * areflect[2];

  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;

  double p[3];
  double l[3];
  double cosine;

  l[0] = light[0][0];
  l[1] = light[0][1];
  l[2] = light[0][2];

  p[0] = light[1][0];
  p[1] = light[1][1];
  p[2] = light[1][2];

  normalize(l);
  
  cosine = dot_product(normal,l);
  if(cosine < 0) cosine = 0;
  //printf("Cosine (Diffuse): %lf\n", cosine);

  d.red = p[0] * dreflect[0] * cosine;
  d.green = p[1] * dreflect[1] * cosine;
  d.blue = p[2] * dreflect[2] * cosine;

  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {

  color s;

  double p[3];
  double l[3];
  double t[3];
  double r[3];
  double temp, cosine;
  double sPower = 2;

  l[0] = light[0][0];
  l[1] = light[0][1];
  l[2] = light[0][2];

  normalize(l);

  p[0] = light[1][0];
  p[1] = light[1][1];
  p[2] = light[1][2];

  temp = dot_product(normal, l);

  t[0] = normal[0] * temp;
  t[1] = normal[1] * temp;
  t[2] = normal[2] * temp;

  r[0] = (2 * t[0]) - l[0];
  r[1] = (2 * t[1]) - l[1];
  r[2] = (2 * t[2]) - l[2];

  cosine = dot_product(r,view);
  //printf("Cosine (Specular): %lf\n", cosine);
  if(cosine < 0) cosine = 0;
  cosine = pow(cosine, sPower);

  s.red = p[0] * sreflect[0] * cosine;
  s.green = p[1] * sreflect[1] * cosine;
  s.blue = p[2] * sreflect[2] * cosine;

  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255) c->red = 255;
  if(c->green > 255) c->green = 255;
  if(c->blue > 255) c->blue = 255;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
