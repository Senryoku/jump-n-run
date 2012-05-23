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
	float TextAlpha; // Alpha du texte pour les menu de CD
	MenuOfItems* ToBeDeleted; //MenuId d'un message qui a été montré et qui doit ètre supprimé
	Bool CloseMessage;
	s_SharedResources* SR;
	ItemID LastChoice; ///< Contient le dernier choix d'un menu ou d'un message affiché
	char* LastInput; ///< Contient l'input du dernier message
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


void msgCreateMessage(MessageManager* MM,const char* Title, unsigned int ItemCount);

void msgCreateMenu(MessageManager* MM, unsigned int ItemCount);
//Ajoute un item au message qui va ètre montré
ItemID msgAddItem(MessageManager* MM, const char* Text, ItemType Type, void (*Function)(void), void* Data);
ItemID msgAddItemWithArg(MessageManager* MM, const char* Text, void (*Function)(void*), void* Arg);
ItemID msgAddCloseItem(MessageManager* MM, const char* Text);

//Montre le message et attend  une réponse. ensuite il est détruit
void msgDisplay(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

//Montre le message et attend  une réponse. ensuite il est détruit. ceci renvoit l'indice du choix de l'utilisateur
ItemID msgGetChoice(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

//montre un message et retourne l'input
const char* msgGetInput(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);


void CloseMessage(void* MM);

Menu* msgGetMenu(MessageManager* MM);

Bool msgCanDisplay(MessageManager* MM);


/**@}**/
#endif