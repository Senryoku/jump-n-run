#include "Message.h"
#include <Rendering/OpenGL.h>

typedef struct s_Message {
	List* Queue, *QueueID; //queue contient les pointeurs vers les menuofitems et queueid contient les id, qui permettent de passer d'un menu à un autre
	Menu* Messages; //Boites de dialogues
	Menu* Menus; //Menus de click droit (CD)
	Menu* MainMenu; //Menu principal pour le rendre inactif
	float TextAlpha; // Alpha du texte pour les menu de CD
	MenuOfItems* ToBeDeleted; //MenuId d'un message qui a été montré et qui doit ètre supprimé
	Bool CloseMessage;
} MessageManager;

MessageManager MM;

Menu* msgGetMenu()
{
	return MM.Messages;
}

void msgSetMainMenu(Menu* M)
{
	MM.MainMenu = M;
}

Bool msgCanBeDrawn()
{
	return (daGetSize(MM.Messages->Menus) >0);
}

void CloseMessage()
{
	mnSetHide(MM.Messages, TRUE);
	MM.CloseMessage = 1;
}

void CloseMessageOLD()
{
	//On ferme puis regarde la queue, si elle n'est pas vide on affiche le nouveau sinon on remet le MainMenu en Actif
	Node* it = lstFirst(MM.QueueID);
	assert(it!=NULL);
	MM.ToBeDeleted = (MenuOfItems*)nodeGetData(lstFirst(MM.Queue));
	it = nodeGetNext(it); //on prends le deuxième élément
	
	if (!nodeEnd(it)) //il y a des messages qu attendent dans la queue
	{
		//On passe au menu suivant
		mnGoToMenu(MM.Messages, *(MenuID*)nodeGetData(it));
		mnSetHide(MM.Messages, FALSE);
		mnSetActive(MM.MainMenu, FALSE);
		mnSetActive(MM.Menus, FALSE);
		
		//On supprime de la queue le premier élément
		free(nodeGetData(lstFirst(MM.QueueID))); //on libère la mémoire
		lstPopFront(MM.QueueID);
		lstPopFront(MM.Queue);
	}
	else
	{
		//On va réactiver les autres menus
		mnSetHide(MM.Messages, TRUE);
		mnSetActive(MM.MainMenu, TRUE);
		mnSetActive(MM.Menus, TRUE);
		
		//On supprime de la queue le premier élément
		free(nodeGetData(lstFirst(MM.QueueID))); //on libère la mémoire
		lstPopFront(MM.QueueID);
		lstPopFront(MM.Queue);
	}
}

//Fonction d'aide pour la queue de message
MenuID* newmnID(MenuID ID);

void msgInit(Menu* MainMenu)
{
	MM.MainMenu = MainMenu;
	MM.Queue = newList();
	MM.QueueID = newList();
	MM.Messages = (Menu*)malloc(sizeof(Menu));
	MM.Menus = (Menu*)malloc(sizeof(Menu));
	MM.TextAlpha = 0.f;
	MM.ToBeDeleted = NULL;
	
	mnInit(MM.Menus);
	mnInit(MM.Messages);
	MM.Menus->Type = MENU_TYPE_MESSAGE;
}

void msgFree()
{
	Node* it = lstFirst(MM.QueueID);
	while (!nodeEnd(it))
	{
		free(nodeGetData(it));
		it = nodeGetNext(it);
	}
	delList(MM.Queue); //Pas besoin de liberer les nodes car ce ne sont que des pointeurs
	delList(MM.QueueID);
	
	mnFree(MM.Messages);
	mnFree(MM.Menus);
	
	free(MM.Messages);
	free(MM.Menus);
}

MessageID msgShow(const char* Title, const char* Text, const char* Button, Bool Force)
{
	MenuID MID;
	MID = mnAddMenu(MM.Messages, Title, 2);
	mnAddItem(MM.Messages, MID, Text, ITEM_LABEL, NULL, NULL);
	mnAddItem(MM.Messages, MID, Button, ITEM_BUTTON, &CloseMessageOLD, NULL);
	
	if (Force)
	{
		lstAddAtBeginning(MM.Queue, mnGetMenuPtr(MM.Messages, MID));
		lstAddAtBeginning(MM.QueueID, newmnID(MID));
		//chacher l'autre et changer de menu
		mnGoToMenu(MM.Messages, MID);
		mnSetHide(MM.Messages, FALSE);
		mnSetActive(MM.MainMenu, FALSE);
		mnSetActive(MM.Menus, FALSE);
	}
	else
	{
		lstAdd(MM.Queue, mnGetMenuPtr(MM.Messages, MID));
		lstAdd(MM.QueueID, newmnID(MID));
	}
		
	
	return mnGetMenuPtr(MM.Messages, MID);
}

MenuID* newmnID(MenuID ID)
{
	MenuID* MID = (MenuID*)malloc(sizeof(MenuID));
	*MID = ID;
	return MID;
}

void msgHandleEvent(const sf::Event& event)
{
	if (daGetSize(MM.Menus->Menus) >0)
	{
		
		mnHandleEvent(MM.Menus, event);
	}
	
	
	if (daGetSize(MM.Messages->Menus) >0)
	{
		
		
		// Messages doit avoir la priorité sur les menus, étant donné qu'on l'a desactivé c'est bon
		mnHandleEvent(MM.Messages, event);
	}
}

void msgUpdate()
{
	if (MM.ToBeDeleted != NULL && !mnIsVisible(MM.Messages)) //un message doit être detruit
	{
		mnRemoveMenu(MM.Messages, MM.ToBeDeleted);
		MM.ToBeDeleted = NULL;
	}
	
	
	//Dégradé de l'opacité du texte
	if (mnGetMessageScale(MM.Menus)>0.95f)
		MM.TextAlpha = MIN(1.f, MM.TextAlpha + 0.05f);
	else
		MM.TextAlpha = MAX(0.f, MM.TextAlpha - 0.05f);
	
	//On fait des update que si des menu existent!
	if (daGetSize(MM.Menus->Menus) >0)
	{
		mnUpdate(MM.Menus, vec2(0.f, 0.f), vec2(0.f, 0.f));
		//mnHandleEvent(MM.Menus, event);
	}
		
	
	if (daGetSize(MM.Messages->Menus) >0)
	{
		float x = 1200.f/2.f - moiGetSize(mnGetCurrentMenu(MM.Messages)).x/2.f;
		mnUpdate(MM.Messages, vec2(x, 800.f/2.f - moiGetSize(mnGetCurrentMenu(MM.Messages)).y/2.f), vec2(x, -mnGetHeight(MM.Messages) - 100.f));
		
		// Messages doit avoir la priorité sur les menus, étant donné qu'on l'a desactivé c'est bon
		//mnHandleEvent(MM.Messages, event);
	}
	
	
	
	
}



void msgCreateMessage(const char* Title, unsigned int ItemCount)
{
	assert(daGetSize(MM.Messages->Menus) == 0);
	mnAddMenu(MM.Messages, Title, ItemCount);
}

void msgAddItem(const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	assert(daGetSize(MM.Messages->Menus) > 0);
	
	mnAddItem(MM.Messages, 0, Text, Type, Function, Data);
}

Bool msgCanDisplay()
{
	return (daGetSize(MM.Messages->Menus) > 0);
}

void msgDisplay(sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	mnSetHide(MM.Messages, FALSE);
	
	sf::Clock cl;
	cl.restart();
	sf::Texture Screenshot; Texture Screeny; sf::Image img;
	Screenshot.create(win.getSize().x, win.getSize().y);
	Screenshot.update(win);
	float MouseX, MouseY;
	MM.CloseMessage = FALSE;
	mnSetPosition(MM.Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM.Messages)).x/2.f, -100.f));
	mnSetItemNormalZoomFactor(MM.Messages, 0.75f);
	
	//écran de fond
	img = Screenshot.copyToImage();
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &Screeny);
	glBindTexture(GL_TEXTURE_2D, Screeny);
	

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Screenshot.getSize().x, Screenshot.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


	printf("time: %f\n", cl.getElapsedTime().asSeconds());
	
	mnSetPosition(MM.Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM.Messages)).x/2.f, -100.f));

	while (mnIsVisible(MM.Messages) || !MM.CloseMessage)
	{

		MouseX = sf::Mouse::getPosition(win).x;
		MouseY = sf::Mouse::getPosition(win).y;
		
		sf::Event event;
		while (win.pollEvent(event))
		{
			mnHandleEvent(MM.Messages, event);
			
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				CloseMessage();
		}
		
		mnUpdate(MM.Messages, vec2(ViewWidth/2.f-moiGetSize(mnGetCurrentMenu(MM.Messages)).x/2.f, 100.f), vec2(ViewWidth/4.f-moiGetSize(mnGetCurrentMenu(MM.Messages)).x/2.f, -mnGetHeight(MM.Messages) - 100.f));
		
		
		glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0.f, 0.f, 0.f, 0.2f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);
		
		
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Screeny);
		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(0.f, 0.f);
		glTexCoord2f(1.f, 0.f);
		glVertex2f(ViewWidth, 0.f);
		glTexCoord2f(1.f, 1.f);
		glVertex2f(ViewWidth, ViewHeight);
		glTexCoord2f(0.f, 1.f);
		glVertex2f(0.f, ViewHeight);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		
		
		
		glDrawMenuBox(win, MM.Messages, ViewX, ViewY, ViewWidth, ViewHeight);
		glDrawMenuItems(win, MM.Messages, ViewX, ViewY, ViewWidth, ViewHeight);
				
		win.display();
		
	}
	
	//On libère le menu
	mnRemoveMenu(MM.Messages, (MenuID)0);
	glTexFree(Screeny);
	
	printf("Message displayed\n");
}
