#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <Menu/Menu.h>
#include <Core/List.h>

/**
 * @defgroup Message
 * Module permettant d'afficher des message. Ces messages utilisent le module des menu ce qui permet de lire des donnés avec les input.
 * préfixe: msg
 * @{
 */


typedef MenuOfItems* MessageID;

/** @brief Initialise Le manager de Messages
 *
 * @param MainMenu Menu principal (Il doit être désactivé lors des messages)
 **/
void msgInit(Menu* MainMenu);

/** @brief Libère Le manager de Messages
 *
 **/
void msgFree();

/** @brief Crée un message simple et l'ajoute à la queue
 *
 * Ce message attend dans la queue pour être montré. pour le montrer directement en cachant le menu actuel utiliser msgForceToShow
 * @param Title titre de la boîte de dialogue. Peut être nulle
 * @param Texte du message
 * @param Button Texte qui sera affiché dans le bouton pour fermer le message ex: Ok
 * @param Force Force le menu à apparaître ou il est mis dans la queue
 * @return MessageID du message qu'on vient de créer. Nécessaire si on veut faire des modifications ou récuperer des donnés (Pour des messages ayant des inputs)
 **/
MessageID msgShow(const char* Title, const char* Text, const char* Button, Bool Force);

void msgSetMainMenu(Menu* M);

void msgCreateMessage(const char* Title, unsigned int ItemCount);
//Ajoute un item au message qui va ètre montré
void msgAddItem(const char* Text, ItemType Type, void (*Function)(void), void* Data);

//Montre le message et attend  une réponse. ensuite il est détruit
void msgDisplay(sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);


void msgUpdate();
void msgHandleEvent(const sf::Event& event);

Bool msgCanDisplay();


void CloseMessage();
void CloseMessageOLD();

Menu* msgGetMenu();

Bool msgCanBeDrawn();

/**@}**/
#endif