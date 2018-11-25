/* Dynamic Array - Connor Gill */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "da.h"

static void shrinkArray(DA *usrDA);
static void growArray(DA *usrDA);


struct da
{
  void * (*array);  //struct will contain Dyn array of type void
  int size;
  int capacity;
  int debugVal;
  void (*freeP)(void * ptr); //pointer for use in freeDA function
  void (*displayP)(void * ptr, FILE *fp); //pointer for use in displayDA function
};

extern DA   *newDA(void)
{
  DA * arrStruct = malloc(sizeof(DA));  //allocates struct
  assert(arrStruct != 0);   //ensure struct does not = 0
  arrStruct->capacity = 1;  //empty capacity to 1
  arrStruct->array = malloc(sizeof(void *) * arrStruct->capacity); //allocates array
  assert(arrStruct->array != 0);  //ensures array does not = 0
  arrStruct->size = 0; //empty size
  arrStruct->debugVal = 0; //empty val
  arrStruct->freeP = 0; //empty ptr
  arrStruct->displayP = 0;  //empty ptr

  return arrStruct;
}

/*This method is passed a function that knows how to display the generic
value stored in an array slot.*/
extern void  setDAdisplay(DA *usrDA, void (*disPtr)(void *ptr, FILE *fp))
{
  usrDA->displayP = disPtr;
}

/*This method is passed a function that knows how to free the generic
value stored in an array slot.*/
extern void  setDAfree(DA *usrDA, void (*freePtr)(void *ptr))
{
  usrDA->freeP = freePtr;
}

/*The insert method places the given item at the slot named by given index.
The previous item at that slot shifts to the next higher slot (and so on).
If there is no room for the insertion, the array grows by doubling. It should
run in amortized constant time when inserting an item a constant distance
from the back and in linear time otherwise.*/
extern void  insertDA(DA *usrDA, int index, void *val)
{
  assert(index >= 0 && index <= sizeDA(usrDA));   //ensure index is within arr
  if (usrDA->capacity == sizeDA(usrDA)) //if full, double capacity
  {
    growArray(usrDA);
  }

  if (index == sizeDA(usrDA)) //if index == size, insert @& inc size
  {
    usrDA->array[index] = val;
    usrDA->size += 1;
  }
  else //if index already holds a value
  {
    for (int i = sizeDA(usrDA); i >= index + 1; i--)  //shifts values
    {
      usrDA->array[i] = usrDA->array[i - 1];
    }
    usrDA->array[index] = val;
    usrDA->size += 1;
   }

}

/*The remove method removes the item at the given index. The item at the next
higher slot shifts to that slot (and so on). The method returns the removed
item. If the ratio of the size to the capacity drops below 25%, the array
shrinks by half. The array should never shrink below a capacity of one. Also,
an empty array should always have a capacity of one. It should run in amortized
constant time when removing an item a constant distance from the back and
in linear time otherwise.*/
extern void *removeDA(DA *usrDA, int index)
{
  assert(sizeDA(usrDA) > 0);  //ensure array is not of size 0

  void * remVal = getDA(usrDA, index); //set to val @ index
  for (int i = index; i < sizeDA(usrDA) - 1; i++) //shifts values
  {
    usrDA->array[i] = usrDA->array[i + 1];
  }
  usrDA->size -= 1;

  if (sizeDA(usrDA)/((double)usrDA->capacity) < .25)
  {
    shrinkArray(usrDA);

    if (sizeDA(usrDA)/((double)usrDA->capacity) < .25)
    {
      shrinkArray(usrDA);
    }
  }

  return remVal;
}

/*The union method takes two
array and moves all the arr in the donor array to the recipient array.
If the recipient array has the arr [3,4,5] and the donor array has the
arr [1,2], then, after the union, the donor array will be empty and recipient
array will have the arr [3,4,5,1,2]. It should run in amortized linear time
with respect to the size of the donor array.*/
extern void  unionDA(DA *recipient, DA *donor)   //FIX THIS
{
  for (int i = 0; i < donor->size; i++) //insert donor into rec 1x1
  {
    insertDA(recipient, recipient->size, donor->array[i]);
   }
   free(donor->array);
   donor->size = 0;
   donor->capacity = 1;
   donor->array = malloc(sizeof(void *) * donor->capacity);
  }

/*The get method returns the value at the given index.
It should run in constant time.*/
extern void *getDA(DA *usrDA, int index)
{
  assert(index >= 0 && index < sizeDA(usrDA)); //ensure index is within range
  return usrDA->array[index];
}

/*If the given index is equal to the size, the value is inserted via the insert
method. The method returns the replaced value or the null pointer if no value
was replaced. It should run in constant or amortized constant time (in the
case of the insert method being called).*/
extern void *setDA(DA *usrDA,int index,void *val)
{
  assert(index >= 0 && index <= sizeDA(usrDA));   //ensure index is within arr


  if (index == sizeDA(usrDA))   //if index = size, insert new, return null ptr
  {
    insertDA(usrDA, index, val);
    return 0; //returns null pointer
  }
  else   //if no value gets replaced
  {
    void (*temp) = getDA(usrDA, index);   //gets val at index
    usrDA->array[index] = val;
    return temp;
  }

}

/*The size method returns the number of arr stored in the array.
It should run in amortized constant time.*/
extern int   sizeDA(DA *usrDA)
{
  return usrDA->size;
}

/*This visualizing method prints out the filled region, enclosed in brackets
and separated by commas. If the integers 5, 6, 2, 9, and 1 are stored in the
array (listed from index 0 upwards) and the debug level is zero (see debugDA),
the method would generate this output: [5,6,2,9,1] with no preceding or
following whitespace. If no display method is set (see setDAdisplay), the
address of each item is printed (using %p). Each address is preceded by an @ sign. */

extern void  displayDA(DA *usrDA,FILE *fp)
{
  if ((sizeDA(usrDA) == 0) && (usrDA->debugVal == 0)) // if empty & zero debug
  {
      fprintf(fp, "[]");
  }

  else if ((sizeDA(usrDA) == 0) && (usrDA->debugVal > 0)) // if empty & non zero debug
  {
    fprintf(fp, "[[%d]]", usrDA->capacity);
  }

  else if ((usrDA->displayP == 0) && (usrDA->debugVal == 0)) // if no display, print addy
  {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(usrDA); i++)
    {
      fprintf(fp, "@%p,", &usrDA->array[i]);
    }
    fprintf(fp, "]");
  }

  else if ((usrDA->displayP == 0) && (usrDA->debugVal > 0)) // if no display, print addy
  {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(usrDA); i++)
    {
      fprintf(fp, "@%p,", &usrDA->array[i]); // print addy
    }
      fprintf(fp, "[%d]]", (usrDA->capacity - sizeDA(usrDA)));
  }

  else if ((usrDA->displayP != 0) && (usrDA->debugVal == 0)) // display  pointer takes in values
  {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(usrDA); i++)
    {
      usrDA->displayP(usrDA->array[i], fp);
      if (i != (sizeDA(usrDA) - 1))
      {
        fprintf(fp, ",");
      }
    }
    fprintf(fp, "]");
  }

  else if ((usrDA->displayP != 0) && (usrDA->debugVal > 0)) // if display, print addy
  {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(usrDA); i++)
    {
      usrDA->displayP(usrDA->array[i], fp);   //display pointer takes in values
      if (i != (sizeDA(usrDA) - 1))
      {
        fprintf(fp, ",");
      }
    }
    fprintf(fp, ",[%d]]", (usrDA->capacity - sizeDA(usrDA)));
  }
}

/*The debug method sets an internal flag in the object to the given value.
If the flag is greater than zero, the display method adds a comma
(if needed) and the number of empty slots (in brackets) immediately after the
last element. If the above array had a capacity of 8, the display would be
[5,6,2,9,1,[3]]. An empty array with capacity 1 displays as [[1]].
The method returns the previous debug value.*/
extern int   debugDA(DA *usrDA, int newBug)
{
  int prevBug = usrDA->debugVal;
  usrDA->debugVal = newBug;

  return prevBug;
}

/*If no free method is set, the individual arr are not freed. In any case,
the array and its supporting allocations are freed.*/
extern void  freeDA(DA *usrDA)
{
  if (usrDA->freeP != 0)  //if free pointer is set
  {
    for (int i = 0; i < sizeDA(usrDA); i++)
    {
      usrDA->freeP(usrDA->array[i]); //free index x index
    }
  }
  free(usrDA->array);   //free entire array
  free(usrDA);    //free struct
}

//half array if <25% is full
static void shrinkArray(DA *usrDA)
{
  usrDA->capacity = (usrDA->capacity) / 2;  //capacity is halved
  assert(usrDA->capacity >= 1);   //capacity must be atleast 1
  usrDA->array = realloc(usrDA->array, sizeof(void *) * usrDA->capacity); //resize
  assert(usrDA->array != 0);   //array can't be empty
}

//double array if full
static void growArray(DA *usrDA)
{
  usrDA->capacity = (usrDA->capacity) * 2;  //capacity is doubled
  usrDA->array = realloc(usrDA->array, sizeof(void *) * usrDA->capacity); //resize
  assert(usrDA->array != 0);  //array can't be empty
}
