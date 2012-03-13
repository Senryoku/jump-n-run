#ifndef _RIGID_H_
#define _RIGID_H_

#include "Physics/Vertex.h"

/** @defgroup Rigid
 *
 * Liaison Rigid entre deux Vertex
 * Préfixe des méthodes : rd
 * @todo Test de régression complet
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
Rigid* newRigid(Vertex* V1, Vertex* V2, float L);

/** @brief Initialise R
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
**/
void rdInit(Rigid* R, Vertex* V1, Vertex* V2, float L);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices
**/
void delRigid(Rigid* R);

/** @brief Résout la contrainte
 *
 * @param R Contrainte rigide à résoudre
**/
void rdResolve(Rigid* R);

/** @brief Retourne le vecteur formé par les deux points de la contrainte
 *
**/
Vec2 rdVector(Rigid* R);

/** @brief Accesseur de V1
 *
**/
Vertex* rdGetV1(Rigid* R);

/** @brief Accesseur de V2
 *
**/
Vertex* rdGetV2(Rigid* R);

/** @}
**/

#endif
