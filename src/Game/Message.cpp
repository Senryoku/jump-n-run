#include "Message.h"

typedef struct {
	List* Queue;
	Menu* Messages;
	Menu* MainMenu;
} MessageManager;

MessageManager MM;


void CloseMessage()
{
	
}

//Fonction d'aide pour la queue de message
MessageID* newmsgID(MessageID ID);

void msgInit(Menu* MainMenu)
{
	MM.MainMenu = MainMenu;
	MM.Queue = newList();
	MM.Messages = (Menu*)malloc(sizeof(Menu));
}

void msgFree()
{
	Node* it = lstFirst(MM.Queue);
	while (!nodeEnd(it))
	{
		free(nodeGetData(it));
		it = nodeGetNext(it);
	}
	delList(MM.Queue);
	free(MM.Messages);
}

MessageID msgShow(const char* Title, const char* Text, const char* Button, Bool Force)
{
	MenuID MID;
	MID = mnAddMenu(MM.Messages, Title, 2);
	mnAddItem(MM.Messages, MID, Text, ITEM_LABEL, NULL, NULL);
	mnAddItem(MM.Messages, MID, Button, ITEM_BUTTON, &CloseMessage, NULL);
	
	if (Force)
	{
		lstAddAtBeginning(MM.Queue, newmsgID(MID));
		//chacher l'autre et changer de menu
		mnGoToMenu(MM.Messages, MID);
		mnSetHide(MM.Messages, FALSE);
		mnSetActive(MM.MainMenu, FALSE);
	}
	else
		lstAdd(MM.Queue, newmsgID(MID));
	
	return MID;
}

MessageID* newmsgID(MessageID ID)
{
	MessageID* MID = (MessageID*)malloc(sizeof(MessageID));
	*MID = ID;
	return MID;
}


void msgUpdate()
{
	
}