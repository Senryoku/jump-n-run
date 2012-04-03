#include "DynArr.h"

DynArr da()
{
	DynArr DA;
	daInit(&DA);
	return DA;
}

DynArr* newDynArr()
{
	DynArr* DA = (DynArr*) malloc(sizeof(DynArr));
	daInit(DA);
	return DA;
}

void daInit(DynArr* DA)
{
	DA->First = (void**)malloc(sizeof(void*));
	DA->Capacity = 1;
	DA->Size = 0;
}

void daFree(DynArr* DA)
{
	free(DA->First);
	DA->Capacity = 0;
	DA->Size = 0;
}

void delDynArr(DynArr* DA)
{
	daFree(DA);
	free(DA);
}

void daReserve(DynArr* DA, unsigned int newCapa)
{
	void* *tmp;
	unsigned int i;
	if(newCapa > DA->Capacity)
	{
		tmp = (void**)malloc(newCapa*sizeof(void*));
		for(i = 0; i < DA->Size; i++)
			tmp[i] = DA->First[i];
		free(DA->First);
		DA->First = tmp;
		DA->Capacity = newCapa;
	}
}

void daAdd(DynArr* DA, void* ptr)
{
	if(DA->Size == DA->Capacity)
	{
		daReserve(DA, DA->Capacity*2);
	}
	DA->First[DA->Size] = ptr;
	DA->Size++;
}

void daDel(DynArr* DA, unsigned int Pos)
{
	unsigned int i;
	for(i = Pos; i < DA->Size - 1; i++)
		DA->First[i] = DA->First[i+1];
	DA->Size--;
}

void daFastDel(DynArr* DA, unsigned int Pos)
{
	DA->Size--;
	DA->First[Pos] = DA->First[DA->Size];
}

void* daGet(const DynArr* DA, unsigned int Pos)
{
	return DA->First[Pos];
}

unsigned int daGetSize(const DynArr* DA)
{
	return DA->Size;
}

unsigned int daGetCapacity(const DynArr* DA)
{
	return DA->Capacity;
}

unsigned int daGetID(DynArr* D,const void* ptr)
{
	unsigned int i = 0;
	
	while (i<daGetSize(D) && daGet(D, i)!=ptr)
		i++;
	
	return i;
	
}
