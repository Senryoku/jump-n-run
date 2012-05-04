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
	M->MenuX = 0.f;
	M->MenuY = -100.f;
	M->SubAnim = 0.f;
	M->Force = 0.25f;
	M->Friction = 0.4f;
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

float mnGetForce(const Menu* M)
{
	return M->Force;
}

float mnGetFriction(const Menu* M)
{
	return M->Friction;
}

void mnSetForce(Menu* M, float Force)
{
	M->Force = Force;
}

void mnSetFriction(Menu* M, float Friction)
{
	M->Friction = Friction;
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

void mnSetItemNormalZoomFactor(Menu* M, float ItemNormalZoomFactor)
{
	M->ItemNormalZoomFactor = ItemNormalZoomFactor;
}

float mnGetItemNormalZoomFactor(const Menu* M)
{
	return M->ItemNormalZoomFactor;
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
	
	M->SubAnim+=0.25f;
	if (M->SubAnim>=19.f)
		M->SubAnim = 0.f;
	
	if (M->CurrentMenu != M->PreviousMenu)
	{
		Wobble(&M->MenuX, OutPos.x, M->Force, M->Friction, &M->spd[0]);
		Wobble(&M->MenuY, OutPos.y, M->Force, M->Friction, &M->spd[1]);
		
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
		Wobble(&M->MenuX, MenuPos.x, M->Force, M->Friction, &M->spd[0]);
		Wobble(&M->MenuY, MenuPos.y, M->Force, M->Friction, &M->spd[1]);
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
	return (MenuOfItems*)daGet(M->Menus, M->PreviousMenu);
}

void mnMoveCursor(Menu* M, MenuDirection Direction)
{
	moiMoveCursor(mnGetCurrentMenu(M), Direction);
}

void mnSetCursor(Menu* M, Vec2 MousePos)
{
	MousePos.y-=17.f;
	if (MousePos.y < M->MenuY || MousePos.y > M->MenuY+mnGetHeight(M))
		mnGetCurrentMenu(M)->ItemSelected = INVALID_ITEM_ID;
	else
	{
		int pos = (MousePos.y - M->MenuY)/(mnGetItemHeight(M)*mnGetItemNormalZoomFactor(M));
		//printf("pos: %i\n", pos);
		//if ((pos+1)*(mnGetItemHeight(M)*mnGetItemNormalZoomFactor(M)) < MousePos.y - M->MenuY <
		if (pos < mnGetCurrentMenu(M)->ItemsAdded)
			moiSetCursor(mnGetCurrentMenu(M), (ItemID)pos);
	}
	
}

Vec2 mnGetPosition(const Menu* M)
{
	return vec2(M->MenuX, M->MenuY);
}

MenuItem* mnGetItem(const Menu* M, MenuID MID, ItemID IID)
{
	return moiGetItem((MenuOfItems*) daGet(M->Menus, MID), IID);
}

MenuItem* mnGetCurrentItem(const Menu* M)
{
	return moiGetItemSelected(mnGetCurrentMenu(M));
}

void mnHandleEvent(Menu* M, const sf::Event& event)
{
	if (!mnGetActive(M))
		return;
	
	if (event.type == sf::Event::MouseMoved)
		mnSetCursor(M, vec2(event.mouseMove.x, event.mouseMove.y));
	
	Bool Enter = ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return));
	
	if (Enter)
		printf("Enter\n");
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		mnMoveCursor(M, MENU_GO_DOWN);
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
		mnMoveCursor(M, MENU_GO_UP);
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		mniUse(M, mnGetCurrentItem(M), FALSE, MOVE_LEFT, 0, FALSE);
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		mniUse(M, mnGetCurrentItem(M), FALSE, MOVE_RIGHT, 0, FALSE);
	 
	
	
	if (event.type == sf::Event::TextEntered)
	{
		unsigned int c;
		sf::Utf32::encodeAnsi(event.text.unicode, &c);
		//if (i>=32 && i<=126) /* printables chars */
		mniUse(M, mnGetCurrentItem(M), FALSE, MOVE_NONE, (unsigned char) c, FALSE);
		//if (mniGetType(moiGetItemSelected(mnGetCurrentMenu(&G->GameMenu))) == ITEM_INPUT_VALUE)
		//	printf("value is %f\n", mniGetInputValue(moiGetItemSelected(mnGetCurrentMenu(&G->GameMenu))));
		//printf("text entered: %c", c);
	}
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Back)
		mniUse(M ,mnGetCurrentItem(M), FALSE, MOVE_NONE, 0, TRUE);
	
	if (Enter)
		mniUse(M, mnGetCurrentItem(M), TRUE, MOVE_NONE, 0, FALSE);
	
}

float mnGetHeight(const Menu* M)
{
	return (float)(mnGetCurrentMenu(M)->ItemsAdded*M->ItemHeight*mnGetItemNormalZoomFactor(M));
}
