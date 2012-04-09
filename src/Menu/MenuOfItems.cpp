#include "MenuOfItems.h"

void moiInit(MenuOfItems* M, const char* Text, unsigned short ItemCount)
{
	unsigned short i;
	M->Text = (char*)malloc(sizeof(char)*strlen(Text));
	strcpy(M->Text, Text);
	M->ItemCount = ItemCount;
	M->Items = (MenuItem*)malloc(sizeof(MenuItem)*ItemCount);
	M->ItemsZoomspd = (float*)malloc(sizeof(float)*ItemCount);
	for (i=0; i<ItemCount; i++)
		M->ItemsZoomspd[i] = 0.f;
	M->ItemsAdded = 0;
	M->ItemSelected = 0;
}

void moiFree(MenuOfItems* M)
{
	unsigned short i;
	free(M->Text);
	/* On libère d'abord les items */
	for (i=0; i<M->ItemCount; i++)
		mniFree(&M->Items[i]);
	free(M->Items);
	free(M->ItemsZoomspd);
}

void moiAddItem(MenuOfItems* M, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	assert(M->ItemsAdded < M->ItemCount);
	mniInit(&M->Items[M->ItemsAdded], Text, Type, Function, Data);
	M->ItemsAdded++;
}

void moiMoveCursor(MenuOfItems* M, MenuDirection Direction)
{
	unsigned short moved = 0;
	/* Si on a que des labels (CHOSE À NE PAS FAIRE) ceci serait une boucle infinie */
	while (moved<M->ItemCount)
	{
		if (Direction == MENU_GO_DOWN)
			M->ItemSelected++;
		else if (M->ItemSelected != 0)
			M->ItemSelected--;
		else
			M->ItemSelected = M->ItemCount-1;
		
		if (M->ItemSelected >= M->ItemCount)
			M->ItemSelected = 0;
		
		if (mniGetType(moiGetItemSelected(M)) != ITEM_LABEL )
			moved = M->ItemCount;
		else
			moved++;
	}
}

MenuItem* moiGetItemSelected(MenuOfItems* M)
{
	return &M->Items[M->ItemSelected];
}

void moiUpdateZooms(MenuOfItems* M, float SelectedFactor)
{
	unsigned short i;
	float Factor;
	for (i=0; i<M->ItemsAdded; i++)
	{
		if (M->ItemSelected == i)
			Factor = SelectedFactor;
		else
			Factor = 1.f;
		
		Wobble(mniGetZoom(&M->Items[i]), Factor, 0.5f, 0.5f, &M->ItemsZoomspd[i]);
	}
}
