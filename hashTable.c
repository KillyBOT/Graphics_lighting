#include <stdio.h>
#include <stdlib.h>

#include "hashTable.h"
#include "gmath.h"

// struct htElement** createHT(){
// 	struct htElement** ht = malloc(sizeof(struct htElement) * HASHTABLE_SIZE);

// 	for(int x = 0; x < HASHTABLE_SIZE; x++){
// 		ht[x] = NULL;
// 	}

// 	return ht;
// }

// void freeHT(struct htElement** ht){
// 	for(int x = 0; x < HASHTABLE_SIZE; x++){
// 		if(ht[x] != NULL) free(ht[x]);
// 	}
// 	free(ht);
// }

// unsigned int getKey(double * vertex){
// 	unsigned int f = ((unsigned int)vertex[0] + (unsigned int)vertex[1] * 11 - (unsigned int)vertex[2] * 17);
// 	return ((f >> 22) ^ (f >> 12) ^ f) % HASHTABLE_SIZE;
// }

// void addNormal(struct htElement** ht, double* vertex, double* normal){

// 	unsigned int index = getKey(vertex);

// 	while(ht[index] != NULL && (ht[index]->vertex[0] != vertex[0] || ht[index]->vertex[1] != vertex[1] || ht[index]->vertex[2] != vertex[2])){
// 		index++;
// 		index %= HASHTABLE_SIZE;
// 	}

// 	if(ht[index] == NULL) {
// 		ht[index] = malloc(sizeof(struct htElement));
// 		ht[index]->vertex[0] = vertex[0];
// 		ht[index]->vertex[1] = vertex[1];
// 		ht[index]->vertex[2] = vertex[2];
// 		ht[index]->normal[0] = normal[0];
// 		ht[index]->normal[1] = normal[1];
// 		ht[index]->normal[2] = normal[2];
// 		ht[index]->num = 1;
// 	} else {
// 		ht[index]->normal[0] += normal[0];
// 		ht[index]->normal[1] += normal[1];
// 		ht[index]->normal[2] += normal[2];
// 		ht[index]->num++;
// 	}
// }

// double* getNormal(struct htElement** ht, double* vertex){

// 	// double vNew[3];
// 	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
// 	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
// 	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

// 	unsigned int index = getKey(vertex);

// 	while(ht[index] != NULL && ht[index]->vertex[0] != vertex[0] && ht[index]->vertex[1] != vertex[1] && ht[index]->vertex[2] != vertex[2]){
// 		index++;
// 		index %= HASHTABLE_SIZE;
// 	}

// 	return ht[index]->normal;
// }

// color getColor(struct htElement** ht, double* vertex){
// 	// double vNew[3];
// 	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
// 	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
// 	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

// 	int index = getKey(vertex);

// 	while(ht[index]->vertex[0] != vertex[0] || ht[index]->vertex[1] != vertex[1] || ht[index]->vertex[2] != vertex[2]){
// 		index++;
// 		index %= HASHTABLE_SIZE;
// 	}
// 	//printf("%d %d %d\n", ht[index]->c.red, ht[index]->c.green, ht[index]->c.blue);
// 	return ht[index]->c;
// 	//return c;
// }

// void htNormalize(struct htElement** ht, double *view, double light[2][3], color ambient,
//   double *areflect, double *dreflect, double *sreflect){
// 	for(int x = 0; x < HASHTABLE_SIZE; x++){
// 		if(ht[x] != NULL){
// 			ht[x]->normal[0] /= ht[x]->num;
// 			ht[x]->normal[1] /= ht[x]->num;
// 			ht[x]->normal[2] /= ht[x]->num;
// 			//normalize(ht[x]->normal);
// 			ht[x]->c = get_lighting(ht[x]->normal, view, ambient, light, areflect, dreflect, sreflect);
// 			//printf("%f\n",ht[x]->normal[0]*ht[x]->normal[0] + ht[x]->normal[1]*ht[x]->normal[1] + ht[x]->normal[2]*ht[x]->normal[2]);
// 		}
// 	}
// }

// void printHT(struct htElement** ht){
// 	int total = 0;
// 	for(int x = 0; x < HASHTABLE_SIZE; x++){
// 		if(ht[x] != NULL){
// 			printf("%f %f %f\n\t%f %f %f\n\tIntensity:%d %d %d\n", ht[x]->vertex[0], ht[x]->vertex[1], ht[x]->vertex[2], ht[x]->normal[0], ht[x]->normal[1], ht[x]->normal[2],ht[x]->c.red, ht[x]->c.green, ht[x]->c.blue);
// 			total++;
// 		}
// 	}
// 	printf("Total: %d\n\n", total);
// }
//int * getTouching(struct htElement** ht, double * vertex);
//double * getVertexKey(struct htElement** ht, int key);

//This is using a new hashTable structure

struct hashTable* createHT(){
	struct hashTable* h = malloc(sizeof(struct hashTable));
	h->size = 0;
	h->maxSize = 8;

	h->ht = malloc(sizeof(struct htElement) * h->maxSize);

	for(int n = 0; n < h->maxSize; n++){
		h->ht[n] = NULL;
	}

	return h;
}

void freeHT(struct hashTable* h){
	for(int x = 0; x < h->maxSize; x++){
		if(h->ht[x] != NULL) free(h->ht[x]);
	}
	free(h->ht);
	free(h);
}

unsigned int getKey(struct hashTable* h, double * vertex){
	unsigned int f = ((unsigned int)vertex[0] + (unsigned int)vertex[1] * 11 - (unsigned int)vertex[2] * 17);
	return ((f >> 22) ^ (f >> 12) ^ f) % h->maxSize;
}

struct hashTable* addNormal(struct hashTable* h, double* vertex, double* normal){
	//printf("Test\n");

	struct hashTable* hNew;

	if(h->size >= h->maxSize){
		//printf("Expanding from %d to %d\n",h->size,h->size*2);
		hNew = createHT();
		hNew->maxSize = h->maxSize*2;
		hNew->ht = realloc(hNew->ht, sizeof(struct htElement) * hNew->maxSize);

		for(int n = 0; n < hNew->maxSize; n++){
			hNew->ht[n] = NULL;
		}

		for(int n = 0; n < h->maxSize; n++){
			addNormal(hNew, h->ht[n]->vertex,h->ht[n]->normal);
		}
		freeHT(h);
		addNormal(hNew, vertex, normal);
		return hNew;
	}

	unsigned int index = getKey(h, vertex);

	while(h->ht[index] != NULL && (h->ht[index]->vertex[0] != vertex[0] || h->ht[index]->vertex[1] != vertex[1] || h->ht[index]->vertex[2] != vertex[2])){
		index++;
		index %= h->maxSize;
	}

	if(h->ht[index] == NULL) {
		h->ht[index] = malloc(sizeof(struct htElement));
		h->ht[index]->vertex[0] = vertex[0];
		h->ht[index]->vertex[1] = vertex[1];
		h->ht[index]->vertex[2] = vertex[2];
		h->ht[index]->normal[0] = normal[0];
		h->ht[index]->normal[1] = normal[1];
		h->ht[index]->normal[2] = normal[2];
		h->ht[index]->num = 1;
		h->size++;
	} else {
		h->ht[index]->normal[0] += normal[0];
		h->ht[index]->normal[1] += normal[1];
		h->ht[index]->normal[2] += normal[2];
		h->ht[index]->num++;
	}

	return h;
}

// double* getNormal(struct htElement** ht, double* vertex){

// 	// double vNew[3];
// 	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
// 	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
// 	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

// 	unsigned int index = getKey(vertex);

// 	while(ht[index] != NULL && ht[index]->vertex[0] != vertex[0] && ht[index]->vertex[1] != vertex[1] && ht[index]->vertex[2] != vertex[2]){
// 		index++;
// 		index %= HASHTABLE_SIZE;
// 	}

// 	return ht[index]->normal;
// }

color getColor(struct hashTable* h, double* vertex){
	// double vNew[3];
	// vNew[0] = (int)(vertex[0]*1000)/1000.0;
	// vNew[1] = (int)(vertex[1]*1000)/1000.0;
	// vNew[2] = (int)(vertex[2]*1000)/1000.0;

	unsigned int index = getKey(h, vertex);

	while(h->ht[index]->vertex[0] != vertex[0] || h->ht[index]->vertex[1] != vertex[1] || h->ht[index]->vertex[2] != vertex[2]){
		index++;
		index %= h->maxSize;
	}
	//printf("%d %d %d\n", ht[index]->c.red, ht[index]->c.green, ht[index]->c.blue);
	return h->ht[index]->c;
	//return c;
}

void htNormalize(struct hashTable* h, double *view, double light[2][3], color ambient,
  double *areflect, double *dreflect, double *sreflect){
	for(int x = 0; x < h->maxSize; x++){
		if(h->ht[x] != NULL){
			h->ht[x]->normal[0] /= h->ht[x]->num;
			h->ht[x]->normal[1] /= h->ht[x]->num;
			h->ht[x]->normal[2] /= h->ht[x]->num;
			//normalize(ht[x]->normal);
			h->ht[x]->c = get_lighting(h->ht[x]->normal, view, ambient, light, areflect, dreflect, sreflect);
			//printf("%f\n",ht[x]->normal[0]*ht[x]->normal[0] + ht[x]->normal[1]*ht[x]->normal[1] + ht[x]->normal[2]*ht[x]->normal[2]);
		}
	}
}

void printHT(struct hashTable* h){
	for(int x = 0; x < h->maxSize; x++){
		if(h->ht[x] != NULL){
			printf("%f %f %f\n\t%f %f %f\n\tIntensity:%d %d %d\n", h->ht[x]->vertex[0], h->ht[x]->vertex[1], h->ht[x]->vertex[2], h->ht[x]->normal[0], h->ht[x]->normal[1], h->ht[x]->normal[2],h->ht[x]->c.red, h->ht[x]->c.green, h->ht[x]->c.blue);
		}
	}
	printf("Total size: %d\n\n", h->size);
}