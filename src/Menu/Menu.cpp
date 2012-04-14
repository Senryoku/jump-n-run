#include "Menu.h"

void mnInit(Menu* M)
{
	M->Menus = newDynArr();
	M->CurrentMenu = 0;
	M->PreviousMenu = 0;
	M->Active = TRUE;
	M->ItemHeight = 25.f;
	M->ItemSelectedZoomFactor = 1.2f;
	M->ItemNormalZoomFactor = 0.5f;
	M->spd[0] = 0.f;
	M->spd[1] = 0.f;
}


void mnFree(Menu* M)
{
	unsigned int i;
	for (i=0; i<daGetSize(M->Menus); i++)
	{
		moiFree((MenuOfItems*)daGet(M->Menus, i));
		free(daGet(M->Menus, i));
	}
	delDynArr(M->Menus);
}

MenuID mnAddMenu(Menu* M, const char* Text, unsigned short ItemCount)
{
	MenuOfItems* moi = (MenuOfItems*)malloc(sizeof(MenuOfItems));
	moiInit(moi, Text, ItemCount);
	daAdd(M->Menus, moi);
	
	return daGetSize(M->Menus)-1;
}

ItemID mnAddItem(Menu* M, MenuID MID, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	MenuOfItems* moi = (MenuOfItems*)daGet(M->Menus, MID);
	moiAddItem(moi, Text, Type, Function, Data);
	
	return (moi->ItemsAdded-1);
}

void mnSetItemHeight(Menu* M, float ItemHeight)
{
	M->ItemHeight = ItemHeight;
}

float mnGetItemHeight(const Menu* M)
{
	return M->ItemHeight;
}

void mnSetItemSelectedZoomFactor(Menu* M, float ItemSelectedZoomFactor)
{
	M->ItemSelectedZoomFactor = ItemSelectedZoomFactor;
}

float mnGetItemSelectedZoomFactor(const Menu* M)
{
	return M->ItemSelectedZoomFactor;
}

void mnSetActive(Menu* M, Bool Active)
{
	M->Active = Active;
}

Bool mnGetActive(const Menu* M)
{
	return M->Active;
}

void mnUpdate(Menu* M, Vec2 MenuPos, Vec2 OutPos)
{
	moiUpdateVisuals((MenuOfItems*)daGet(M->Menus, M->CurrentMenu), M->ItemSelectedZoomFactor, M->ItemNormalZoomFactor);
	
	if (M->CurrentMenu != M->PreviousMenu)
	{
		Wobble(&M->MenuX, OutPos.x, 0.5f, 0.5f, &M->spd[0]);
		Wobble(&M->MenuY, OutPos.y, 0.5f, 0.5f, &M->spd[1]);
		
		if (ABS(M->MenuX-OutPos.x) < 1.f && ABS(M->MenuY-OutPos.y) < 1.f)
		{
			M->PreviousMenu = M->CurrentMenu;
			M->Active = TRUE;
			/* On va aller chercher le premier élement du menu qui peut être sélectioné */
			if (mniGetType(moiGetItemSelected(mnGetCurrentMenu(M))) == ITEM_LABEL)
				moiMoveCursor(mnGetCurrentMenu(M), MENU_GO_DOWN);
		}
	}
	else
	{
		Wobble(&M->MenuX, MenuPos.x, 0.5f, 0.5f, &M->spd[0]);
		Wobble(&M->MenuY, MenuPos.y, 0.5f, 0.5f, &M->spd[1]);
	}
}

void mnGoToMenu(Menu* M, MenuID MID)
{
	M->CurrentMenu = MID;
	M->Active = FALSE;
}

MenuID mnGetCurrentMenuID(const Menu* M)
{
	return M->CurrentMenu;
}

MenuOfItems* mnGetCurrentMenu(const Menu* M)
{
	return (MenuOfItems*)daGet(M->Menus, M->CurrentMenu);
}

void mnMoveCursor(Menu* M, MenuDirection Direction)
{
	moiMoveCursor(mnGetCurrentMenu(M), Direction);
}

void mnSetCursor(Menu* M, Vec2 MousePos)
{
	
}

Vec2 mnGetPosition(const Menu* M)
{
	return vec2(M->MenuX, M->MenuY);
}

MenuItem* mnGetItem(const Menu* M, MenuID MID, ItemID IID)
{
	return moiGetItem((MenuOfItems*) daGet(M->Menus, MID), IID);
}