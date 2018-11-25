#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "bst.h"
#include "queue.h"


typedef enum { false = 0, true = !false } bool;

BSTNODE *addNode(BST *, BSTNODE*, void*);
BSTNODE *searchBST(BST*, BSTNODE*, void*);
BSTNODE *newNode(BST*, BSTNODE*, void*);
void displayNode(void *, FILE*);
void printNode(FILE*, BST*, BSTNODE*);
bool isLeaf(BSTNODE *node);
int findMaxDepth(BSTNODE *);
int findMinDepth(BSTNODE *);

struct bstnode
{
	void *store;
	BSTNODE *parent;
	BSTNODE *left;
	BSTNODE *right;
	void (*display)(FILE *, void*);
};

	extern void *
getBSTNODE(BSTNODE *node)
{

		return node->store;

}

	extern void
setBSTNODE(BSTNODE *node,void *value)
{
	node->store = value;
}

	extern BSTNODE *
getBSTNODEleft(BSTNODE *node)
{

		return node->left;

}

	extern void
setBSTNODEleft(BSTNODE *node,BSTNODE *newLeft)
{
	node->left = newLeft;
	if(newLeft)
		newLeft->parent = node;
}

	extern BSTNODE *
getBSTNODEright(BSTNODE *node)
{

		return node->right;

}

	extern void
setBSTNODEright(BSTNODE *node,BSTNODE *newRight)
{
	node->right = newRight;
	if(newRight)
		newRight->parent = node;
}

	extern BSTNODE *
getBSTNODEparent(BSTNODE *node)
{

		return node->parent;

}

	extern void
setBSTNODEparent(BSTNODE *node,BSTNODE *newParent)
{
	node->parent = newParent;
}

struct bst
{
	void (*display)(FILE *, void*);
	int (*comparator)(void *, void*);
	void (*swapper)(BSTNODE *, BSTNODE*);
	BSTNODE *root;
	long size;

};

	extern BST *
	newBST(
			void (*display)(FILE *,void *),
			int (*comparator)(void *,void *),
			void(*swapper)(BSTNODE *, BSTNODE*))
{

	BST *newBST;
	newBST = (BST *) malloc( sizeof(BST) );
	assert(newBST != 0);

	newBST->display = display;
	newBST->comparator = comparator;
	newBST->swapper = swapper;

	newBST->root = NULL;
	newBST->size = 0;

	return newBST;

}

	extern void
setBSTroot(BST *tree,BSTNODE *replacement)
{

	tree->root = replacement;
}

	extern BSTNODE *
getBSTroot(BST *tree)
{
	return tree->root;
}

	extern BSTNODE*
insertBST(BST *tree,void *value)
{

	tree->size = tree->size + 1;

	return addNode(tree, tree->root, value);
}

	extern BSTNODE *
findBST(BST *tree,void *value)
{

	BSTNODE *foundValue = NULL;
	if(tree->root)
	{

		foundValue = searchBST(tree, tree->root, value);

	}


	return foundValue;

}

	extern BSTNODE *
deleteBST(BST *tree,void *value)
{

	BSTNODE *deleteNode = findBST(tree, value);

	BSTNODE* pred = deleteNode->left;
	BSTNODE* succ = deleteNode->right;


	if(pred) {while(pred->right) pred = pred->right;}
	if(succ) {while(succ->left) succ = succ->left;}
	if (deleteNode)
	{
		deleteNode = swapToLeafBST(tree, deleteNode);
		pruneLeafBST(tree, deleteNode);

	}
	else
	{

		fprintf(stdout, "Value ");
		tree->display(stdout, value);
		fprintf(stdout, " not found.\n");
	}
	return deleteNode;
}

	extern BSTNODE *
swapToLeafBST(BST *tree,BSTNODE *n)
{


	BSTNODE* replacement;


	if(n->left)
	{
		while(!isLeaf(n))
		{
			replacement = n->left;
			while(replacement->right)
			{
				replacement = replacement->right;
			}

			if(tree->swapper == NULL)
			{

				void *temp = n->store;
				n->store = replacement->store;
				replacement->store = temp;
				n = replacement;
			}
			else
			{
				tree->swapper(replacement,n);
				n = replacement;
			}
		}

	}
	else if (n->right)
	{
		while (!isLeaf(n))
		{
			replacement = n->right;
			while(replacement->left)
			{
				replacement = replacement->left;
			}

			if(tree->swapper == NULL)
			{

				void *temp = n->store;
				n->store = replacement->store;
				replacement->store = temp;
				n = replacement;
			}
			else
			{
				tree->swapper(replacement,n);
				n = replacement;
			}
		}
	}

	return n;

}

	extern void
pruneLeafBST(BST *tree,BSTNODE *leaf)
{

	if(getBSTNODEparent(leaf) == leaf)
	{

		setBSTroot(tree, NULL);

	}
	else if(getBSTNODEleft(leaf->parent) == leaf)
	{

		setBSTNODEleft(leaf->parent, NULL);
	}
	else
	{

		setBSTNODEright(leaf->parent, NULL);
	}


	tree->size--;

}

	extern int
sizeBST(BST *tree)
{

	return tree->size;
}

	extern void
statisticsBST(FILE *fp, BST* tree)
{

	int minDepth = -1;
	int maxDepth = -1;
	int numWords = tree->size;

	minDepth = findMinDepth(tree->root);
	maxDepth = findMaxDepth(tree->root);

	if(minDepth == -1) minDepth = 0;
	if (maxDepth == -1) maxDepth = 0;
	fprintf(fp, "Nodes: %d\n", numWords);
	fprintf(fp, "Minimum depth: %d\n", minDepth);
	fprintf(fp, "Maximum depth: %d\n", maxDepth);

}



	extern void
displayBST(FILE *fp,BST *tree)
{

	if(tree->size == 0)
	{
		fprintf(fp, "EMPTY\n");
		return;
	}


	QUEUE *displayQUEUE = newQUEUE();
	setQUEUEdisplay(displayQUEUE, displayNode);
	if(tree->size != 0)
		enqueue(displayQUEUE, tree->root);
	int level = 0;

	while(sizeQUEUE(displayQUEUE) != 0)
	{

		int size = sizeQUEUE(displayQUEUE);
		fprintf(fp, "%d: ", level);
		while(size > 0)
		{

			BSTNODE *node = (BSTNODE*)malloc(sizeof(BSTNODE));
			assert(node != 0);
			node = dequeue(displayQUEUE);
			assert(node != NULL);
			if(node->left)
			{

				enqueue(displayQUEUE, node->left);


			}

			if(node->right)
			{

				enqueue(displayQUEUE, node->right);


			}

			printNode(fp, tree, node);



			if((size-1) > 0)
				fprintf(fp," ");
			size--;
		}
		level++;
		fprintf(fp, "\n");
	}

}



	BSTNODE *
addNode(BST *tree, BSTNODE * n, void *value)
{

	if(tree->root == NULL)
	{

		n = newNode(tree, NULL, value);
		tree->root = n;

		return n;
	}

	else
	{
		int compareValue = tree->comparator(n->store, value);

		if(compareValue > 0)
		{
			if (n->left)
			{
				return addNode(tree, n->left, value);

			}
			else
			{
				n->left = newNode(tree, n, value);
				return n->left;
			}
		}
		else
		{
			if (n->right)
			{
				return addNode(tree, n->right, value);

			}
			else
			{
				n->right = newNode(tree, n, value);
				return n->right;
			}
		}
	}
}

	BSTNODE *
searchBST(BST *tree, BSTNODE *n, void *value)
{
	int compareValue = tree->comparator(n->store, value);
	if (compareValue == 0) { return n;}
	else if (compareValue > 0)
	{
		if (n->left)
			return searchBST(tree, n->left, value);
		else
			return NULL;
	}
	else
	{
		if(n->right)
			return searchBST(tree, n->right, value);
		else
			return NULL;
	}

}

	BSTNODE *
newNode(BST *tree, BSTNODE *p, void *v)
{
	BSTNODE *newNode;
	newNode = (BSTNODE *) malloc( sizeof(BSTNODE) );
	assert(newNode != 0);

	if(p == NULL)
		newNode->parent = newNode;
	else
		newNode->parent = p;

	newNode->store = v;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->display = tree->display;

	return newNode;
}

	void
displayNode(void *node2, FILE* fp)
{
	BSTNODE* node = node2;
	node->display(fp, node->store);
}

	void
printNode(FILE *fp,BST* tree, BSTNODE *node)
{

	if(isLeaf(node) )
		fprintf(fp,"=");

	displayNode(node, fp);
	fprintf(fp,"(");
	displayNode(node->parent, fp);
	fprintf(fp,")");

	if(node == tree->root)
		fprintf(fp,"-");

	if(node == getBSTNODEleft(node->parent) )
		fprintf(fp,"-l");

	if(node == getBSTNODEright(node->parent) )
		fprintf(fp,"-r");
}

bool isLeaf(BSTNODE *node)
{
	if(node->left)
		return false;
	else if (node->right)
		return false;
	else
		return true;
}

int
findMaxDepth(BSTNODE* n)
{
	int leftSide = 0;
	int rightSide = 0;
	if(n == NULL) return 0;
	leftSide = 1+findMaxDepth(n->left);
	rightSide = 1+findMaxDepth(n->right);
	if(leftSide > rightSide) return leftSide;
	else return rightSide;
}

int
findMinDepth(BSTNODE *n)
{
	int leftSide = 0;
	int rightSide = 0;
	if(n == NULL) return 0;
	leftSide = 1 + findMinDepth(n->left);
	rightSide = 1 + findMinDepth(n->right);
	if(leftSide < rightSide) return leftSide;
	else return rightSide;
}
