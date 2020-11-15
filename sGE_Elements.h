#pragma once

/********************************************************/
/*	Elements.h											*/
/*														*/
/*	Key Information:									*/
/*														*/
/*														*/
/* 														*/
/* 														*/
/* 														*/
/* 	by s0lly											*/
/* 														*/
/********************************************************/



/********************************************************/
/*	Includes											*/
/********************************************************/

#include <assert.h>
#include <intrin.h>
#include <string>


/********************************************************/
/*	Defines												*/
/********************************************************/

#define ArrayCount(arr) (sizeof(arr) / sizeof((arr)[0]))
#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)
#define Terabytes(value) (Gigabytes(value) * 1024LL)

#define ELEMENTS_MEMORY_MAX_SIZE Megabytes(100)

#define maxComponents 64000

/********************************************************/
/*	Enums												*/
/********************************************************/





/********************************************************/
/*	Structs												*/
/********************************************************/

typedef struct ElementManager* Elements;


struct ElementsMemoryManager
{
	size_t maxSize = ELEMENTS_MEMORY_MAX_SIZE;
	size_t currentSize;
	char memory[ELEMENTS_MEMORY_MAX_SIZE];
};

/********************************************************/
/*	Functions											*/
/********************************************************/


void* Elements_GetPointerFromID(Elements elementManager, int id);

void* Elements_GetPointerFromIndex(Elements elementManager, int index);

int Elements_GetIDFromIndex(Elements elementManager, int index);

int Elements_GetIDFromPointer(Elements elementManager, void *pointer);

void Elements_SetAsRemoved(Elements elementManager, int id);

void Elements_SetAsRemovedGlobal(Elements* elements, int numElementsArray, int id);

void Elements_RemoveAndSort(Elements elementManager, int maxSwaps);

void* Elements_Insert(Elements elementManager, int id, void* data); // CAREFUL: might invalidate existing pointers

int Elements_GetCount(Elements elementManager);

Elements Elements_Create(ElementsMemoryManager* elementsMemory, int sizeOfEachElement);