#include "Message.h"
#include <Rendering/OpenGL.h>



Menu* msgGetMenu(MessageManager* MM)
{
	return MM->Messages;
}

void CloseMessage(void* MM);

void CloseMessage(void* Data)
{
	MessageManager* MM = (MessageManager*)Data;
	mnSetHide(MM->Messages, TRUE);
	MM->CloseMessage = 1;
	MM->LastChoice = moiGetItemSelectedID(mnGetCurrentMenu(MM->Messages));

	//Pour les input on va prendre l'input du premier item car normalement un message à input n'a qu'un seul input
	MenuItem* I = NULL;
	int i;
	for (i=0; i<moiGetItemCount(mnGetCurrentMenu(MM->Messages)); i++)
	{
		ItemType T = mniGetType(mnGetItem(MM->Messages, 0, i));
		if (T == ITEM_INPUT || T == ITEM_INPUT_MULTILINE || T == ITEM_INPUT_VALUE || T == ITEM_VALUE)
		{
			I = mnGetItem(MM->Messages, 0, i);
			break;
		}

	}

	if (I == NULL) return;

	ItemType T = mniGetType(I);

	if (T == ITEM_INPUT || T == ITEM_INPUT_MULTILINE || T == ITEM_INPUT_VALUE)
	{
		free(MM->LastInput);
		size_t l = strlen(mniGetInputChr(I));
		MM->LastInput = (char*)malloc(sizeof(char)*l);
		strcpy(MM->LastInput, mniGetInputChr(I)); //le \0 est copié aussi
	}
	else if (T == ITEM_VALUE)
	{
		float v = *(float*)mniGetData(I);
		free(MM->LastInput);
		MM->LastInput = (char*)malloc(sizeof(char)*100);
		sprintf(MM->LastInput, "%f",v);

	}

}


void msgInit(MessageManager* MM, s_SharedResources* SR)
{
	MM->Messages = (Menu*)malloc(sizeof(Menu));
	MM->SR = SR;
	MM->LastChoice = 0;
	MM->LastInput = (char*)malloc(sizeof(char));
	MM->LastInput[0] = '\0';

	mnInit(MM->Messages);
}

void msgFree(MessageManager* MM)
{
	mnFree(MM->Messages);

	free(MM->Messages);
	free(MM->LastInput);
}



void msgCreateMessage(MessageManager* MM,const char* Title, unsigned int ItemCount)
{
	assert(daGetSize(MM->Messages->Menus) == 0);
	mnAddMenu(MM->Messages, Title, ItemCount);
	mnSetType(MM->Messages, MENU_TYPE_DEFAULT);
}

void msgCreateMenu(MessageManager* MM, unsigned int ItemCount)
{
	assert(daGetSize(MM->Messages->Menus) == 0);
	mnAddMenu(MM->Messages, "", ItemCount);
	mnSetType(MM->Messages, MENU_TYPE_RIGHT_CLICK);
}

ItemID msgAddItem(MessageManager* MM,const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	assert(daGetSize(MM->Messages->Menus) > 0);
	return mnAddItem(MM->Messages, 0, Text, Type, Function, Data);
}

ItemID msgAddCloseItem(MessageManager* MM, const char* Text)
{
	assert(daGetSize(MM->Messages->Menus) > 0);
	return mnAddItemWithArg(MM->Messages, 0, Text, &CloseMessage, MM);
}

ItemID msgAddItemWithArg(MessageManager* MM, const char* Text, void (*Function)(void*), void* Arg)
{
	assert(daGetSize(MM->Messages->Menus) > 0);
	return mnAddItemWithArg(MM->Messages, 0, Text, Function, Arg);
}


void msgDisplay(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	mnSetHide(MM->Messages, FALSE);

	glPushMatrix();
	
	//sf::Clock cl;
	//cl.restart();
	sf::Texture Screenshot; Texture Screeny; sf::Image img;
	Screenshot.create(win.getSize().x, win.getSize().y);
	Screenshot.update(win);
	float MouseX, MouseY;
	MouseX = sf::Mouse::getPosition(win).x;
	MouseY = sf::Mouse::getPosition(win).y;


	MM->CloseMessage = FALSE;
	mnSetPosition(MM->Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f, -100.f));
	mnSetItemNormalZoomFactor(MM->Messages, 0.75f);

	//écran de fond
	img = Screenshot.copyToImage();
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &Screeny);
	glBindTexture(GL_TEXTURE_2D, Screeny);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Screenshot.getSize().x, Screenshot.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


	//c'est un peu lent... 0.03s
	//printf("time: %f\n", cl.getElapsedTime().asSeconds());
	if (mnGetType(MM->Messages) == MENU_TYPE_DEFAULT)
		mnSetPosition(MM->Messages, vec2(win.getSize().x/2.f - moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f, -100.f));
	else
		mnSetPosition(MM->Messages, vec2(MouseX, MouseY));

	while (mnIsVisible(MM->Messages) || !MM->CloseMessage)
	{

		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				CloseMessage(MM);
				if (event.key.code == sf::Keyboard::Escape)
					MM->LastChoice = INVALID_ITEM_ID;
				break; // ça évite de doubler l'event avec les bouttons
			}


			mnHandleEvent(MM->Messages, event);


		}

		float menuPosx = win.getSize().x/2.f - moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f;
		if (mnGetType(MM->Messages) == MENU_TYPE_DEFAULT)
			mnUpdate(MM->Messages, vec2(menuPosx, 100.f), vec2(menuPosx, -mnGetHeight(MM->Messages) - 100.f));
		else
			mnUpdate(MM->Messages, vec2(0.f, 0.f), vec2(0.f, 0.f));


		glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0.f, 0.f, 0.f, 0.2f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glViewport(0.f, 0.f, win.getSize().x, win.getSize().y);
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);

		
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Screeny);
		glPushMatrix();
		glTranslatef(ViewX, ViewY, 0.f);
		//glScalef(ViewWidth/win.getSize().x, ViewHeight/win.getSize().y, 1.f);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2f(0.f, 0.f);
		glTexCoord2i(1, 0);
		glVertex2f(ViewWidth, 0.f);
		glTexCoord2i(1, 1);
		glVertex2f(ViewWidth, ViewHeight);
		glTexCoord2i(0, 1);
		glVertex2f(0.f, ViewHeight);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
		
		
		glDrawMenuBox(MM->SR, win, MM->Messages, ViewX, ViewY, ViewWidth, ViewHeight);
	

		glDrawMenuItems(MM->SR, win, MM->Messages, ViewX, ViewY, ViewWidth, ViewHeight);

		if (mnIsItemSelectedWithCursor(MM->Messages))
			glDrawCursor(win, ViewWidth, ViewHeight, sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y, shGetCursorSprite(MM->SR, SPR_CURSOR_HAND));
		else
		glDrawCursor(win, ViewWidth, ViewHeight, sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y, shGetCursorSprite(MM->SR, SPR_CURSOR));
		
		win.display();

	}

	//On libère le menu
	mnRemoveMenu(MM->Messages, (MenuID)0);
	glTexFree(Screeny);
	
	glPopMatrix();

}

ItemID msgGetChoice(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	msgDisplay(MM, win, ViewX, ViewY, ViewWidth, ViewHeight);
	return MM->LastChoice;
}

const char* msgGetInput(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	msgDisplay(MM, win, ViewX, ViewY, ViewWidth, ViewHeight);
	return MM->LastInput;
}

/*Bool msgCanDisplay(MessageManager* MM)
{
	return daGetSize(MM->Messages->Menus) > 0;
}*/