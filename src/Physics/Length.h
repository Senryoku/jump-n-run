#ifndef _LENGHT_H
#define _LENGHT_H

#include "Physics/Vertex.h"

/** @defgroup Lenght
 *
 * Liaison Lenght entre deux Vertex
 * Attention ! Des liaisons de longueur nulle donneront des résultats
 * assez aléatoire (pas de plantage mais un comportement peu réaliste...)
 * Préfixe des méthodes : rd
 * @todo Test de régression complet
 * @{
 **/

typedef struct
{
	Vertex* V1;
	Vertex* V2;
	float MinLenght, MaxLenght; /**< Longueur à l'équilibre **/
} Lenght;


/** @brief Initialise R
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
 **/
void lnInit(Lenght* R, Vertex* V1, Vertex* V2, float min, float max);

/** @brief Résout la contrainte
 *
 * @param R Contrainte Lenghte à résoudre
 **/
void lnResolve(Lenght* R);


/** @}
 **/

#endif
