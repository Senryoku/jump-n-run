#ifndef _MENUITEM_H
#define _MENUITEM_H

#include <Core/DynArr.h>
#include <Core/Vec2.h>
#include <Core/Tools.h>
#include <string.h>
#include <string>

/** @defgroup menu Menu
 *@{
 */

typedef unsigned char ItemType;
typedef unsigned char ItemDirection; /* gauche ou droite pour incrémenter des valeurs */

#define ITEM_BUTTON 0x00 /**< un boutton classique */
#define ITEM_INPUT 0x01 /**< On écrit dans cet item */
#define ITEM_CHECKBOX 0x02 /**< Booleen ex fullscreen */
#define ITEM_VALUE 0x03 /**< Plage de valeur ex 0-50 */
#define ITEM_LABEL 0x04 /**< Un objet qui ne peut pas être sélectioné */
#define ITEM_INPUT_VALUE 0x05 /**< On écrit dans cet item que des chiffres ou un point */
#define ITEM_MENU_SWITCHER 0x06 /**< Change d'un menu à un autre */
#define ITEM_INPUT_MULTILINE 0x07 /**< On écrit dans cet item. on peut mettre des sauts à la ligne*/

#define MOVE_NONE 0x00
#define MOVE_LEFT 0x01
#define MOVE_RIGHT 0x02

struct SMenu;

/**
 * @brief structure qui défini un menu d'items
 */
typedef struct
{
	char* Text; /**< texte qui est écrit par le item **/
	float Zoom; /**< facteur de zoom avec lequel il est dessiné **/
	ItemType Type; /**< type d'item bouton, input, percentage **/
	void (*Function)(void); /**< Fonction de l'item **/
	void (*FunctionArg)(void*); /**< Fonction avec argument de l'item **/
	void* Data; /**< Data qui peut être modifié par l'utilisateur ou argument à passer à la fonction**/
	std::string* Str; /**< Permet de simplifier l'utilisation de ITEM_INPUT_VALUE **/
	unsigned char Prec; /**< Chiffres après la virgule pour l'affichage des ITEM_VALUE **/
	float Incr; /**< incrémentation pour les ITEM_VALUE **/
	float MinValue; /**< Valeur minimale pour les ITEM_VALUE **/
	float MaxValue; /**< Valeur maximale pour les ITEM_VALUE **/

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
 * @brief Constructeur
 * @param I MenuItem auquel s'applique la fonction
 * @param Text texte de l'item
 * @param Function fonction avec argument associée à l'item
 * @param Data pointeur vers des données
 */
void mniInitWithArg(MenuItem* I, const char* Text, void (*Function)(void*), void* Data);

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
 * @brief Mutateur du text d'un ITEM_INPUT
 * @param I MenuItem auquel s'applique la fonction
 * @param Text nouveau texte
 */
void mniSetInput(MenuItem* I, const char* Text);

/**
 * @brief Change la précision de l'affichage pour un ITEM_[INPUT_]VALUE
 * @param I MenuItem auquel s'applique la fonction
 * @param Prec Nouvelle précision
 */
void mniSetFloatPrecision(MenuItem* I, unsigned char Prec);

/**
 * @brief Donne la précision de l'affichage pour un ITEM_[INPUT_]VALUE
 * @param I MenuItem auquel s'applique la fonction
 * @param Prec Nouvelle précision
 */
unsigned char mniGetFloatPrecision(const MenuItem* I);


/**
 * @brief Accesseur de Text
 * @param I MenuItem auquel s'applique la fonction
 * @return Texte de l'item
 */
const char* mniGetText(const MenuItem* I);

/**
 * @brief Mutateur de Incr
 * @param I MenuItem auquel s'applique la fonction
 * @param Incr nouvelle valeur d'incrémentation
 */
void mniSetIncr(MenuItem* I, float Incr);

/**
 * @brief Accesseur de Incr
 * @param I MenuItem auquel s'applique la fonction
 * @param Incr nouvelle valeur d'incrémentation
 */
float mniGetIncr(const MenuItem* I);

/**
 * @brief Mutateur des limites pour un ITEM_VALUE
 * @param I MenuItem auquel s'applique la fonction
 * @param Min Valeur minimale
 * @param Max Valeur maximale
 */
void mniSetMinMaxValues(MenuItem* I, float Min, float Max);

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
 * @brief Accesseur au texte rentré pour les ITEM_INPUT et les ITEM_INPUT_VALUE
 * @param I MenuItem auquel s'applique la fonction
 * @return référence constante vers le texte
 */
const std::string& mniGetInput(const MenuItem* I);

/**
 * @brief Accesseur au texte rentré pour les ITEM_INPUT et les ITEM_INPUT_VALUE
 * @param I MenuItem auquel s'applique la fonction
 * @return pointeur constant ver le texte
 */
const char* mniGetInputChr(const MenuItem* I);


/**
 * @brief Lance la fonction associée à l'item
 * @param I MenuItem auquel s'applique la fonction
 */
void mniRunFunction(MenuItem* I);

/**
 * @brief gère l'entrée de l'item selon son type
 *
 * Cette fonction est à implémenter en dehors de ce module car elle est dépendante des libraries utilisées pour le fenêtrage
 * @param I MenuItem aquel s'applique la fonction
 *
 */
void mniUse(SMenu* M, MenuItem* I, Bool EnterPressed, ItemDirection IDir, unsigned char KeyCode, Bool Del);

void mniRegressionTest(void);

/**@}*/

#endif
