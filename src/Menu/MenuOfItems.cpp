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
	M->SizeSpd[0] = 0.f;
	M->SizeSpd[1] = 0.f;
	M->Size = vec2(0.f, 0.f);
	M->SizeTo = vec2(100.f, 100.f);
}

void moiFree(MenuOfItems* M)
{
	unsigned short i;
	free(M->Text);
	/* On libère d'abord les items */
	for (i=0; i<M->ItemsAdded; i++)
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
	while (moved<M->ItemsAdded)
	{
		if (Direction == MENU_GO_DOWN)
			M->ItemSelected++;
		else if (M->ItemSelected != 0)
			M->ItemSelected--;
		else
			M->ItemSelected = M->ItemsAdded-1;
		
		if (M->ItemSelected >= M->ItemsAdded)
			M->ItemSelected = 0;
		
		if (mniGetType(moiGetItemSelected(M)) != ITEM_LABEL )
			moved = M->ItemsAdded+1;
		else
			moved++;
	}
	
	if (moved == M->ItemsAdded)
		M->ItemSelected = INVALID_ITEM_ID;
}

MenuItem* moiGetItemSelected(MenuOfItems* M)
{
	return &M->Items[M->ItemSelected];
}

MenuItem* moiGetItem(MenuOfItems* M, ItemID IID)
{
	assert(IID < M->ItemsAdded);
	return &M->Items[IID];
}

void moiUpdateVisuals(MenuOfItems* M, float SelectedFactor, float UnselectedFactor)
{
	unsigned short i;
	float Factor;
	for (i=0; i<M->ItemsAdded; i++)
	{
		if (M->ItemSelected == i)
			Factor = SelectedFactor;
		else
			Factor = UnselectedFactor;
		
		Wobble(mniGetZoom(&M->Items[i]), Factor, 0.5f, 0.5f, &M->ItemsZoomspd[i]);
	}
	
	Wobble(&M->Size.x, M->SizeTo.x, 0.5f, 0.5f, &M->SizeSpd[0]);
	Wobble(&M->Size.y, M->SizeTo.y, 0.5f, 0.5f, &M->SizeSpd[1]);
}

void moiSetSize(MenuOfItems* M, Vec2 Size)
{
	M->SizeTo = Size;
}

Vec2 moiGetSize(const MenuOfItems* M)
{
	return M->SizeTo;
}

void moiSetActualSize(MenuOfItems* M, Vec2 Size)
{
	M->Size = Size;
}

unsigned short moiGetItemCount(const MenuOfItems* M)
{
	return M->ItemsAdded;
}

ItemID moiGetItemSelectedID(MenuOfItems* M)
{
	return M->ItemSelected;
}
