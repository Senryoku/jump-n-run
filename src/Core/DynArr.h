#ifndef _DYNARR_H_
#define _DYNARR_H_

#include <stdlib.h>

/** @defgroup DynArr
 *
 * Tableau dynamique de void*
 * Préfixe des méthodes : da
 * @todo Test de régression complet
 * @{
**/

typedef struct
{
	void* *First;
	unsigned int Capacity;
	unsigned int Size;
} DynArr;

/** @brief Renvoi un DynArr initialisé
 *
**/
DynArr da();

/** @brief Constructeur
 *
**/
DynArr* newDynArr();

/** @brief Initialise un DynArr
 *
 * @param DA Le DynArr à intialiser
**/
void daInit(DynArr* DA);

/** @brief Libère le tableau
 *
 * @param DA Le DynArr à libérer
**/
void daFree(DynArr* DA);

/** @brief Destructeur
 *
**/
void delDynArr(DynArr* DA);

/** @brief Redimensionne le tableau
 *
 * Si Capacity > newCapa, ne fait rien.
 * Complexité en O(n)
 * @param DA Le DynArr à modifier
**/
void daReserve(DynArr* DA, unsigned int newCapa);

/** @brief Ajoute un élément en fin de tableau
 *
 * Modifie la capacité si nécéssaire
 * Complexité en O(log n) (O(1) la pluspart du temps, O(n) dans le pire des cas)
 * @param DA Le DynArr à modifier
 * @param ptr Elément à ajouter
**/
void daAdd(DynArr* DA, void* ptr);

/** @brief Supprime l'élément à la position P
 *
 * Complexité en O(n - P)
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daDel(DynArr* DA, unsigned int Pos);

/** @brief Supprime l'élément à la position P d'une manière plus rapide
 *
 * Ne conserve pas l'ordre des objets (important pour certaines applications !)
 * Complexité en O(1)
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daFastDel(DynArr* DA, unsigned int Pos);

/** @brief Retourne l'élément en position Pos
 *
 * Complexité en O(1)
 * @param DA Le DynArr à accéder
 * @param Pos Position de l'entrée à renvoyer (0 - Size-1)
**/
void* daGet(const DynArr* DA, unsigned int Pos);

/** @brief Accesseur de la taille utilisée
 *
**/
unsigned int daGetSize(const DynArr* DA);

/** @brief Accesseur de la capacité
 *
**/
unsigned int daGetCapacity(const DynArr* DA);

#endif

