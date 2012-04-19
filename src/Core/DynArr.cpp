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
#include <stdio.h>
void daReserve(DynArr* DA, unsigned int newCapa)
{
	void* *tmp;
	unsigned int i;
	if(newCapa > DA->Capacity)
	{
		printf("1\n");
		tmp = (void**) malloc(newCapa*sizeof(void*));
		printf("2\n");
		for(i = 0; i < DA->Size; i++)
			tmp[i] = DA->First[i];
		printf("3\n");
		free(DA->First);
		printf("4\n");
		DA->First = tmp;
		printf("5\n");
		DA->Capacity = newCapa;
		printf("6\n");
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

unsigned int daGetID(DynArr* D, const void* ptr)
{
	unsigned int i = 0;

	while (i<daGetSize(D) && daGet(D, i)!=ptr)
		i++;

	return i;

}

#include <assert.h>
void daRegressionTest()
{
	DynArr* DA = newDynArr();
	unsigned int i;
	for(i = 0; i < 100; i++)
	{
		daAdd(DA, (void*) i);
		assert(daGet(DA, i) == (void*) i);
		assert(daGetSize(DA) <= daGetCapacity(DA));
	}
	i = daGetSize(DA);
	daDel(DA, 50);
	assert(daGet(DA, 50) == (void*) 51);
	assert(daGetSize(DA) == i - 1);
	daFastDel(DA, 50);
	assert(daGet(DA, 50) == (void*) 99);
	assert(daGetSize(DA) == i - 2);
	delDynArr(DA);
	assert(daGetSize(DA) == 0);

	DA = newDynArr();
	daReserve(DA, 21);
	assert(daGetCapacity(DA) == 21);
	for(i = 0; i < 21; i++)
	{
		daAdd(DA, (void*) i);
		assert(daGet(DA, i) == (void*) i);
		assert(daGetSize(DA) <= daGetCapacity(DA));
	}
	assert(daGetCapacity(DA) == 21);
	delDynArr(DA);
}
