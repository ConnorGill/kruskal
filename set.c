#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "da.h"
#include "integer.h"

typedef enum {false = 0, true = 1} bool;
struct set
{
	void (*display)(void *ptr, FILE *fp);
	void (*free)(void *ptr);
	DA *representatives;
	int numReps;
	int level;
};

typedef struct vertex2 vertex2;
struct vertex2
{

	unsigned repIndex;

	int isRep;
	unsigned index;

	void* value;
	int rank;
};

typedef struct edge edge;
struct edge
{
	INTEGER* weight;
};


int rank(SET *d, vertex2 *v);

vertex2 * newVertex2(unsigned rep, void* store)
{
	vertex2 *newVertex = (vertex2*)malloc(sizeof(vertex2));
	assert(newVertex != 0);

	newVertex->repIndex = rep;
	newVertex->index = 0;
	newVertex->value = store;
	newVertex->rank = 0;
	newVertex->isRep = 1;


	return newVertex;
}

extern SET *newSET(void)
{

	SET *newSet = (SET*)malloc(sizeof(SET));
	assert(newSet != 0);
	newSet->display = 0;
	newSet->free = 0;
	newSet->representatives = newDA();
	assert(newSet->representatives != 0);
	newSet->numReps = 0;
	newSet->level = 0;

	return newSet;
}

extern void  setSETdisplay(SET *s,void (*d)(void * ptr,FILE * fp))
{
	s->display = d;
}
extern void  setSETfree(SET *s,void (*d)(void * ptr))
{
	s->free = d;
}

extern int makeSET(SET *d,void* value)
{

	vertex2 * newValue = newVertex2(0, value);
	newValue->repIndex = sizeDA(d->representatives);

	newValue->index = newValue->repIndex;
	int tempIndex = sizeDA(d->representatives);
	insertDA(d->representatives, tempIndex, newValue);
	d->numReps++;

	return (sizeDA(d->representatives)-1);
}

extern int findSET(SET *d,int index)
{

	vertex2 *value = getDA(d->representatives, index);


	if(value->index != value->repIndex)
	{

		value->repIndex = findSET(d, value->repIndex);
	}

	return value->repIndex;
}

extern int unionSET(SET *d,int index1,int index2)
{
	if(findSET(d, index1) == findSET(d, index2)) return 0;
	d->numReps--;
	vertex2 *v1 = getDA(d->representatives, index1);
	vertex2 *v2 = getDA(d->representatives, index2);
	vertex2 *v1Rep = v1; while(v1Rep->index != v1Rep->repIndex) {v1Rep = getDA(d->representatives, v1Rep->repIndex);}
	vertex2 *v2Rep = v2; while(v2Rep->index != v2Rep->repIndex) {v2Rep = getDA(d->representatives, v2Rep->repIndex);}


	if(rank(d, v1Rep) > rank(d, v2Rep))
	{

		v2Rep->repIndex = v1Rep->repIndex;
		v2Rep->isRep = 0;
		//v2Rep->rank++;

	}
	else if(rank(d, v1Rep) < rank(d, v2Rep))
	{

		v1Rep->repIndex = v2Rep->repIndex;
		v1Rep->isRep = 0;
		//v1Rep->rank++;

	}
	else
	{

		if(v1Rep->repIndex < v2Rep->repIndex)
		{

			v2Rep->repIndex = v1Rep->repIndex;
			//v2Rep->rank++;
			v1Rep->rank++;
			//v2Rep->isRep = 1;
			v2Rep->isRep = 0;
			v1Rep->isRep = 1;
		}
		else
		{

			v1Rep->repIndex = v2Rep->repIndex;
			//v1Rep->rank++;
			v2Rep->rank++;
			v2Rep->isRep = 1;
			v1Rep->isRep = 0;

		}
	}

	return 1;

}

extern void displaySET(SET *d, FILE *fp)
{

	int i = 0;

	for(i = 0; i < sizeDA(d->representatives); i++)
	{
		fprintf(fp, "%d: ", i);
		vertex2 *currVertex = getDA(d->representatives, i);
		d->display(currVertex->value, fp);
		if(currVertex->isRep == 1)
		{
			fprintf(fp, "(%d)", currVertex->rank);
		}
		while(currVertex->index != currVertex->repIndex)
		{

			currVertex = getDA(d->representatives, currVertex->repIndex);

			fprintf(fp, " ");
			d->display(currVertex->value, fp);

		}
		fprintf(fp, "\n");
	}

	return;

}

int rank(SET *d, vertex2 *v)
{
	if(v->repIndex == v->index) return v->rank;
	else return rank(d, getDA(d->representatives, v->repIndex));

}

extern int   debugSET(SET *d,int level)
{
	d->level = level;
	return 0;
}

extern void  freeSET(SET *d)
{
  if(d->numReps == 0)
  {
    free(d);
    return;
  }

	for(int i = 0; i < sizeDA(d->representatives); i++)
	{
		vertex2 *currVertex = getDA(d->representatives, i);
		free(currVertex->value);
		free(currVertex);
	}

	freeDA(d->representatives);
  free(d);
}
