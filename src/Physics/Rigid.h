#ifndef _RIGID_H_
#define _RIGID_H_

#include "Vertex.h"

/** @defgroup Rigid Rigid
 *
 * Liaison Rigid entre deux Vertex
 * Attention ! Des liaisons de longueur nulle donneront des résultats
 * assez aléatoire (pas de plantage mais un comportement peu réaliste...)
 * Préfixe des méthodes : rd
 * @{
**/

typedef struct
{
	Vertex* V1; /**< Premier Vertex **/
	Vertex* V2; /**< Deuxième Vertex **/
	float Length; /**< Longueur à l'équilibre **/
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
Vertex* rdGetV1(const Rigid* R);

/** @brief Accesseur de V2
 *
**/
Vertex* rdGetV2(const Rigid* R);

/** @brief Mutateur de Length
 *
**/
void rdSetLength(Rigid* R, float newLength);

/** @brief Accesseur de Length
 *
 **/
float rdGetLength(const Rigid* R);

/** @brief Test de Régression
 *
**/
void rdRegressionTest();


/** @brief Donne la distance au carré d'un point à la droite
 * @param[in] E Elastic auquel s'applique la fonction
 * @param[in] V Position du point
 * @return Distance au carré
 **/
float rdGetSquaredDistanceToPoint(const Rigid* R, Vec2 V);

/** @}
**/

#endif
