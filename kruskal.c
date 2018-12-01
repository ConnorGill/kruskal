#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "da.h"
#include "queue.h"
#include "rbt.h"
#include "set.h"
#include "string.h"
#include "integer.h"
#include "scanner.h"


typedef enum {false = 0, true = 1}bool;

typedef struct vertexWrapper
{
	int index;
	int value;
}vertexWrapper;

typedef struct edge
{
	INTEGER *weight;
	int src;
	int dest;
}edge;

typedef struct vertex
{
	int value;
	bool visited;
	DA *edgeList;
}vertex;

edge * newEdge(int edgeVertex)
{
	edge* newEdge1 = (edge*)malloc(sizeof(edge));
	assert(newEdge1 != 0);
	newEdge1->weight = newINTEGER(edgeVertex);

	return newEdge1;
}

void displayEDGE (void *x, FILE* fp)
{
	edge *y = x;
	fprintf(fp, "%d", y->dest);
	fprintf(fp, "(%d)", y->src);
	displayINTEGER(y->weight, fp);
}

vertex* newVertex(int x)
{
	vertex* newVertex = (vertex*)malloc(sizeof(vertex));
	assert(newVertex != 0);
	newVertex->value = x;
	newVertex->visited = false;
	newVertex->edgeList = newDA();
	setDAdisplay(newVertex->edgeList, displayEDGE);

	return newVertex;
}

void displayVERTEX(void* x, FILE *fp)
{
	vertex* y = x;
	fprintf(fp, "%d", y->value);
}



void displayGRAPH(FILE *fp, vertex **vertexArray, int numVertices);
int compareEDGE(void *, void*);
int compareEDGE2(void *, void*);

void TopDownMergeSort(int A[], int B[], int n);
void TopDownSplitMerge(int B[], int iBegin, int iEnd, int A[]);
void TopDownMerge(int A[], int iBegin, int iMiddle, int iEnd, int B[]);
void CopyArray(int A[], int iBegin, int iEnd, int B[]);

void TopDownMergeSortEdge(edge* A[], edge* B[], int n);
void TopDownSplitMergeEdge(edge* B[], int iBegin, int iEnd, edge* A[]);
void TopDownMergeEdge(edge* A[], int iBegin, int iMiddle, int iEnd, edge* B[]);
void CopyArrayEdge(edge* A[], int iBegin, int iEnd, edge* B[]);

void TopDownMergeSortEdge2(edge* A[], edge* B[], int n);
void TopDownSplitMergeEdge2(edge* B[], int iBegin, int iEnd, edge* A[]);
void TopDownMergeEdge2(edge* A[], int iBegin, int iMiddle, int iEnd, edge* B[]);
void CopyArrayEdge2(edge* A[], int iBegin, int iEnd, edge* B[]);

vertex *findVertex(vertex **vertexArray, int value, int numVertices);
int findVertexIndex(vertex **vertexArray, int value, int numVertices);
void addEdge(vertex*, edge*);
edge **sortEdges(QUEUE *breadthFirst);


void TopDownMergeSortNew(void* A[], void* B[], int n);
void TopDownSplitMergeNew(void* B[], int iBegin, int iEnd, void* A[]);
void TopDownMergeNew(void* A[], int iBegin, int iMiddle, int iEnd, void* B[]);
void CopyArrayNew(void* A[], int iBegin, int iEnd, void* B[]);

void TopDownMergeSortEdgeNew(void* A[], void* B[], int n);
void TopDownSplitMergeEdgeNew(void* B[], int iBegin, int iEnd, void* A[]);
void TopDownMergeEdgeNew(void* A[], int iBegin, int iMiddle, int iEnd, void* B[]);
void CopyArrayEdgeNew(void* A[], int iBegin, int iEnd, void* B[]);


void displayVERTEXWRAPPER(void *, FILE *);
int  compareVERTEXWRAPPER(void *, void*);
int findVertexIndex2(RBT *t, int v);



int main(int argc, char*argv[])
{
	for(int a = 1; a < argc; a++)
	{
		if(strcmp("-v", argv[a]) == 0)
		{
			printf("Connor Gill\n");
			exit(0);
		}
	}

	FILE* inputFile;
	if (argc != 2)
	{
		printf("./kruskal (input file)\n");
		exit(0);
	}

	bool noInput = true;
	inputFile = fopen(argv[1], "r");
	if(fopen(argv[1], "r"))
		noInput = false;

	if(noInput == true)
		exit(0);


	int v1 = 0;
	int v2 = 0;
	int weight = 1;
	DA *edges = newDA();
	setDAdisplay(edges, displayEDGE);
	DA *vertices = newDA();
	setDAdisplay(vertices, displayINTEGER);

	while(noInput == false)
	{
		char *firstCommand = readToken(inputFile);
		if(!feof(inputFile))
		{
			if(strcmp(";", firstCommand) != 0)
			{
				v1 = 0;
				v2 = 0;
				weight = 1;

				v1 = atoi(firstCommand);

				char *secondCommand = readToken(inputFile); v2 = atoi(secondCommand);
				char *thirdCommand = readToken(inputFile);

				if(isdigit(atoi(thirdCommand)))
				{
					char *fourthCommand = readToken(inputFile);
					if(fourthCommand) fourthCommand = '\0';
				}

				if(strcmp(thirdCommand, ";") != 0)
					weight = atoi(thirdCommand);


				edge *newEdge = (edge*)malloc(sizeof(edge*));
				assert(newEdge != 0);
				newEdge->weight = newINTEGER(weight);
				newEdge->src = v1;
				newEdge->dest = v2;


				int tempIndex = sizeDA(edges);
				for (int i = 0; i < tempIndex; i++)
				{
					edge* tempEdge = getDA(edges, i);
					if ((tempEdge->src == newEdge->src && tempEdge->dest == newEdge->dest)
						|| (tempEdge->src == newEdge->dest && tempEdge->dest == newEdge->src))	//if already exists
					{
						tempEdge->weight = newEdge->weight; //update to newest weight
					}
				}

				insertDA(edges, tempIndex, newEdge);

				tempIndex = sizeDA(vertices);
				INTEGER *iV1 = newINTEGER(v1);
				insertDA(vertices, tempIndex, iV1);
				tempIndex = sizeDA(vertices);
				INTEGER *iV2 = newINTEGER(v2);
				insertDA(vertices, tempIndex, iV2);
			}

		}
		else
		{
			break;
		}

	}

	int i = 0;

	int numVertices2 = sizeDA(vertices);
	void **vertexArray = malloc(sizeof(void *) * numVertices2);
	for(i = 0; i < sizeDA(vertices); i++)
	{
		vertexArray[i] = getDA(vertices, i);
	}

	void *vertexBuffer[numVertices2];

	int numEdges2 = sizeDA(edges);
	void **edgeArray = malloc(sizeof(void *) * numEdges2);
	for(i = 0; i < sizeDA(edges); i++)
	{
		edgeArray[i] = getDA(edges, i);
	}

	void *edgeBuffer[numEdges2];

	TopDownMergeSortNew(vertexArray, vertexBuffer, numVertices2);
	TopDownMergeSortEdgeNew(edgeArray, edgeBuffer, numEdges2);

	SET* graph2 = newSET();
	setSETdisplay(graph2, displayEDGE);
	DA* sets2 = newDA();
	setDAdisplay(sets2, displayINTEGER);

	RBT *vertexTree = newRBT(compareVERTEXWRAPPER);
	setRBTdisplay(vertexTree, displayVERTEXWRAPPER);
	vertex **vertexList2 = (vertex**)malloc(sizeof(vertex*) * numVertices2);

	int j = 0;
	int numSets = 0;

	for(i = 0; i < numVertices2; i++)
	{
		if(i == 0)
		{

			int index = makeSET(graph2, vertexArray[i]);
			numSets++;
			int tempIndex2 = sizeDA(sets2);
			insertDA(sets2, tempIndex2, newINTEGER(index));
			vertexArray[j] = vertexArray[i];


			vertexWrapper *newValue = malloc(sizeof(vertexWrapper));
			newValue->value = getINTEGER(vertexArray[i]);
			newValue->index = index;

			insertRBT(vertexTree, newValue);
			vertexList2[j] = newVertex(getINTEGER(vertexArray[j]));

			j++;
		}
		else if ((i > 0) && (getINTEGER(vertexArray[i]) != getINTEGER(vertexArray[i-1])))
		{
			int index = makeSET(graph2, vertexArray[i]);
			numSets++;
			int tempIndex3 = sizeDA(sets2);
			insertDA(sets2, tempIndex3, newINTEGER(index));
			vertexArray[j] = vertexArray[i];

			vertexWrapper *newValue = malloc(sizeof(vertexWrapper));
			newValue->value = getINTEGER(vertexArray[i]);
			newValue->index = index;

			insertRBT(vertexTree, newValue);
			vertexList2[j] = newVertex(getINTEGER(vertexArray[j]));

			j++;
		}
	}

	vertexArray = realloc(vertexArray, j * sizeof(void*));
	assert(vertexArray != 0);
	vertexList2 = realloc(vertexList2, j * sizeof(vertex*));
	assert(vertexList2 != 0);
	int numVertices3 = sizeDA(sets2);


	for(i = 0; i < numEdges2; i++)
	{
		edge *currEdge = edgeArray[i];
		int u = findVertexIndex2(vertexTree, currEdge->src);
		int v = findVertexIndex2(vertexTree, currEdge->dest);

		if(findSET(graph2, u) != findSET(graph2, v))
		{
			unionSET(graph2, u, v);
			numSets--;
			vertex* src = vertexList2[u];
			vertex* dst = vertexList2[v];

			addEdge(src, currEdge);
			addEdge(dst, currEdge);
		}
	}

	displayGRAPH(stdout, vertexList2, numVertices3);

	if(inputFile)
		fclose(inputFile);
	return 0;
}


void displayGRAPH(FILE *fp, vertex **vertexArray, int numVertices)
{
	if(numVertices <= 1)
	{
		fprintf(fp, "EMPTY\n");
		return;
	}

	int i = 0;
	int tracker = numVertices;		//keeps track of remaining verts
	for(i = 0; i < numVertices; i++)
	{
		vertexArray[i]->visited = false;	//reset all vertices to false

	}


		int totalWeight = 0;

		int level = 0;
		QUEUE *breadthFirst = newQUEUE();
		setQUEUEdisplay(breadthFirst, displayEDGE);
		QUEUE *vert  		= newQUEUE();
		setQUEUEdisplay(vert, displayVERTEX);

		int firstIndex = 0;
		while(vertexArray[firstIndex]->visited == true)
			firstIndex++;

		int nonVisitedCount = 0;
		for(int k = 0; k < numVertices; k++)
		{
			if(vertexArray[k]->visited == false)
				nonVisitedCount++;
		}

		if(firstIndex > numVertices) return;
		if(tracker > 1)	//If at least one vertice is present
		{
			fprintf(fp, "%d: ", level);
			displayVERTEX(vertexArray[firstIndex], fp);
			tracker--;
			vertexArray[firstIndex]->visited = true;
			printf("\n");

			level++;

		}
		if(numVertices > 1)
		{
			vertex *first = vertexArray[firstIndex];

			for(int j = 0; j < sizeDA(first->edgeList); j++)
			{
				edge *currEdge = getDA(first->edgeList, j);

				if(currEdge->src != first->value)
				{
					int temp = currEdge->src;
					currEdge->src = currEdge->dest;
					currEdge->dest = temp;
				}

				enqueue(breadthFirst, currEdge);
				int index = findVertexIndex(vertexArray, currEdge->dest, numVertices);
				vertex* currVertex = vertexArray[index];
				vertexArray[index]->visited = true;

				enqueue(vert, currVertex);
			}
		}

		while(sizeQUEUE(breadthFirst) != 0)	//BFS search of vertices
		{
			int size = sizeQUEUE(breadthFirst);
			//fprintf(fp, "TESTING2\n");
			fprintf(fp, "%d: ", level);

			edge **sortedList = sortEdges(breadthFirst);

			int j =0;
			for(j = 0; j < size; j++)	//
			{
				edge *curr = sortedList[j];
				vertex *currVertex = findVertex(vertexArray, curr->dest, numVertices);

				displayEDGE(curr, fp);
				tracker--;	//one less vert
				vertex *previousVertex = findVertex(vertexArray, curr->src, numVertices);
				int k = 0;
				for(k = 0; k < sizeDA(currVertex->edgeList); k++)
				{
					edge *currEdge = getDA(currVertex->edgeList, k);

					if(currEdge->src != currVertex->value)	//set currEdges vals
					{
						int temp = currEdge->src;
						currEdge->src = currEdge->dest;
						currEdge->dest = temp;
					}

					if(currEdge->dest != previousVertex->value)	//if next vertices is not same as last
					{
						enqueue(breadthFirst, currEdge);
						int index = findVertexIndex(vertexArray, currEdge->dest, numVertices);
						vertex* currVertex = vertexArray[index];
						vertexArray[index]->visited = true;
						enqueue(vert, currVertex);
					}

				}

				totalWeight += getINTEGER(curr->weight);	//calculate final weight
				if(j < (size-1)) fprintf(fp, " ");
				else fprintf(fp, "\n");
			}
			level++;
		}
		if (totalWeight > 0)
		{
				fprintf(fp, "weight: %d\n", totalWeight);
		}



}

int compareEDGE(void *first, void* second)
{
	edge *x = first;
	edge *y = second;

	if(getINTEGER(x->weight) != getINTEGER(y->weight))
		return compareINTEGER(x->weight, y->weight);

	else
	{
		int es1 = x->src;
		int ed1 = x->dest;
		int es2 = y->src;
		int ed2 = y->dest;


		int minv1 = 0;
		int minv2 = 0;
		if(es1 < ed1) minv1 = es1;
		else minv1 = ed1;

		if(es2 < ed2) minv2 = es2;
		else minv2 = ed2;


		if(minv1 == minv2)
		{
			int temp1 = 0;
			int temp2 = 0;
			if(es1 == minv1) temp1 = ed1;
			else temp1 = es1;

			if(es2 == minv2) temp2 = ed2;
			else temp2 = es2;

			return(compareINTEGER(newINTEGER(temp1), newINTEGER(temp2)));
		}
		else
		{
			return (compareINTEGER(newINTEGER(minv1), newINTEGER(minv2)));
		}
	}
}

int compareVertex(void *first, void* second)
{
	INTEGER *x = first;
	INTEGER *y = second;

	return compareINTEGER(x, y);
}


void TopDownMergeSortEdge(edge* A[], edge* B[], int n)
{
	CopyArrayEdge(A, 0, n ,B);
	TopDownSplitMergeEdge(B, 0, n, A);
}

void TopDownSplitMergeEdge(edge* B[], int iBegin, int iEnd, edge* A[])
{
	if(iEnd - iBegin < 2)
		return;
	int iMiddle = (iEnd + iBegin) / 2;

	TopDownSplitMergeEdge(A, iBegin, iMiddle, B);
	TopDownSplitMergeEdge(A, iMiddle, iEnd, B);

	TopDownMergeEdge(B, iBegin, iMiddle, iEnd, A);

}

void TopDownMergeEdge(edge* A[], int iBegin, int iMiddle, int iEnd, edge* B[])
{
	int i = iBegin, j = iMiddle, k = 0;

	for(k = iBegin; k < iEnd; k++)
	{
		if(i < iMiddle && (j >= iEnd || (compareEDGE(A[i], A[j]) <= 0)))
			{
				B[k] = A[i];
				i++;
			}
		else
		{

			B[k] = A[j];
			j++;
		}
	}
}

void CopyArrayEdge(edge* A[], int iBegin, int iEnd, edge* B[])
{
	int k = 0;
	for(k = iBegin; k < iEnd; k++)
	{
		B[k] = A[k];
	}
}


void TopDownMergeSort(int A[], int B[], int n)
{

	CopyArray(A, 0, n ,B);
	TopDownSplitMerge(B, 0, n, A);
}

void TopDownSplitMerge(int B[], int iBegin, int iEnd, int A[])
{
	if(iEnd - iBegin < 2)
		return;
	int iMiddle = (iEnd + iBegin) / 2;

	TopDownSplitMerge(A, iBegin, iMiddle, B);
	TopDownSplitMerge(A, iMiddle, iEnd, B);

	TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMerge(int A[], int iBegin, int iMiddle, int iEnd, int B[])
{
	int i = iBegin, j = iMiddle, k = 0;

	for(k = iBegin; k < iEnd; k++)
	{
		if(i < iMiddle && (j >= iEnd || A[i] <= A[j]))
		{
			B[k] = A[i];
			i++;
		}
		else
		{
			B[k] = A[j];
			j++;
		}
	}
}

void CopyArray(int A[], int iBegin, int iEnd, int B[])
{
	int k = 0;
	for(k = iBegin; k < iEnd; k++)
	{
		B[k] = A[k];
	}
}

vertex * findVertex(vertex **vertexArray, int value, int numVertices)
{
	int i = 0;

	for(i = 0; i < numVertices; i++)
	{
		vertex* curr = vertexArray[i];
		if(curr->value == value)
			return curr;
	}
	return NULL;
}

int findVertexIndex(vertex **vertexArray, int value, int numVertices)
{
	int i = 0;
	for(i = 0; i < numVertices; i++)
	{
		vertex* curr = vertexArray[i];
		if(curr->value == value)
			return i;
	}
	return 0;
}

void addEdge(vertex* v, edge* e)
{

	int tempIndex = sizeDA(v->edgeList);
	insertDA(v->edgeList, tempIndex, e);

}

edge ** sortEdges(QUEUE *breadthFirst)
{

	edge **unsortedEdgeArray = malloc(sizeof(edge*) * sizeQUEUE(breadthFirst));
	assert(unsortedEdgeArray != 0);
	int i = 0;
	int size = sizeQUEUE(breadthFirst);

	for(i = 0; i < size; i++)
	{
		unsortedEdgeArray[i] = dequeue(breadthFirst);

	}

	edge *buffer[size];


	TopDownMergeSortEdge2(unsortedEdgeArray, buffer, size);

	return unsortedEdgeArray;
}



void TopDownMergeSortEdge2(edge* A[], edge* B[], int n)
{

	CopyArrayEdge2(A, 0, n ,B);
	TopDownSplitMergeEdge2(B, 0, n, A);
}

void TopDownSplitMergeEdge2(edge* B[], int iBegin, int iEnd, edge* A[])
{
	if(iEnd - iBegin < 2)
		return;
	int iMiddle = (iEnd + iBegin) / 2;

	TopDownSplitMergeEdge2(A, iBegin, iMiddle, B);
	TopDownSplitMergeEdge2(A, iMiddle, iEnd, B);

	TopDownMergeEdge2(B, iBegin, iMiddle, iEnd, A);

}

void TopDownMergeEdge2(edge* A[], int iBegin, int iMiddle, int iEnd, edge* B[])
{
	int i = iBegin, j = iMiddle, k = 0;

	for(k = iBegin; k < iEnd; k++)
	{
		if(i < iMiddle && (j >= iEnd || (compareEDGE2(A[i], A[j]) <= 0)))
			{
				B[k] = A[i];
				i++;
			}
		else
		{

			B[k] = A[j];
			j++;
		}
	}
}

void CopyArrayEdge2(edge* A[], int iBegin, int iEnd, edge* B[])
{
	int k = 0;
	for(k = iBegin; k < iEnd; k++)
	{
		B[k] = A[k];
	}
}

int compareEDGE2(void *first, void* second)
{


	edge *x = first;
	edge *y = second;

	int ed1 = getINTEGER(x->weight);
	int ed2 = getINTEGER(y->weight);

	int ed3 = x->dest;
	int ed4 = y->dest;

	int ed5 = x->src;
	int ed6 = y->src;

	if(compareINTEGER(newINTEGER(ed1), newINTEGER(ed2)) == 0) //if equal weight
	{
		if(compareINTEGER(newINTEGER(ed5), newINTEGER(ed6)) == 0) //if equal source
		{
			return compareINTEGER(newINTEGER(ed3), newINTEGER(ed4));	//sort by vertex
		}
		return compareINTEGER(newINTEGER(ed5), newINTEGER(ed6)); //sort by source
	}

	return compareINTEGER(newINTEGER(ed1), newINTEGER(ed2));	//sort by weight

}


void TopDownMergeSortNew(void* A[], void* B[], int n)
{

	CopyArrayNew(A, 0, n ,B);
	TopDownSplitMergeNew(B, 0, n, A);
}

void TopDownSplitMergeNew(void* B[], int iBegin, int iEnd, void* A[])
{
	if(iEnd - iBegin < 2)
		return;
	int iMiddle = (iEnd + iBegin) / 2;

	TopDownSplitMergeNew(A, iBegin, iMiddle, B);
	TopDownSplitMergeNew(A, iMiddle, iEnd, B);

	TopDownMergeNew(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMergeNew(void* A[], int iBegin, int iMiddle, int iEnd, void* B[])
{
	int i = iBegin, j = iMiddle, k = 0;

	for(k = iBegin; k < iEnd; k++)
	{
		if(i < iMiddle && (j >= iEnd ||	(compareINTEGER(A[i], A[j]) <= 0)))
		{
			B[k] = A[i];
			i++;
		}
		else
		{
			B[k] = A[j];
			j++;
		}
	}
}

void CopyArrayNew(void* A[], int iBegin, int iEnd, void* B[])
{
	int k = 0;
	for(k = iBegin; k < iEnd; k++)
	{
		B[k] = A[k];
	}
}


void TopDownMergeSortEdgeNew(void* A[], void* B[], int n)
{

	CopyArrayEdgeNew(A, 0, n ,B);
	TopDownSplitMergeEdgeNew(B, 0, n, A);
}

void TopDownSplitMergeEdgeNew(void* B[], int iBegin, int iEnd, void* A[])
{
	if(iEnd - iBegin < 2)
		return;
	int iMiddle = (iEnd + iBegin) / 2;

	TopDownSplitMergeEdgeNew(A, iBegin, iMiddle, B);
	TopDownSplitMergeEdgeNew(A, iMiddle, iEnd, B);

	TopDownMergeEdgeNew(B, iBegin, iMiddle, iEnd, A);

}

void TopDownMergeEdgeNew(void* A[], int iBegin, int iMiddle, int iEnd, void* B[])
{
	int i = iBegin, j = iMiddle, k = 0;

	for(k = iBegin; k < iEnd; k++)
	{
		if(i < iMiddle && (j >= iEnd || (compareEDGE(A[i], A[j]) <= 0)))
			{
				B[k] = A[i];
				i++;
			}
		else
		{
			B[k] = A[j];
			j++;
		}
	}
}

void CopyArrayEdgeNew(void* A[], int iBegin, int iEnd, void* B[])
{
	int k = 0;
	for(k = iBegin; k < iEnd; k++)
	{
		B[k] = A[k];
	}
}


void displayVERTEXWRAPPER(void *v, FILE *fp)
{
	vertexWrapper *x = v;
	fprintf(fp, "%d", x->value);
}

int compareVERTEXWRAPPER(void *x, void *y)
{
	vertexWrapper *first = x;
	vertexWrapper *second = y;

	return compareINTEGER(newINTEGER(first->value), newINTEGER(second->value));
}

int findVertexIndex2(RBT *t, int v)
{
	vertexWrapper *findNode = malloc(sizeof(vertexWrapper));
	assert(findNode != 0);

	findNode->value = v;
	findNode->index = -1;

	int index = -1;

	findNode = findRBT(t, findNode);

	index = findNode->index;


	return index;
}
