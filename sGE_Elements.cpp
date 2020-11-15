/********************************************************/
/*	Elements.cpp										*/
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

#include "sGE_Elements.h"




/********************************************************/
/*	Enums												*/
/********************************************************/





/********************************************************/
/*	Structs												*/
/********************************************************/

struct ElementInfo
{
	int id;
	bool isRemovable;
};



struct ElementManager
{
	int count;
	size_t elementSize;
	ElementInfo infos[maxComponents];
	void* data;
};



/********************************************************/
/*	Functions											*/
/********************************************************/


void* Elements_GetPointerFromID(Elements elementManager, int id)
{

	if (elementManager->count == 0)
	{
		return nullptr;
	}

	ElementInfo* start = nullptr;
	ElementInfo* mid = nullptr;
	ElementInfo* end = nullptr;

	int startIndex = 0;
	int endIndex = (int)elementManager->count - 1;
	int midIndex = (startIndex + endIndex) / 2;

	while (true)
	{
		if (startIndex == endIndex)
		{
			start = &elementManager->infos[startIndex];
			if (start->id == id)
			{
				if (!start->isRemovable)
				{
					return (char*)elementManager->data + (start - (ElementInfo*)(elementManager->infos)) * elementManager->elementSize;
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}

		start = &elementManager->infos[startIndex];
		if (start->id == id)
		{
			if (!start->isRemovable)
			{
				return (char*)elementManager->data + (start - (ElementInfo*)(elementManager->infos)) * elementManager->elementSize;
			}
			else
			{
				return nullptr;
			}
		}

		mid = &elementManager->infos[midIndex];
		if (mid->id == id)
		{
			while (mid != start)
			{
				if (mid->id == id)
				{
					if (!mid->isRemovable)
					{
						return (char*)elementManager->data + (mid - (ElementInfo*)(elementManager->infos)) * elementManager->elementSize;
					}
					else
					{
						mid -= 1;
					}
				}
				else
				{
					return nullptr;
				}
			}
			if (mid == start)
			{
				return nullptr;
			}
		}

		end = &elementManager->infos[endIndex];
		if (end->id == id)
		{
			while (end != mid)
			{
				if (end->id == id)
				{
					if (!end->isRemovable)
					{
						return (char*)elementManager->data + (end - (ElementInfo*)(elementManager->infos)) * elementManager->elementSize;
					}
					else
					{
						end -= 1;
					}
				}
				else
				{
					return nullptr;
				}
			}
			if (end == mid)
			{
				return nullptr;
			}
		}

		if (id < start->id || id > end->id)
		{
			return nullptr;
		}

		if (id < mid->id)
		{
			endIndex = midIndex - 1;
			midIndex = (startIndex + endIndex) / 2;
		}
		else
		{
			startIndex = midIndex + 1;
			midIndex = (startIndex + endIndex) / 2;
		}

	}
}

void* Elements_GetPointerFromIndex(Elements elementManager, int index)
{

	if (index >= elementManager->count)
	{
		return nullptr;
	}

	if (elementManager->infos[index].isRemovable)
	{
		return nullptr;
	}

	return (char*)elementManager->data + index * elementManager->elementSize;
}

int Elements_GetIDFromIndex(Elements elementManager, int index)
{

	if (index >= elementManager->count)
	{
		return -1;
	}

	if (elementManager->infos[index].isRemovable)
	{
		return -1;
	}

	return elementManager->infos[index].id;
}


int Elements_GetIDFromPointer(Elements elementManager, void *pointer)
{

	if (pointer == nullptr)
	{
		return -1;
	}

	int index = (int)((char*)pointer - (char*)elementManager->data) / (int)elementManager->elementSize;
	
	return elementManager->infos[index].id;
}

int Elements_GetNextInsertionIndex(Elements elementManager, int id)
{
	if (elementManager->count == 0)
	{
		return 0;
	}

	int insertionIndex = elementManager->count;

	for (int i = 0; i < elementManager->count; i++)
	{
		if (elementManager->infos[i].id < id && elementManager->infos[i].isRemovable)
		{
			insertionIndex = i;
		}
		if (elementManager->infos[i].id == id)
		{
			insertionIndex = i;
			break;
		}
		if (elementManager->infos[i].id > id)
		{
			insertionIndex = i;
			break;
		}
	}

	return insertionIndex;
}

void Elements_SetAsRemoved(Elements elementManager, int id)
{
	if (elementManager->count == 0)
	{
		return;
	}

	void* selectedElement = Elements_GetPointerFromID(elementManager, id);

	if (selectedElement == nullptr)
	{
		return;
	}

	elementManager->infos[(((char*)selectedElement - (char*)elementManager->data) / elementManager->elementSize)].isRemovable = true;
}

void Elements_SetAsRemovedGlobal(Elements* elements, int numElementsArray, int id)
{
	for (int i = 0; i < numElementsArray; i++)
	{
		Elements_SetAsRemoved(elements[i], id);
	}
}


void Elements_RemoveAndSort(Elements elementManager, int maxSwaps)
{

	if (elementManager->count == 0 || maxSwaps < 1)
	{
		return;
	}

	int numSwaps = 0;
	int numNextRemovables = 0;

	void* pointerToElement = elementManager->data;
	for (int i = 0; i < elementManager->count; i++)
	{
		if (elementManager->infos[i].isRemovable)
		{
			void* pointerToNextElement = (char*)pointerToElement + elementManager->elementSize * (1 + (size_t)numNextRemovables);
			bool madeSwap = false;

			for (int j = i + 1 + numNextRemovables; j < elementManager->count; j++)
			{
				if (!elementManager->infos[j].isRemovable)
				{
					memcpy(pointerToElement, pointerToNextElement, elementManager->elementSize);
					elementManager->infos[j].isRemovable = true;

					//numNextRemovables = j - i - 1;

					madeSwap = true;
					numSwaps++;

					int setId = elementManager->infos[j].id;
					for (int k = 0; k < numNextRemovables; k++)
					{
						elementManager->infos[j - k - 1].id = setId;
					}

					elementManager->infos[i].isRemovable = false;
					elementManager->infos[i].id = setId;

					j = (int)elementManager->count;
				}
				else
				{
					numNextRemovables++;
				}

				pointerToNextElement = (char*)pointerToNextElement + elementManager->elementSize;
			}

			if (!madeSwap)
			{
				elementManager->count = i;
				return;
			}
			else
			{
				if (numSwaps == maxSwaps)
				{
					return;
				}
			}
		}

		pointerToElement = (char*)pointerToElement + elementManager->elementSize;
	}

}


// STUPID - Need to allow for case where ID already exists - return nullptr

void* Elements_Insert(Elements elementManager, int id, void* data)
{

	if (elementManager->count < maxComponents && Elements_GetPointerFromID(elementManager, id) == nullptr)
	{
		int insertionIndex = elementManager->count;

		if (!(elementManager->count == 0 || elementManager->infos[elementManager->count - 1].id < id))
		{
			insertionIndex = Elements_GetNextInsertionIndex(elementManager, id);

			if (!elementManager->infos[insertionIndex].isRemovable)
			{
				int elementsToPushBack = elementManager->count - insertionIndex;

				void* tempData = malloc(((size_t)elementManager->count - (size_t)insertionIndex) * elementManager->elementSize);
				memcpy(tempData, (char*)elementManager->data + insertionIndex * elementManager->elementSize, ((size_t)elementManager->count - (size_t)insertionIndex) * elementManager->elementSize);
				memcpy((char*)elementManager->data + ((size_t)insertionIndex + 1) * elementManager->elementSize, tempData, ((size_t)elementManager->count - (size_t)insertionIndex) * elementManager->elementSize);

				void* tempInfo = malloc(((size_t)elementManager->count - (size_t)insertionIndex) * sizeof(ElementInfo));
				memcpy(tempInfo, (char*)elementManager->infos + insertionIndex * sizeof(ElementInfo), ((size_t)elementManager->count - (size_t)insertionIndex) * sizeof(ElementInfo));
				memcpy((char*)elementManager->infos + ((size_t)insertionIndex + 1) * sizeof(ElementInfo), tempInfo, ((size_t)elementManager->count - (size_t)insertionIndex) * sizeof(ElementInfo));

				free(tempData);
				free(tempInfo);
			}
			else
			{
				elementManager->count--;
			}
		}

		void* pointerToElement = (char*)elementManager->data + insertionIndex * elementManager->elementSize;

		elementManager->infos[insertionIndex].id = id;
		elementManager->infos[insertionIndex].isRemovable = false;
		memcpy(pointerToElement, data, elementManager->elementSize);

		elementManager->count++;

		return pointerToElement;

	}
	else
	{
		return nullptr;
	}
}


int Elements_GetCount(Elements elementManager)
{
	return elementManager->count;
}

Elements Elements_Create(ElementsMemoryManager* elementsMemory, int sizeOfEachElement)
{
	ElementManager* elementManager = (ElementManager*)(&elementsMemory->memory[elementsMemory->currentSize]);
	elementManager->count = 0;
	elementManager->elementSize = sizeOfEachElement;
	elementManager->data = (ElementManager*)(&elementsMemory->memory[elementsMemory->currentSize]) + 1;
	elementsMemory->currentSize += sizeof(ElementManager) + (unsigned long long)sizeOfEachElement * (unsigned long long)maxComponents;

	assert(elementsMemory->currentSize < elementsMemory->maxSize);

	return elementManager;
}
