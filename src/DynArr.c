#include "DynArr.h"

DynArr da()
{
	DynArr DA;
	daInit(&DA);
	return DA;
}

void daInit(DynArr* DA)
{
	DA->First = calloc(1, sizeof(void*));
	DA->Capacity = 1;
	DA->Size = 0;
}

void daFree(DynArr* DA)
{
	free(DA->First);
	DA->Capacity = 0;
	DA->Size = 0;
}

void daReserve(DynArr* DA, unsigned int newCapa)
{
	void* *tmp;
	int i;
	if(newCapa > DA->Capacity)
	{
		tmp = calloc(newCapa, sizeof(void*));
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
	int i;
	for(i = Pos; i < DA->Size - 1; i++)
		DA->First[i] = DA->First[i+1];
	DA->Size--;
}

void* daGet(const DynArr* DA, unsigned int Pos)
{
	return DA->First[Pos];
}
