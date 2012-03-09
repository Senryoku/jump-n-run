#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "DynArr.h"
#include "Rigid.h"

/** @defgroup Polygon
 *
 * Polygone composé de Vertex reliés par des Rigid
 * Préfixe des méthodes : poly
 * @{
**/

typedef struct
{
	DynArr Vertices;
	DynArr Rigids;
} Polygon;

/** @brief Constructeur
 *
 * Ne créé pas de contraintes internes
 * @param nbVx Nombre de Vertex
 * @param ... Liste de Vertex*
**/
Polygon* newPolygon(unsigned int nbVx, ...);

/** @brief Initialise P
 *
 * @param P Le Polygon à initialiser
 * @param nbVx le nombre de Vertex
 * @param ... Liste de Vertex*
**/
void polyInit(Polygon* P, unsigned int nbVx, ...);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices, uniquement les contraintes créées
 * par le constructeur !
**/
void delPolygon(Polygon* P);


/** @}
**/

#endif
