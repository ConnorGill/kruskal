#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "string.h"

struct STRING
{
	char* store;
};

/********************* public interface *****************************/

STRING *
newSTRING(char* v)
	{

		//printf("%s : size = %d , size2 = %ld", v, (int)strlen(v) , (sizeof(v) / sizeof(char) ) );
		STRING *p = malloc(sizeof(STRING));
		assert(p != 0); //if assertion is tripped - program ends immediately - displaying to output
		//unsigned size = sizeof(v) / sizeof(char);
		unsigned size = (int)strlen(v);
		p->store = (char*)malloc(sizeof(char) * (size));
		assert(p->store != 0);

		// initialize every field in our object after allocation
		unsigned i = 0;
		for(i = 0; i <= size; i++)
		{
			p->store[i] = v[i];
		}

		
		return p;
	}

char *
getSTRING(STRING *r)
	{
		return r->store;
	}

void
setSTRING(STRING *r, char* v)
	{
		r->store = v;
	}

// this function or newString are causing errors
void 
displaySTRING(FILE* fp, void *p)
	{
		STRING *r = p; // first thing to do - store pointer back in STRING object
		fprintf(fp, "%s", r->store);
	}

int 
compareSTRING(void *v, void* w)
{
	//returns a negative number if the first thing is considered less than the second thing
	return strcmp(getSTRING(v), getSTRING(w));
}