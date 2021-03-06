#include "MenuOfItems.h"

void moiInit(MenuOfItems* M, const char* Text, unsigned short ItemCount)
{
	unsigned short i;
	M->Text = (char*) malloc(sizeof(char)*(strlen(Text) + 1));
	strcpy(M->Text, Text);
	M->ItemCount = ItemCount;
	M->Items = (MenuItem*) malloc(sizeof(MenuItem)*ItemCount);
	M->ItemsZoomspd = (float*) malloc(sizeof(float)*ItemCount);
	for (i=0; i<ItemCount; i++)
		M->ItemsZoomspd[i] = 0.f;
	M->ItemsAdded = 0;
	M->ItemSelected = 0;
	M->SizeSpd[0] = 0.f;
	M->SizeSpd[1] = 0.f;
	M->Size = vec2(0.f, 0.f);
	M->SizeTo = vec2(100.f, 100.f);
	M->Force = 0.5f;
	M->Friction = 0.4f;
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

const char* moiGetText(const MenuOfItems* M)
{
	return M->Text;
}

void moiAddItem(MenuOfItems* M, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	assert(M->ItemsAdded < M->ItemCount);
	mniInit(&M->Items[M->ItemsAdded], Text, Type, Function, Data);
	M->ItemsAdded++;
}

void moiAddItemWithArg(MenuOfItems* M, const char* Text, void (*Function)(void*), void* Arg)
{
	assert(M->ItemsAdded < M->ItemCount);
	mniInitWithArg(&M->Items[M->ItemsAdded], Text, Function, Arg);
	M->ItemsAdded++;
}

void moiMoveCursor(MenuOfItems* M, MenuDirection Direction)
{
	unsigned short moved = 0;
	if (M->ItemSelected == INVALID_ITEM_ID)
		M->ItemSelected = 0;
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

void moiSetCursor(MenuOfItems* M, ItemID IID)
{
	if (mniGetType(moiGetItem(M, IID)) == ITEM_LABEL)
		M->ItemSelected = INVALID_ITEM_ID;
	else
		M->ItemSelected = IID;
		
}

MenuItem* moiGetItemSelected(MenuOfItems* M)
{
	if (M->ItemSelected == INVALID_ITEM_ID)
		return NULL;
	else
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

		Wobble(mniGetZoom(&M->Items[i]), Factor, M->Force, M->Friction, &M->ItemsZoomspd[i]);
	}

	Wobble(&M->Size.x, M->SizeTo.x, M->Force, M->Friction, &M->SizeSpd[0]);
	Wobble(&M->Size.y, M->SizeTo.y, M->Force, M->Friction, &M->SizeSpd[1]);
}

void moiSetSize(MenuOfItems* M, Vec2 Size)
{
	M->SizeTo = Size;
}

Vec2 moiGetSize(const MenuOfItems* M)
{
	return M->Size;
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

float moiGetForce(const MenuOfItems* M)
{
	return M->Force;
}

float moiGetFriction(const MenuOfItems* M)
{
	return M->Friction;
}

void moiSetForce(MenuOfItems* M, float Force)
{
	M->Force = Force;
}

void moiSetFriction(MenuOfItems* M, float Friction)
{
	M->Friction = Friction;
}

void moiRegressionTest()
{
	printf("=== moiRegressionTest Begin ===\n");
	MenuOfItems moi;
	
	moiInit(&moi, "teste", 4);
	
	for (int i=0; i<4; i++)
		moiAddItem(&moi, "texte", ITEM_INPUT, NULL, NULL);
	
	moiSetCursor(&moi, 0);
	moiMoveCursor(&moi, MENU_GO_UP);
	
	assert(moiGetItemSelectedID(&moi) == 3);
	
	
	moiFree(&moi);
	printf("=== moiRegressionTest End =====\n");
}
