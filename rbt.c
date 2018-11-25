

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "rbt.h"
// #include "string.h"

typedef enum { false = 0, true = !false } bool;

typedef struct rbtvalue RBTVALUE;

struct rbtvalue
{
	void *store;
	int frequency;
	char color;
	void (*display)(FILE *, void*);
	int (*comparator)(void *, void*);
};

struct rbt
{
	BST *bstTree;
	void (*display)(FILE *, void*);
	int (*comparator)(void *, void*);
	long numWords;
};

void insertionFixUp(RBT *tree, BSTNODE* x);
void deletionFixUp(RBT *tree, BSTNODE* x);
void leftRotation(RBT *tree, BSTNODE* x, BSTNODE* parent);
void rightRotation(RBT *tree, BSTNODE* x, BSTNODE* parent);
bool linear(BSTNODE* x, BSTNODE* parent, BSTNODE* grandparent);
bool isLeafRBT(BSTNODE *node);
RBTVALUE *newRBTVALUE(RBT* tree, void *v);


void
displayRBTFn(FILE *fp, void *value)
{
	RBTVALUE *n = value;
	n->display(fp, n->store);
	if(n->frequency > 1)
		fprintf(fp, "-%d", n->frequency);
	fprintf(fp, "-%c", n->color);
}

int
comparatorRBTFn(void* value1, void* value2) // these are RBT values?
{
	// printf("RBTComparator\n");

	RBTVALUE *n1 = value1;
	RBTVALUE *n2 = value2;

	// printf("here\n");
	return n1->comparator(n1->store, n2->store);
}
// function from Dr. Lusth
void
swapper2(BSTNODE *a,BSTNODE *b)
{
	// printf("in RBT swapper function\n");
	RBTVALUE *ra = getBSTNODE(a); // returns a void * which in this case is the RBTNODE object
	RBTVALUE *rb = getBSTNODE(b); // same thing
	// printf("here\n");

	// displaySTRING(stdout, ra->store);
	//displayRBTFn(stdout, ra);

	/* swap the keys stored in the RBT value objects */
	void *vtemp = ra->store;
	// printf("here2\n");
	ra->store = rb->store;
	rb->store = vtemp;
	// printf("stored values swapped\n");

	/* swap the counts stored in the RBT value objects */
	int ctemp = ra->frequency;
	ra->frequency = rb->frequency;
	rb->frequency = ctemp;

	/* note: colors are NOT swapped */
}

extern RBT *
newRBT(
			void (*display)(FILE *,void *),           //display
			int (*comparator)(void *,void *))           //comparator
{
	/* The constructor is passed two functions, one that knows how to display
	   the generic value to be stored and one that can compare two of these
	   generic values.
	 */
	RBT *newRBT;
	newRBT = (RBT *) malloc( sizeof(RBT) );
	assert(newRBT != 0);

	newRBT->display = display;
	newRBT->comparator = comparator;
	newRBT->numWords = 0;

	void (*swapper3)(BSTNODE*, BSTNODE*) = swapper2;
	void (*display3)(FILE*, void*) = displayRBTFn;
	int (*comparator3)(void*, void*) = comparatorRBTFn;
	newRBT->bstTree = newBST(display3, comparator3, swapper3);

	return newRBT;
}

	extern void
insertRBT(RBT *tree,void *value)
{
	/* Start out by using a regular binary search tree insertion. Color the newly
	   inserted node red. Call insertionFixUp, passing a pointer to the newly
	   inserted node. */
	// printf("in insertRBT\n");
	BSTNODE* insertedNode;
	RBTVALUE *newValue = newRBTVALUE(tree, value);
	// printf("new RBTValue created\n");
	insertedNode = findBST(tree->bstTree, newValue);
	// printf("search for existing node completed\n");

	if(insertedNode) // node exists, increase frequency
	{
		// printf("node exists\n");
		newValue = getBSTNODE(insertedNode);
		newValue->frequency++;
	}
	else // node doesn't exist, create it
	{
		// printf("node does not exist\n");
		//newValue = newRBTVALUE(tree, value);
		insertedNode = insertBST(tree->bstTree, newValue); // RBTVALUE passed as value to insertBST
		// printf("node has been inserted into BST\n");
		newValue->color = 'R';
		// printf("executing insertion fixup\n");
		insertionFixUp(tree, insertedNode);
		// printf("insertion fixup Complete\n");
	}

	tree->numWords++;
}

	extern int
findRBTCount(RBT *tree,void *value)
{
	/* This method returns the frequency of the searched-for value. If the value
	   is not in the tree, the method should return zero.
	 */
	RBTVALUE *searchValue = newRBTVALUE(tree, value); // different frequncies?
	BSTNODE *findValue = findBST(tree->bstTree, searchValue);
	int frequency = 0;

	if(findValue)
	{
		RBTVALUE* valueStored = getBSTNODE(findValue);
		frequency = valueStored->frequency;
	}
	return frequency;
}

	extern void
deleteRBT(RBT *tree,void *value)
{
	/* You should ignore, but report an attempt to delete something that does
	   not exist in the tree.
	 */
	// printf("delete RBT\n");
	BSTNODE* findNode;
	RBTVALUE* nodeValue = newRBTVALUE(tree, value);
	// printf("value created\n");
	findNode = findBST(tree->bstTree, nodeValue);
	// printf("node found\n");
	if(findNode)
	{
		// printf("node found\n");
		nodeValue = getBSTNODE(findNode);
		tree->numWords--;
		if(nodeValue->frequency == 1)
		{
			// printf("\ndeleting:");
			// displaySTRING(stdout, nodeValue->store);
			// printf("\n");
			// findNode = deleteBST(tree->bstTree, nodeValue);
			// deletionFixUp(tree, findNode);
			findNode = swapToLeafBST(tree->bstTree, findNode);
			deletionFixUp(tree, findNode);
			pruneLeafBST(tree->bstTree, findNode);
		}
		else
			nodeValue->frequency--;
	}
	else
	{
		//findNode = deleteBST(tree->bstTree, nodeValue);
		printf("Value ");
		tree->display(stdout, value);
		printf(" not found.\n");
	}
}

	extern int
sizeRBT(RBT *tree)
{
	/* This method returns the number of nodes currently in the tree. It should
	   run in amortized constant time.
	 */
	return sizeBST(tree->bstTree);
}

	extern int
wordsRBT(RBT *tree)
{
	/* This method returns the number of words (including duplicates) currently
	   in the tree. It should run in amortized constant time.
	 */
	return tree->numWords;
}

	extern void
statisticsRBT(FILE *fp, RBT* tree)
{
	/* This method should display the number of nodes in the tree as well as the
	   minimum and maximum heights of the tree. It should run in linear time.
	   1. the number of words/phrases in the tree
	   2. the number of nodes in the tree
	   3. the minimum depth, which is the distance from the root to the closest
	   node with a null child pointer
	   4. the maximum depth, which is the distance from the root to the furthest
	   node with a null child pointer
	   If the root had a null child, the minimum depth would be 1

	   This method should display the number of words/phrases (including duplicates)
	   and then call the BST display method.
	 */
	fprintf(fp, "Words/Phrases: %ld\n", tree->numWords);
	statisticsBST(fp, tree->bstTree);

}

	extern void
displayRBT(FILE *fp,RBT *tree)
{
	if(tree->numWords == 0)
	{
		fprintf(fp, "EMPTY\n");
		return;
	}

	displayBST(fp, tree->bstTree);

}

// Fixup routines
/* The insertion and deletion fixup routines for red-black trees must follow
   the pseudocode found on the Beastie website.
   There can be no whitespace other than a newline after the last
   printable character of each line in any output. No lines of output are indented.
 */
	void
insertionFixUp(RBT *tree, BSTNODE* x)
{
	// printf("in insertionFixUp\n");
	bool breakLoop = false;
	while(breakLoop == false)
	{
		// printf("finding parent\n");
		BSTNODE *parent = getBSTNODEparent(x);
		// printf("finding parent value\n");
		RBTVALUE *parentValue = getBSTNODE(parent);

		// printf("finding grandparent\n");
		BSTNODE *grandparent = getBSTNODEparent(parent);
		// printf("finding grandparent value\n");
		RBTVALUE *grandparentValue = getBSTNODE(grandparent);

		BSTNODE *uncle = NULL;

		// printf("finding uncle\n");
		if(parent == getBSTNODEleft(grandparent) ) uncle = getBSTNODEright(grandparent);
		else if (parent == getBSTNODEright(grandparent)) uncle = getBSTNODEleft(grandparent);

		// printf("finding uncle value\n");
		RBTVALUE *uncleValue = NULL;

		if(uncle)
			uncleValue = getBSTNODE(uncle);

		// printf("first line\n");
		if(x == getBSTroot(tree->bstTree) ) {breakLoop = true; break; }
		if(parentValue->color == 'B') {breakLoop = true; break; }

		if(uncle && uncleValue->color == 'R'){
			// printf("red uncle\n");
			if(uncleValue->color == 'R')
			{
			parentValue->color = 'B';
			uncleValue->color = 'B';
			grandparentValue->color = 'R';
			x = grandparent;
			}
		}
		else // uncle is black
		{
			// printf("black uncle\n");
			if(linear(x, parent, grandparent) == false)
			{
				// rotate x to parent
				if(x == getBSTNODEleft(parent) ) rightRotation(tree, x, parent);
				else leftRotation(tree, x, parent);
				BSTNODE *temp = x;
				x = parent;
				parent = temp;
			}
			parentValue = getBSTNODE(parent);
			grandparentValue = getBSTNODE(grandparent);

			parentValue->color = 'B';
			grandparentValue->color = 'R';
			// rotate parent to grandparent
			if(parent == getBSTNODEleft(grandparent) ) rightRotation(tree, parent, grandparent);
			else leftRotation(tree, parent, grandparent);
			breakLoop = true;
			break;
		}
	}

	BSTNODE* root = getBSTroot(tree->bstTree);
	RBTVALUE *rootValue = getBSTNODE(root);
	rootValue->color = 'B';
}

	void
deletionFixUp(RBT *tree, BSTNODE* x)
{
	// printf("in deletionFixUp\n");
	bool breakLoop = false;
	// printf("getting bstnode value \n");
	RBTVALUE *xValue = getBSTNODE(x);
	// int count = 0;
	while(breakLoop == false)
	{
		// if(count == 3) {xValue->color = 'B'; return;}

		// printf("displaying tree\n");
		// displayRBT(stdout, tree);
		// printf("finding parent : \n");
		BSTNODE* parent = getBSTNODEparent(x);
		// printf("finding parent value : \n");
		RBTVALUE *parentValue = getBSTNODE(parent);

		// printf("displaying x : ");
		// displayRBTFn(stdout, xValue);
		// printf("\n");

		// printf("parent of x : ");
		// displayRBTFn(stdout, parentValue);
		// printf("\n");
		/*
		   BSTNODE* grandparent = getBSTNODEparent(parent);
		   RBTVALUE *grandparentValue = getBSTNODE(grandparent);
		 */
		BSTNODE* sibling = NULL;
		RBTVALUE *siblingValue = NULL;
		if(x == getBSTNODEleft(parent) ) sibling = getBSTNODEright(parent);
		else if(x != getBSTNODEparent(x)) sibling = getBSTNODEleft(parent);
		if(sibling)
			siblingValue = getBSTNODE(sibling);


		BSTNODE *niece = NULL;
		BSTNODE *nephew = NULL;
		RBTVALUE *nieceValue = NULL;
		RBTVALUE *nephewValue = NULL;

		if(x == getBSTNODEleft(parent) )
		{
			// printf("left of parent\n");
			if(sibling)
			{

			niece = getBSTNODEleft(sibling);
			if (niece) nieceValue = getBSTNODE(niece);
			nephew = getBSTNODEright(sibling);
			if (nephew) nephewValue = getBSTNODE(nephew);
			}
		}
		else // x is to the right of the parent
		{
			// printf("right of parent\n");
			if(sibling)
			{
				// printf("sibling exists\n");
			niece = getBSTNODEright(sibling);
			if(niece) nieceValue = getBSTNODE(niece);
			nephew = getBSTNODEleft(sibling);
			if (nephew) nephewValue = getBSTNODE(nephew);
			}
		}

		// printf("here\n");
		// printf("first line\n");
		if(x == getBSTroot(tree->bstTree) ) {breakLoop = true; xValue->color = 'B'; return;} // exit the loop
		// printf("second line\n");
		if(xValue->color == 'R') {breakLoop = true; xValue->color = 'B'; return;} // exit the loop
		// printf("after second line\n");
		if(sibling && siblingValue->color == 'R')
		{
			// printf("first if\n");
			parentValue->color = 'R';
			siblingValue->color = 'B';
			if(sibling == getBSTNODEleft(parent) ) rightRotation(tree, sibling, parent);
			else leftRotation(tree, sibling, parent);
			// should have a black sibling now
		}
		else if (nephew && nephewValue->color == 'R') // nephew is red
		{
			// printf("second if\n");
			// color sibling the same as parent
			if(parentValue->color == 'B') siblingValue->color = 'B';
			else siblingValue->color = 'R';

			parentValue->color = 'B'; // color parent black

			if(nephew)
				nephewValue->color = 'B'; // color nephew black
			// rotate sibling to parent
			if(sibling == getBSTNODEleft(parent)) rightRotation(tree, sibling, parent);
			else leftRotation(tree, sibling, parent);
			// subtree and tree is BH balanced
			breakLoop = true;
			break; // exit the loop?
		}
		else if (niece && nieceValue->color == 'R') // niece is red
		{
			// printf("third if\n");
			// nephew must be black
			nieceValue->color = 'B'; // color niece black
			siblingValue->color = 'R'; // color sibling red
			// rotate niece to sibling
			if(niece == getBSTNODEleft(sibling)) rightRotation(tree, niece, sibling);
			else leftRotation(tree, niece, sibling);
			// should have red nephew now
		}
		else // sibling, niece, and nephew must be black
		{
			// printf("else\n");
			if(sibling)
			{
				// printf("sibling exists\n");
				siblingValue->color = 'R'; // color sibling red
			}
			x = parent; // x = parent
			xValue = parentValue;
			// this subtree is BH balanced, but tree is not
		}
		// count++;
	}
	xValue->color = 'B'; // color x black
}

	void
leftRotation(RBT *tree, BSTNODE* node, BSTNODE* parent)
{
	// printf("in leftRotation\n");
	BSTNODE *x 		= parent;
	BSTNODE *y      = node;

	y = getBSTNODEright(x); // set y
	setBSTNODEright(x, getBSTNODEleft(y) ); // turn y's left subtree into x's right subtree
	if( getBSTNODEleft(y) != NULL) // y.left != T.nil
		setBSTNODEparent( getBSTNODEleft(y),x); // y.left.p = x
	setBSTNODEparent( y, getBSTNODEparent(x) ); //y.p = x.p

	if (getBSTNODEparent(x) == x) //x.p == T.nil
	{
		setBSTroot(tree->bstTree, y); // T.root = y
		setBSTNODEparent(y, y);
	}
	else if (x == getBSTNODEleft( getBSTNODEparent(x) ) )// x == x.p.left
		setBSTNODEleft( getBSTNODEparent(x) , y);// x.p.left = y
	else
		setBSTNODEright( getBSTNODEparent(x), y); // x.p.right = y
	setBSTNODEleft(y, x); // y.left = x
	setBSTNODEparent(x, y); // x.p = y
}

	void
rightRotation(RBT *tree, BSTNODE* x, BSTNODE* parent)
{
	// printf("in rightRotation\n");
	BSTNODE *y  = parent;


	x = getBSTNODEleft(y); // x = y.left
	setBSTNODEleft(y, getBSTNODEright(x) ); //y.left = x.right
	if( getBSTNODEright(x) != NULL)  // if(x.right != NULL)
		setBSTNODEparent( getBSTNODEright(x) , y); // x.right.parent = y
	setBSTNODEparent(x, getBSTNODEparent(y) ); // x.p = y.p
	if (getBSTNODEparent(y) == y) //y.p == T.nil (or itself for the root)
	{
		setBSTroot(tree->bstTree, x); // T.root = x
		setBSTNODEparent(x, x);
	}
	else if (y == getBSTNODEleft(getBSTNODEparent(y))) // y == y.p.left
		setBSTNODEleft(getBSTNODEparent(y), x); // y.p.left = x
	else // y == y.p.right
		setBSTNODEright(getBSTNODEparent(y), x); // y.p.right = x
	setBSTNODEright(x, y); // x.right = y
	setBSTNODEparent(y, x);// y.parent = x
}

	bool
linear(BSTNODE* x, BSTNODE* parent, BSTNODE* grandparent)
{
	if(parent == getBSTNODEleft(grandparent) )
	{
		if(x == getBSTNODEleft(parent) )
			return true;
		else
			return false;
	}
	else // parent == right of grandparent
	{
		if(x == getBSTNODEright(parent) )
			return true;
		else
			return false;
	}
}

RBTVALUE *
newRBTVALUE(RBT* tree, void *v)
{
	RBTVALUE *newVALUE;
	newVALUE = (RBTVALUE *) malloc( sizeof(RBTVALUE) );
	assert(newVALUE != 0);

	newVALUE->store = v;
	newVALUE->frequency = 1;
	newVALUE->color = 'R';
	newVALUE->display = tree->display;
	newVALUE->comparator = tree->comparator;


	return newVALUE;
}

bool isLeafRBT(BSTNODE *node)
{
	if(getBSTNODEleft(node))
		return false;
	else if (getBSTNODEright(node))
		return false;
	else
		return true;
}

void *
findRBT(RBT* t, void* v)
{
	// v is the structure we are looking for
	// returns the value stored within that structure (in this case the index)

	// FIXME
	BSTNODE* foundNode;
	RBTVALUE *newValue = newRBTVALUE(t, v);
	foundNode = findBST(t->bstTree, newValue);

	if(foundNode) // node exists, increase frequency
	{
		// printf("node exists\n");
		newValue = getBSTNODE(foundNode);
		return newValue->store;
	}
	else // node doesn't exist, create it
	{
		return NULL;
	}

	return v;
}
