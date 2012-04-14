#ifndef _MENU_H
#define _MENU_H

#include "MenuItem.h"
#include "MenuOfItems.h"

/**
 * @defgroup menu Menu
 * Structure permettant d'avoir une interface de menus
 * Un menu contient en réalité plusieurs menus, chaque menu contient des items et chaque items est lié à une fonction.
 * préfixe: mn
 * @{
 */


typedef unsigned short MenuID;

/**
 * @brief Structure définissant un menu
 */

typedef struct 
{
	DynArr* Menus; /**< Tableau contenant tous les menus **/
	MenuID CurrentMenu; /**< Menu dans lequel on se trouve à l'instant présent **/
	MenuID PreviousMenu; /**< Menu ou on se trouvais avant, utilisé pour les transitions **/
	float MenuY; /**< position x ou est déssiné le menu **/
	float MenuX; /**< position y ou est déssiné le menu **/
	float spd[2]; /**< variable utilisé par wobble **/
	float ItemHeight; /**< largeur d'un item non selectionné **/
	float ItemSelectedZoomFactor; /**< Zoom d'un item quand il est sélectionné **/
	float ItemNormalZoomFactor; /**< Zoom d'un item quand il n'est pas sélectionné **/
	Bool Active; /**<  **/
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

/** @brief Libère un menu
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Text texte du menu à ajouter (peut être nul)
 * @param ItemCount Nombre d'item que va contenir le menu
 * @return ID du menu ajouté (c'est l'indice du menu dans le tableau)
 **/
MenuID mnAddMenu(Menu* M, const char* Text, unsigned short ItemCount);

/** @brief Libère un menu
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

/** @brief Mutateur de Active
 *
 * @param M Menu à laquelle s'applique la fonction
 * @param Active le menu est-il active? Les inputs n'auront aucun effet
 **/
void mnSetActive(Menu* M, Bool Active);

/** @brief Mutateur de Active
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

/**@}*/
#endif