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
 * @param DA Le DynArr à modifier
**/
void daReserve(DynArr* DA, unsigned int newCapa);

/** @brief Ajoute un élément en fin de tableau
 *
 * Modifie la capacité si nécéssaire
 * @param DA Le DynArr à modifier
 * @param ptr Elément à ajouter
**/
void daAdd(DynArr* DA, void* ptr);

/** @brief Supprime l'élément à la position P
 *
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daDel(DynArr* DA, unsigned int Pos);

/** @brief Supprime l'élément à la position P
 *
 * Ne conserve pas l'ordre des objets
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daFastDel(DynArr* DA, unsigned int Pos);

/** @brief Retourne l'élément en position Pos
 *
 * @param DA Le DynArr ) accéder
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

