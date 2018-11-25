#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <stdio.h>

typedef struct STRING STRING;

extern STRING *newSTRING(char* r); //extern is optional - used when a variable is declared in a header file (Lusth thinks variables and functions are the same thing)
extern char* getSTRING(STRING *r);
extern void setSTRING(STRING *r, char* n); //by convention, the object is going to be our first parameter
extern void displaySTRING(FILE *fp, void *p); 
extern int compareSTRING(void *v, void *w);

#endif