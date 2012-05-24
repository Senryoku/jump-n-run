#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <Menu/Menu.h>
#include <Core/List.h>

/**
 * @defgroup MessageManager MessageManager
 * Module permettant d'afficher des message et des menu de click droit. Ces messages utilisent le module des menu ce qui permet de lire des donnés avec les input. L'affichage est bloquant c'est à dire qu'une boucle est créée à l'attente de la fermeture du message ou du menu
 * @code
 //Pour obtenir un choix
 msgCreateMessage(MM, "Pause", 3);
 msgAddCloseItem(MM, "Continue");
 msgAddCloseItem(MM, "Options");
 msgAddCloseItem(MM, "Exit");
 ItemID Choice = msgGetChoice(MM, Window, ViewX, ViewY, ViewWidth, ViewHeight);

 //un message simple
 msgCreateMessage(MM, "Alert", 2);
 msgAddItem(MM, "Voilà un message", ITEM_LABEL, NULL, NULL);
 msgAddCloseItem(MM, "Ok");
 msgDisplay(MM, Window, ViewX, ViewY, ViewWidth, ViewHeight);
 
 //Un message d'input
 msgCreateMessage(MM, "Alert", 2);
 msgAddItem(MM, "valeur", ITEM_INPUT, NULL, NULL);
 msgAddCloseItem(MM, "Ok");
 const char* Input = msgGetInput(MM, Window, ViewX, ViewY, ViewWidth, ViewHeight);
 //Si un autre msgGetInput est appelé alors Input ne sera plus valide!m
 
 //un menu de click droit
 msgCreateMenu(MM, 3); //Il faut juste changer cette ligne on on fait apparaître un menu sur la souris
 msgAddCloseItem(MM, "Continue");
 msgAddCloseItem(MM, "Options");
 msgAddCloseItem(MM, "Exit");
 ItemID Choice = msgGetChoice(MM, Window, ViewX, ViewY, ViewWidth, ViewHeight);
 @endcode
 
 * préfixe: msg
 * @{
 */

struct s_SharedResources;

typedef struct s_Message {
	Menu* Messages; ///<Boites de dialogues et menus de click droit
	Bool CloseMessage; ///< Permet de savoir si le message a été fermé pour sortir de la boucle
	s_SharedResources* SR; ///< Pointeur vers les resources
	ItemID LastChoice; ///< Contient le dernier choix d'un menu ou d'un message affiché
	char* LastInput; ///< Contient l'input du dernier message
} MessageManager;


typedef MenuOfItems* MessageID;

/** @brief Initialise Le manager de Messages
 *
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] SR pointeur vers les resources partagées
 **/
void msgInit(MessageManager* MM, s_SharedResources* SR);

/** @brief Libère Le manager de Messages
 *
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 **/
void msgFree(MessageManager* MM);

/** @brief Prépare l'affichage d'un message
 *
 * Cette fonction est à être appelée avant d'ajouter des items et de faire un msgDisplay, msgGetChoice ou msgGetInput
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] Title Titre de la boîte du message. Peut être vide
 * @param[in] Nombre d'items que contient le message
 **/
void msgCreateMessage(MessageManager* MM, const char* Title, unsigned int ItemCount);

/** @brief Prépare l'affichage d'un menu de click droit
 *
 * Cette fonction est à être appelée avant d'ajouter des items et de faire un msgDisplay, msgGetChoice ou msgGetInput. Le fonctionnement est exactement le même que pour un Message (msgCreateMessage).
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] Nombre d'items que contient le menu
 **/
void msgCreateMenu(MessageManager* MM, unsigned int ItemCount);

/** @brief Ajoute un items au menu ou message qui est en train d'être crée por être affiché
 *
 * Cette fonction est à être appelée avant d'ajouter des items et de faire un msgDisplay, msgGetChoice ou msgGetInput et après avoir créer le message ou le menu avec msgCreateMessage ou msgCreateMenu
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] Text texte de l'item
 * @param[in] Type type de l'item
 * @param[in] Function fonction de l'item
 * @param[in] Data data de l'item
 * @return ID de l'item qui vient d'être ajouté. Cela est nécessaire si on veut changer ses paramètres. Surtout pour des items de type ITEM_VALUE ou ITEM_INPUT @see mnAddItem
 **/
ItemID msgAddItem(MessageManager* MM, const char* Text, ItemType Type, void (*Function)(void), void* Data);

/** @brief Ajoute un item au menu ou message qui est en train d'être crée por être affiché
 *
 * Cette fonction est à être appelée avant d'ajouter des items et de faire un msgDisplay, msgGetChoice ou msgGetInput et après avoir créer le message ou le menu avec msgCreateMessage ou msgCreateMenu. Un item crée ainsi est de type ITEM_BUTTON.
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] Text texte de l'item
 * @param[in] Function fonction de l'item
 * @param[in,out] Arg argument qui sera passé à la fonction. Normalement c'est une structure de données
 * @return ID de l'item qui vient d'être ajouté. Cela est nécessaire si on veut changer ses paramètres. Surtout pour des items de type ITEM_VALUE ou ITEM_INPUT @see mnAddItemWithArg
 **/
ItemID msgAddItemWithArg(MessageManager* MM, const char* Text, void (*Function)(void*), void* Arg);

/** @brief Ajoute un item au menu ou message qui est en train d'être crée por être affiché. Cet item ferme le message ou menu
 *
 * Cette fonction est à être appelée avant d'ajouter des items et de faire un msgDisplay, msgGetChoice ou msgGetInput et après avoir créer le message ou le menu avec msgCreateMessage ou msgCreateMenu. Un item crée ainsi est de type ITEM_BUTTON est contient une fonction qui permet de fermer le message.
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in] Text texte de l'item
 * @return ID de l'item qui vient d'être ajouté. Cela est nécessaire si on veut changer ses paramètres. Surtout pour des items de type ITEM_VALUE ou ITEM_INPUT @see mnAddItem
 **/
ItemID msgAddCloseItem(MessageManager* MM, const char* Text);

/** @brief Affiche le message/menu crée à l'écran et attend la réponse de l'utilisateur. Cette fonction est bloquante.
 *
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in,out] win Fenêtre où se fait l'afichage
 * @param[in] ViewX Position x de la vue
 * @param[in] ViewY Position y de la vue
 * @param[in] ViewWidth Longueur de la vue
 * @param[in] ViewHeight Largeur de la vue
 **/
void msgDisplay(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

/** @brief Affiche le message/menu crée à l'écran et attend la réponse de l'utilisateur. Cette fonction est bloquante.
 *
 * Si on échape avec un Esc alors l'ItemID qui est renvoyé est INVALID_ITEM_ID. Les ItemID sont des indices de 0 à n-1 où n est le nombre d'items dans le menu.
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in,out] win Fenêtre où se fait l'afichage
 * @param[in] ViewX Position x de la vue
 * @param[in] ViewY Position y de la vue
 * @param[in] ViewWidth Longueur de la vue
 * @param[in] ViewHeight Largeur de la vue
 * @return ID de l'item qui était sélectionné lors de la fermeture du message
 **/
ItemID msgGetChoice(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

/** @brief Affiche le message/menu crée à l'écran et attend la réponse de l'utilisateur. Cette fonction est bloquante.
 *
 * L'input renvoyé est le premier du menu. Il ne sert à rien de mettre plusieurs input dans un message ou menu car seule une valeur peut être retournée
 * @param[in,out] MM MessageManager auquel s'applique la fonction
 * @param[in,out] win Fenêtre où se fait l'afichage
 * @param[in] ViewX Position x de la vue
 * @param[in] ViewY Position y de la vue
 * @param[in] ViewWidth Longueur de la vue
 * @param[in] ViewHeight Largeur de la vue
 * @return ID de l'item qui était sélectionné lors de la fermeture du message
 **/
const char* msgGetInput(MessageManager* MM, sf::RenderWindow& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

/** @brief Donne accès au Menu du MessageManager pour pouvoir obtenir un item et faire des actions sur lui
 *
 * @code
 ItemID IID = msgAddItem(MM, "Item", ITEM_INPUT, NULL, NULL);
 mniSetInput(mnGetItem(msgGetMenu(MM), 0, IID), "Texte par défaut");
 @endcode
 * @param[in,out]  MM MessageManager auquel s'applique la fonction
 * @return Pointeur vers le Menu
 **/
Menu* msgGetMenu(MessageManager* MM);

//Bool msgCanDisplay(MessageManager* MM);


/**@}**/
#endif
