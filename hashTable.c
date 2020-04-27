#include <stdio.h>
#include <stdlib.h>

#include "hashTable.h"
#include "gmath.h"

struct htElement** createHT(){
	struct htElement** ht = malloc(sizeof(struct htElement) * HASHTABLE_SIZE);

	for(int x = 0; x < HASHTABLE_SIZE; x++){
		if(ht[x] != NULL){
			free(ht[x]);
		}
		ht[x] = NULL;
	}

	return ht;
}

void freeHT(struct htElement** ht){
	for(int x = 0; x < HASHTABLE_SIZE; x++){
		if(ht[x] != NULL) free(ht[x]);
	}
	free(ht);
}

unsigned int getKey(double * vertex){
	unsigned int f = ((unsigned int)vertex[0] + (unsigned int)vertex[1] * 11 - (unsigned int)vertex[2] * 17);
	return ((f >> 22) ^ (f >> 12) ^ f) % HASHTABLE_SIZE;
}

/*void addTouching(struct htElement** ht, double * vertex, double * touching){
	int vKey = getKey(vertex);
	int index = vKey;

	while(ht[index] != NULL && ht[index]->vertex[0] != vertex[0] && ht[index]->vertex[1] != vertex[1] && ht[index]->vertex[2] != vertex[2]){
		index++;
		index %= HASHTABLE_SIZE;
	}

	if(ht[index] == NULL) {
		ht[index] = malloc(sizeof(struct htElement));
		ht[index]->vertex[0] = vertex[0];
		ht[index]->vertex[1] = vertex[1];
		ht[index]->vertex[2] = vertex[2];
		ht[index]->vNormal[0] = 0;
		ht[index]->vNormal[1] = 0;
		ht[index]->vNormal[2] = 0;
		ht[index]->touchingTotal = 1;
		ht[index]->touching[0][0] = touching[0];
		ht[index]->touching[0][1] = touching[1];
		ht[index]->touching[0][2] = touching[2];
	} else {

		for(int x = 0; x < ht[index]->touchingTotal; x++){
			if(ht[index]->touching[x][0] == touching[0] && ht[index]->touching[x][1] == touching[1] && ht[index]->touching[x][2] == touching[2]){
				return;
			}
		}

		ht[index]->touching[ht[index]->touchingTotal][0] = touching[0];
		ht[index]->touching[ht[index]->touchingTotal][1] = touching[1];
		ht[index]->touching[ht[index]->touchingTotal][2] = touching[2];

		ht[index]->touchingTotal++;
	}

}*/

void addNormal(struct htElement** ht, double* vertex, double* normal){
	// double vNew[3];
	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

	unsigned int index = getKey(vertex);

	while(ht[index] != NULL && (ht[index]->vertex[0] != vertex[0] || ht[index]->vertex[1] != vertex[1] || ht[index]->vertex[2] != vertex[2])){
		index++;
		index %= HASHTABLE_SIZE;
	}

	if(ht[index] == NULL) {
		ht[index] = malloc(sizeof(struct htElement));
		ht[index]->vertex[0] = vertex[0];
		ht[index]->vertex[1] = vertex[1];
		ht[index]->vertex[2] = vertex[2];
		ht[index]->normal[0] = normal[0];
		ht[index]->normal[1] = normal[1];
		ht[index]->normal[2] = normal[2];
		ht[index]->num = 1;
	} else {
		ht[index]->normal[0] += normal[0];
		ht[index]->normal[1] += normal[1];
		ht[index]->normal[2] += normal[2];
		ht[index]->num++;
	}
}

double* getNormal(struct htElement** ht, double* vertex){

	// double vNew[3];
	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

	unsigned int index = getKey(vertex);

	while(ht[index] != NULL && ht[index]->vertex[0] != vertex[0] && ht[index]->vertex[1] != vertex[1] && ht[index]->vertex[2] != vertex[2]){
		index++;
		index %= HASHTABLE_SIZE;
	}

	return ht[index]->normal;
}

color getColor(struct htElement** ht, double* vertex){
	// double vNew[3];
	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

	int index = getKey(vertex);

	while(ht[index]->vertex[0] != vertex[0] || ht[index]->vertex[1] != vertex[1] || ht[index]->vertex[2] != vertex[2]){
		index++;
		index %= HASHTABLE_SIZE;
	}
	//printf("%d %d %d\n", ht[index]->c.red, ht[index]->c.green, ht[index]->c.blue);
	return ht[index]->c;
	//return c;
}

void htNormalize(struct htElement** ht, double *view, double light[2][3], color ambient,
  double *areflect, double *dreflect, double *sreflect){
	for(int x = 0; x < HASHTABLE_SIZE; x++){
		if(ht[x] != NULL){
			ht[x]->normal[0] /= ht[x]->num;
			ht[x]->normal[1] /= ht[x]->num;
			ht[x]->normal[2] /= ht[x]->num;
			//normalize(ht[x]->normal);
			ht[x]->c = get_lighting(ht[x]->normal, view, ambient, light, areflect, dreflect, sreflect);
			//printf("%f\n",ht[x]->normal[0]*ht[x]->normal[0] + ht[x]->normal[1]*ht[x]->normal[1] + ht[x]->normal[2]*ht[x]->normal[2]);
		}
	}
}

void printHT(struct htElement** ht){
	int total = 0;
	for(int x = 0; x < HASHTABLE_SIZE; x++){
		if(ht[x] != NULL){
			printf("%f %f %f\n\t%f %f %f\n\tIntensity:%d %d %d\n", ht[x]->vertex[0], ht[x]->vertex[1], ht[x]->vertex[2], ht[x]->normal[0], ht[x]->normal[1], ht[x]->normal[2],ht[x]->c.red, ht[x]->c.green, ht[x]->c.blue);
			total++;
		}
	}
	printf("Total: %d\n\n", total);
}
//int * getTouching(struct htElement** ht, double * vertex);
//double * getVertexKey(struct htElement** ht, int key);
