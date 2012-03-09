#ifndef _RIGID_H_
#define _RIGID_H_

#include "Vertex.h"

/** @defgroup Rigid
 *
 * Liaison Rigid entre deux Vertex
 * Préfixe des méthodes : rd
 * @{
**/

typedef struct
{
	Vertex* V1;
	Vertex* V2;
	float Length;
} Rigid;

/** @brief Constructeur
 *
 * @param V1 Premier Vertex
 * @param V2 Deuxième Vertex
 * @param L Longueur à l'équilibre
**/
Rigid* newRigid(const Vertex* V1, const Vertex* V2, float L);

/** @brief Initialise R
 *
 * N'agit pas sur les Vertices
**/
void rdInit(Rigid* R, const Vertex* V1, const Vertex* V2, float L);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices
**/
void delRigid(Rigid* R);

void rdResolve(Rigid* R);

/** @}
**/

#endif
