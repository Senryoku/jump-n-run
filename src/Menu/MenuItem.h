#ifndef _MENUITEM_H
#define _MENUITEM_H

#include <Core/DynArr.h>
#include <Core/Vec2.h>
#include <Core/Tools.h>
#include <string.h>

/** @defgroup menu Menu
 *@{
 */

typedef unsigned char ItemType;

#define ITEM_BUTTON 0x00 /**< un boutton classique */
#define ITEM_INPUT 0x01 /**< On ecrit dans cet item */
#define ITEM_CHECKBOX 0x02 /**< Booleen ex fullscreen */
#define ITEM_VALUE 0x03 /**< Plage de valeur ex 0-50 */
#define ITEM_LABEL 0x04 /**< Un objet qui ne peut pas être sélectioné */


/**
 * @brief structure qui défini un menu d'items
 */
typedef struct
{
	char* Text; /**< texte qui est écrit par le item **/
	float Zoom; /**< facteur de zoom avec lequel il est dessiné **/
	ItemType Type; /**< type d'item bouton, input, percentage **/
	void (*Function)(void); /**< Fonction de l'item **/
	void* Data; /**< Data qui peut être modifié par l'utilisateur **/
	
	
} MenuItem;

/**
 * @brief Constructeur
 * @param I MenuItem auquel s'applique la fonction
 * @param Text texte de l'item
 * @param Type type de l'item
 * @param Function fonction associée à l'item
 * @param Data pointeur vers des données
 */
void mniInit(MenuItem* I, const char* Text, ItemType Type, void (*Function)(void), void* Data);

/**
 * @brief Destructeur
 * @param I MenuItem auquel s'applique la fonction
 */
void mniFree(MenuItem* I);

/**
 * @brief Mutateur de Text
 * @param I MenuItem auquel s'applique la fonction
 * @param Text nouveau texte
 */
void mniSetText(MenuItem* I, const char* Text);

/**
 * @brief Accesseur de Text
 * @param I MenuItem auquel s'applique la fonction
 * @return Texte de l'item
 */
const char* mniGetText(const MenuItem* I);

/**
 * @brief Mutateur de Type
 * @param I MenuItem auquel s'applique la fonction
 * @param Type nouveau type
 */
void mniSetType(MenuItem* I, ItemType Type);

/**
 * @brief Accesseur de Type
 * @param I MenuItem auquel s'applique la fonction
 * @return Type de l'item
 */
ItemType mniGetType(const MenuItem* I);

/**
 * @brief Accesseur/Mutateur de Zoom
 *
 * Ici on obtient un pointeur car c'est ainsi qu'on va lire ET écrire sur la Data
 * @param I MenuItem auquel s'applique la fonction
 * @return pointeur vers Zoom de l'item
 */
float* mniGetZoom(MenuItem* I);

/**
 * @brief Accesseur/Mutateur de Data
 * @param I MenuItem auquel s'applique la fonction
 * @return pointeur vers Data de l'item
 */
void* mniGetData(MenuItem* I);

/**
 * @brief Lance la fonction associée à l'item
 * @param I MenuItem auquel s'applique la fonction
 */
void mniRunFunction(MenuItem* I);

/**@}*/

#endif