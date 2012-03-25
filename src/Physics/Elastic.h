#ifndef _ELASTIC_H_
#define _ELASTIC_H_

#include "Physics/Vertex.h"

/** @defgroup Elastic
 *
 * Liaison élastique entre deux Vertex, implémente la loi de Hooke
 * Attention ! Des liaisons de longueur nulle donneront des résultats
 * assez aléatoire (pas de plantage mais un comportement peu réaliste...)
 * Préfixe des méthodes : el
 * @todo Test de régression complet
 * @{
**/

typedef struct
{
	Vertex* V1;
	Vertex* V2;
	float Length; /**< Longueur à l'équilibre **/
	float Spring; /**< Constante de Ressort **/
} Elastic;

/** @brief Constructeur
 *
 * @param V1 Premier Vertex
 * @param V2 Deuxième Vertex
 * @param L Longueur à l'équilibre
 * @param S Constante de Ressort
**/
Elastic* newElastic(Vertex* V1, Vertex* V2, float L, float S);


/** @brief Initialise E
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
 * @param E Contrainte à initialiser
 * @param V1 Premier Vertex
 * @param V2 Deuxième Vertex
 * @param L Longueur à l'équilibre
 * @param S Constante de Ressort
**/
void elInit(Elastic* E, Vertex* V1, Vertex* V2, float L, float S);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices
**/
void delElastic(Elastic* E);

/** @brief Résout la contrainte
 *
 * @param E Contrainte Elastic à résoudre
**/
void elResolve(Elastic* E);

/** @brief Retourne le vecteur formé par les deux points de la contrainte
 *
**/
Vec2 elVector(Elastic* E);

/** @brief Accesseur de V1
 *
**/
Vertex* elGetV1(const Elastic* E);

/** @brief Accesseur de V2
 *
**/
Vertex* elGetV2(const Elastic* E);

/** @brief Mutateur de V1
 *
**/
void elSetV1(Elastic* E, Vertex* V);

/** @brief Mutateur de V2
 *
**/
void elSetV2(Elastic* E, Vertex* V);

/** @brief Mutateur de Length
 *
**/
void elSetLength(Elastic* E, float newLength);

/** @brief Mutateur de Spring
 *
**/
void elSetSpring(Elastic* E, float newSpring);

/** @}
**/

#endif

