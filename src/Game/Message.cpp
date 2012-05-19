#include "Message.h"
#include <Rendering/OpenGL.h>



Menu* msgGetMenu(MessageManager* MM)
{
	return MM->Messages;
}

Bool msgCanBeDrawn(MessageManager* MM)
{
	return (daGetSize(MM->Messages->Menus) >0);
}

void CloseMessage(void* Data)
{
	MessageManager* MM = (MessageManager*)Data;
	mnSetHide(MM->Messages, TRUE);
	MM->CloseMessage = 1;
}


void msgInit(MessageManager* MM, s_SharedResources* SR)
{
	MM->Queue = newList();
	MM->QueueID = newList();
	MM->Messages = (Menu*)malloc(sizeof(Menu));
	MM->Menus = (Menu*)malloc(sizeof(Menu));
	MM->TextAlpha = 0.f;
	MM->ToBeDeleted = NULL;
	MM->SR = SR;
	
	mnInit(MM->Menus);
	mnInit(MM->Messages);
	MM->Menus->Type = MENU_TYPE_MESSAGE;
}

void msgFree(MessageManager* MM)
{
	Node* it = lstFirst(MM->QueueID);
	while (!nodeEnd(it))
	{
		free(nodeGetData(it));
		it = nodeGetNext(it);
	}
	delList(MM->Queue); //Pas besoin de liberer les nodes car ce ne sont que des pointeurs
	delList(MM->QueueID);
	
	mnFree(MM->Messages);
	mnFree(MM->Menus);
	
	free(MM->Messages);
	free(MM->Menus);
}


void msgHandleEvent(MessageManager* MM, const sf::Event& event)
{
	if (daGetSize(MM->Menus->Menus) >0)
	{
		
		mnHandleEvent(MM->Menus, event);
	}
	
	
	if (daGetSize(MM->Messages->Menus) >0)
	{
		
		
		// Messages doit avoir la priorité sur les menus, étant donné qu'on l'a desactivé c'est bon
		mnHandleEvent(MM->Messages, event);
	}
}

void msgUpdate(MessageManager* MM)
{
	if (MM->ToBeDeleted != NULL && !mnIsVisible(MM->Messages)) //un message doit être detruit
	{
		mnRemoveMenu(MM->Messages, MM->ToBeDeleted);
		MM->ToBeDeleted = NULL;
	}
	
	
	//Dégradé de l'opacité du texte
	if (mnGetMessageScale(MM->Menus)>0.95f)
		MM->TextAlpha = MIN(1.f, MM->TextAlpha + 0.05f);
	else
		MM->TextAlpha = MAX(0.f, MM->TextAlpha - 0.05f);
	
	//On fait des update que si des menu existent!
	if (daGetSize(MM->Menus->Menus) >0)
	{
		mnUpdate(MM->Menus, vec2(0.f, 0.f), vec2(0.f, 0.f));
		//mnHandleEvent(MM->Menus, event);
	}
		
	
	if (daGetSize(MM->Messages->Menus) >0)
	{
		float x = 1200.f/2.f - moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f;
		mnUpdate(MM->Messages, vec2(x, 800.f/2.f - moiGetSize(mnGetCurrentMenu(MM->Messages)).y/2.f), vec2(x, -mnGetHeight(MM->Messages) - 100.f));
		
		// Messages doit avoir la priorité sur les menus, étant donné qu'on l'a desactivé c'est bon
		//mnHandleEvent(MM->Messages, event);
	}
	
	
	
	
}



void msgCreateMessage(MessageManager* MM,const char* Title, unsigned int ItemCount)
{
	assert(daGetSize(MM->Messages->Menus) == 0);
	mnAddMenu(MM->Messages, Title, ItemCount);
}

void msgAddItem(MessageManager* MM,const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	assert(daGetSize(MM->Messages->Menus) > 0);
	
	mnAddItem(MM->Messages, 0, Text, Type, Function, Data);
}

void msgAddItemWithArg(MessageManager* MM, const char* Text, void (*Function)(void*), void* Arg)
{
	mnAddItemWithArg(MM->Messages, 0, Text, Function, Arg);
}

Bool msgCanDisplay(MessageManager* MM)
{
	return (daGetSize(MM->Messages->Menus) > 0);
}

void msgDisplay(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	mnSetHide(MM->Messages, FALSE);
	
	sf::Clock cl;
	cl.restart();
	sf::Texture Screenshot; Texture Screeny; sf::Image img;
	Screenshot.create(win.getSize().x, win.getSize().y);
	Screenshot.update(win);
	float MouseX, MouseY;
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


	printf("time: %f\n", cl.getElapsedTime().asSeconds());
	
	mnSetPosition(MM->Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f, -100.f));

	while (mnIsVisible(MM->Messages) || !MM->CloseMessage)
	{

		MouseX = sf::Mouse::getPosition(win).x;
		MouseY = sf::Mouse::getPosition(win).y;
		
		sf::Event event;
		while (win.pollEvent(event))
		{
			mnHandleEvent(MM->Messages, event);
			
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				CloseMessage(MM);
		}
		
		mnUpdate(MM->Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f, 100.f), vec2(ViewWidth/4.f-moiGetSize(mnGetCurrentMenu(MM->Messages)).x/2.f, -mnGetHeight(MM->Messages) - 100.f));
		
		
		glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0.f, 0.f, 0.f, 0.2f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);
		
		
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Screeny);
		glPushMatrix();
		glTranslatef(ViewX, ViewY, 0.f);
		glScalef(ViewWidth/win.getSize().x, ViewHeight/win.getSize().y, 1.f);
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
				
		win.display();
		
	}
	
	//On libère le menu
	mnRemoveMenu(MM->Messages, (MenuID)0);
	glTexFree(Screeny);
	
	printf("Message displayed\n");
}
