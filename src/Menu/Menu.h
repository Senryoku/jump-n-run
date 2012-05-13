#ifndef _MENU_H
#define _MENU_H

#include "MenuItem.h"
#include "MenuOfItems.h"
#include <SFML/Graphics.hpp>

/**
 * @defgroup menu Menu
 * Structure permettant d'avoir une interface de menus
 * Un menu contient en réalité plusieurs menus, chaque menu contient des items et chaque items est lié à une fonction.
 * préfixe: mn
 * @{
 */


typedef unsigned short MenuID;
typedef unsigned short MenuType;

#define MENU_TYPE_DEFAULT 0x00
#define MENU_TYPE_MESSAGE 0x01

/**
 * @brief Structure définissant un menu
 *
 * Ce module permet de créer des Menu avec des effets fluides de zoom. Il existe différent types d'items qui peuvent y être ajoutés. Voici un exemple d'utilisation du module
 * @code
 Menu * M; //À instancier
 //On initialise le menu
 mnInit(M);
 mnSetItemSelectedZoomFactor(M, 1.f);
 mnSetItemNormalZoomFactor(M, 0.75f);
 
 MenuID Main;
 //On ajoute un menu avec 3 items
 Main = mnAddMenu(M, "Main Menu", 3);
 mnAddItem(M, Main, "Play", ITEM_MENU_SWITCHER, NULL, 1); //1 Sera le menu de Play
 mnAddItem(M, Main, "Options", ITEM_MENU_SWITCHER, NULL, 2); //2 sera le menu des options
 //mnAddItem(M, Main, "Exit", ITEM_BUTTON, &Exit, NULL); //Exit() est une fonction
 
 mnAddMenu(M, "Play", 5);
 mnAddItem(M, 1, "I'm a label", ITEM_LABEL, NULL, NULL); //Les label peuvent ètre aussi utilisés comme séparateur si on ne met pas de texte
 float value; ItemID ID;
 ID = mnAddItem(M, 1, "Change my value", ITEM_VALUE, NULL, &value);
 mniSetIncr(mnGetItem(M, 1, ID), 5.f); // la valeur est incrémenté de 5 ou -5 avec les flèches
 mniSetMinMaxValues(mnGetItem(M, 1, ID), 0.f, 100.f); //On limite la valeur
 mnAddItem(M, 1, "Input", ITEM_INPUT, NULL, NULL); //il alloue l'espace nécessaire pour l'input
 //Celui ci ne peut avoir qu'une valeur réelle
 mnAddItem(M, 1, "Input a real", ITEM_INPUT_VALUE, NULL, NULL); //il alloue l'espace nécessaire pour l'input
 mnAddItem(M, 1, "Back", ITEM_MENU_SWITCHER, NULL, Main);
 
 // etc

 // Main Loop 
//MousePosition est un Vec2 contenant la position du curseur relative à la fenêtre
//OutofWindowPosition est un Vec2 contenant la position (relative à la fenêtre) ou doit aller le menu lors des transitions entre menu ou pour le cacher
mnUpdate(M, MousePosition, OutofWindowPosition);


 // Fin 
mnFree(M);
 @endcode
 * @see MenuItem
 * @see MenuOfItems
 */

typedef struct SMenu
{
	DynArr* Menus; /**< Tableau contenant tous les menus **/
	MenuID CurrentMenu; /**< Menu dans lequel on se trouve à l'instant présent **/
	MenuID PreviousMenu; /**< Menu ou on se trouvais avant, utilisé pour les transitions et pour dessiner **/
	float MenuY; /**< position x ou est déssiné le menu **/
	float MenuX; /**< position y ou est déssiné le menu **/
	float SubAnim; /**< Subimage de l'animation du fond **/
	float Friction; /**< Friction du mouvement du menu **/
	float Force; /**< Vitesse du mouvement du mouvement **/
	float spd[2]; /**< variable utilisé par wobble **/
	float ItemHeight; /**< largeur d'un item non selectionné **/
	float ItemSelectedZoomFactor; /**< Zoom d'un item quand il est sélectionné **/
	float ItemNormalZoomFactor; /**< Zoom d'un item quand il n'est pas sélectionné **/
	Bool Active; /**< Dit si le menu est active et si on doit gérer ou pas les entrées. Utilisé lorsqu'on affiche des messages par exemple **/
	Bool Hide; /**< permet de cacher le menu et le rendre inactif **/
	void* Arg; /**< Ici sont stockées les donnés utilisées lors de l'appel à une fonction avec argument. Si un malloc a ´té fait il faudrait que la fonction qui est appelé avec l'Item fasse un free **/
	MenuType Type; /**< type du menu, permet de faire des animations diférentes **/
	float MessageScale; /**< un scale pour les menus qui apparaissent avec un click **/
} Menu;


/** @brief Initialise un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 **/
void mnInit(Menu* M);

/** @brief Libère un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 **/
void mnFree(Menu* M);

/** @brief détruit un menu et libère la place occupée par celui-ci
 *
 * ATTENTION! Cette fonction fausse les MenuID qui ont été retournées avant l'appel! Elle est utilisée par Message Pour libérer la place utiliser et car on n'a plus besoin de MenuID
 * @param M Menu à laquelle s'applique la fonction
 * @param MID ID du menu à supprimer
 **/
void mnRemoveMenu(Menu* M, MenuID MID);

/** @brief détruit un menu et libère la place occupée par celui-ci
 *
 * ATTENTION! Cette fonction fausse les MenuID qui ont été retournées avant l'appel! Elle est utilisée par Message Pour libérer la place utiliser et car on n'a plus besoin de MenuID
 * @param M Menu à laquelle s'applique la fonction
 * @param moi pointeur du menu à supprimer
 **/
void mnRemoveMenu(Menu* M, MenuOfItems* moi);

/** @brief Crée un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Text texte du menu à ajouter (peut être nul)
 * @param ItemCount Nombre d'item que va contenir le menu
 * @return ID du menu ajouté (c'est l'indice du menu dans le tableau)
 **/
MenuID mnAddMenu(Menu* M, const char* Text, unsigned short ItemCount);

/** @brief Crée un item pour un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param MID ID du menu auquel on ajoute l'item
 * @param Text texte de l'item
 * @param Type Type de l'item
 * @param Function fonction associée à l'item
 * @param Data pointeur vers des données
 * @return ID de l'item ajouté (c'est l'indice de l'item dans le tableau du menu)
 **/
ItemID mnAddItem(Menu* M, MenuID MID, const char* Text, ItemType Type, void (*Function)(void), void* Data);

/** @brief Crée un item de ITEM_MENU_SWITCHER pour un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param MID ID du menu auquel on ajoute l'item
 * @param Text texte de l'item
 * @param MIDTo MenuID du menu vers lequel renvoie cet item
 * @return ID de l'item ajouté (c'est l'indice de l'item dans le tableau du menu)
 **/
ItemID mnAddItemMenuSwitcher(Menu* M, MenuID MID, const char* Text, MenuID MIDTo);

/** @brief Mutateur de ItemHeight
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param ItemHeight largeur d'un item dans un menu
 **/
void mnSetItemHeight(Menu* M, float ItemHeight);

/** @brief Accesseur de ItemHeight
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return ItemHeight des menus
 **/
float mnGetItemHeight(const Menu* M);

/** @brief Mutateur de ItemSelectedZoomFactor
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param ItemSelectedZoomFactor facteur de zoom pour les items sélectionnés
 **/
void mnSetItemSelectedZoomFactor(Menu* M, float ItemSelectedZoomFactor);

/** @brief Accesseur de ItemSelectedZoomFactor
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return ItemSelectedZoomFactor des menus
 **/
float mnGetItemSelectedZoomFactor(const Menu* M);

/** @brief Mutateur de ItemNormalZoomFactor
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param ItemNormalZoomFactor facteur de zoom pour les items non sélectionnés
 **/
void mnSetItemNormalZoomFactor(Menu* M, float ItemNormalZoomFactor);

/** @brief Accesseur de ItemNormalZoomFactor
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return ItemNormalZoomFactor des menus
 **/
float mnGetItemNormalZoomFactor(const Menu* M);

/** @brief Accesseur de Force
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return Force des animations des menus
 **/
float mnGetForce(const Menu* M);

/** @brief Accesseur de Friction
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return Friction des animations des menus
 **/
float mnGetFriction(const Menu* M);

/** @brief Mutateur de Force
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Force des animations des menus
 **/
void mnSetForce(Menu* M, float Force);

/** @brief Mutateur de Friction
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Friction des animations des menus
 **/
void mnSetFriction(Menu* M, float Friction);

/** @brief Mutateur de Hide
 *
 * Cette fonction permet de cacher le menu et le rendre inactif (Active==0)
 * @param M Menu à laquelle s'applique la fonction
 * @param Hide Vrai pour cacher le menu
 **/
void mnSetHide(Menu* M, Bool Hide);

/** @brief Accesseur de Hide
 * @param M Menu à laquelle s'applique la fonction
 * @return Vrai si le menu est caché
 **/
Bool mnGetHide(const Menu* M);

/** @brief Dit si le menu est encore sur l'écran selon sa taille et sa position
 * @param M Menu à laquelle s'applique la fonction
 * @return Vrai si le menu est encore sur l'écran
 **/
Bool mnIsVisible(const Menu* M);

/** @brief Mutateur de Active
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Active valeur d'active
 **/
void mnSetActive(Menu* M, Bool Active);

/** @brief Accesseur de Active
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return le menu est-il actif pour les input (changements de l'item sélectioné et lancement de fonctions)
 **/
Bool mnGetActive(const Menu* M);

/** @brief Fait la mise à jour du menu
 *
 * Cette fonction va faire le wobble sur tout et bouger vers le menu selectionné si ce n'est pas celui qui est affiché
 * @param MenuPos Position vers laquelle va le menu actif
 * @param Outpos Position vers laquelle va un menu quand il n'est plus sélectionné
 **/
void mnUpdate(Menu* M, Vec2 MenuPos, Vec2 OutPos);

/** @brief Mutateur de CurrentMenu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param MID ID du menu auquel aller
 **/
void mnGoToMenu(Menu* M, MenuID MID);

/** @brief Accesseur de CurrentMenu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return L'ID du menu actuel
 **/
MenuID mnGetCurrentMenuID(const Menu* M);

/** @brief Donne accès à un item
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param L'ID du menu 
 * @param L'ID de l'item
 * @return pointeur vers l'item
 **/
MenuItem* mnGetItem(const Menu* M, MenuID MID, ItemID IID);

/** @brief Donne accès au menu qui est courament sur l'écran
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return Le menu actuel
 **/
MenuOfItems* mnGetCurrentMenu(const Menu* M);

/** @brief Donne accès à un menu selon son ID
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param MID MenuID du menu
 * @return Le menu actuel
 **/
MenuOfItems* mnGetMenuPtr(const Menu* M, MenuID MID);

/** @brief Donne accès aà l'item qui est selectionné
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return L'item actuel
 **/
MenuItem* mnGetCurrentItem(const Menu* M);

/** @brief Bouge le curseur dans le menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Direction direction dans laquelle bouger le curseur
 **/
void mnMoveCursor(Menu* M, MenuDirection Direction);

/** @brief Bouge le curseur dans le menu à un item en concret selon la positon du curseur
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param MousePos Position du curseur
 **/
void mnSetCursor(Menu* M, Vec2 MousePos);

/** @brief Donne la position du menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return La position du menu
 **/
Vec2 mnGetPosition(const Menu* M);

/** @brief Change la position du menu
 *
 * Cette fonction n'est utilie que pour les type MENU_TYPE_MESSAGE
 * @param M Menu à laquelle s'applique la fonction
 * @param Pos nouvelle position
 **/
void mnSetPosition(Menu* M, Vec2 Pos);

/** @brief Gère le menu selon les entrées
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Event Event utilisé pour les entrées
 **/
void mnHandleEvent(Menu* M, const sf::Event& event);

/** @brief Donne la hauteur du menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return L'hauteur du menu
 **/
float mnGetHeight(const Menu* M);

/** @brief Accesseur de Arg
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return Données à être utilisées par un Item
 **/
void* mnGetArg(Menu* M);

/** @brief Accesseur de MenuScale
 *
 * @param M Menu à laquelle s'applique la fonction
 * @return MessageScale
 **/
float mnGetMessageScale(const Menu* M);

/** @brief Mutateur de Arg
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Arg Données à être utilisées par un Item
 **/
void mnSetArg(Menu* M, void* Arg);

/**@}*/
#endif