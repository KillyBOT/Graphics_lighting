#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HASHTABLE_SIZE 1048576

#include "ml6.h"

struct htElement{
	double vertex[3];
	double normal[3];
	int num;
	int key;
	color c;
};

struct htElement** createHT();
void freeHT(struct htElement** ht);

unsigned int getKey(double * vertex);

void addNormal(struct htElement** ht, double* vertex, double* normal);
double * getNormal(struct htElement** ht, double * vertex);
color getColor(struct htElement** ht, double* vertex);
double * getVertexKey(struct htElement** ht, int key);
void printHT(struct htElement** ht);
void htNormalize(struct htElement** ht, double *view, double light[2][3], color ambient,
  double *areflect, double *dreflect, double *sreflect);
#endif