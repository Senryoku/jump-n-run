#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <Menu/Menu.h>
#include <Core/List.h>

/**
 * @defgroup Message
 * Module permettant d'afficher des message et des menu de click droit. Ces messages utilisent le module des menu ce qui permet de lire des donnés avec les input. L'affichage est bloquant c'est à dire qu'une boucle est créée à l'attente de la fermeture du message ou du menu
 * préfixe: msg
 * @{
 */

struct s_SharedResources;

typedef struct s_Message {
	List* Queue, *QueueID; //queue contient les pointeurs vers les menuofitems et queueid contient les id, qui permettent de passer d'un menu à un autre
	Menu* Messages; //Boites de dialogues
	Menu* Menus; //Menus de click droit (CD)
	float TextAlpha; // Alpha du texte pour les menu de CD
	MenuOfItems* ToBeDeleted; //MenuId d'un message qui a été montré et qui doit ètre supprimé
	Bool CloseMessage;
	s_SharedResources* SR;
} MessageManager;


typedef MenuOfItems* MessageID;

/** @brief Initialise Le manager de Messages
 *
 * @param MM MessageManager auquel s'applique la fonction
 * @param MainMenu Menu principal (Il doit être désactivé lors des messages)
 **/
void msgInit(MessageManager* MM, s_SharedResources* SR);

/** @brief Libère Le manager de Messages
 *
 * @param MM MessageManager auquel s'applique la fonction
 **/
void msgFree(MessageManager* MM);

/** @brief Crée un message simple et l'ajoute à la queue
 *
 * Ce message attend dans la queue pour être montré. pour le montrer directement en cachant le menu actuel utiliser msgForceToShow
 * @param MM MessageManager auquel s'applique la fonction
 * @param Title titre de la boîte de dialogue. Peut être nulle
 * @param Texte du message
 * @param Button Texte qui sera affiché dans le bouton pour fermer le message ex: Ok
 * @param Force Force le menu à apparaître ou il est mis dans la queue
 * @return MessageID du message qu'on vient de créer. Nécessaire si on veut faire des modifications ou récuperer des donnés (Pour des messages ayant des inputs)
 **/
MessageID msgShow(MessageManager* MM, const char* Title, const char* Text, const char* Button, Bool Force);

void msgCreateMessage(MessageManager* MM,const char* Title, unsigned int ItemCount);
//Ajoute un item au message qui va ètre montré
void msgAddItem(MessageManager* MM, const char* Text, ItemType Type, void (*Function)(void), void* Data);
void msgAddItemWithArg(MessageManager* MM, const char* Text, void (*Function)(void*), void* Arg);

//Montre le message et attend  une réponse. ensuite il est détruit
void msgDisplay(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);


void msgUpdate(MessageManager* MM);
void msgHandleEvent(MessageManager* MM, const sf::Event& event);

Bool msgCanDisplay(MessageManager* MM);


void CloseMessage(void* MM);
void CloseMessageOLD(void* MM);

Menu* msgGetMenu(MessageManager* Data);

Bool msgCanBeDrawn(MessageManager* Data);

/**@}**/
#endif