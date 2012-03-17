#ifndef _ELASTIC_H_
#define _ELASTIC_H_

#include "Physics/Vertex.h"

/** @defgroup Elastic
 *
 * Liaison élastique entre deux Vertex, implémente la loi de Hooke
 * Attention ! Des liaisons de longueur nulle donneront des résultats
 * assez aléatoire (pas de plantage mais un comportement peu réaliste...)
 * Préfixe des méthodes : elastic
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
**/
Elastic* newElastic(Vertex* V1, Vertex* V2, float L);


/** @brief Initialise R
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
**/
void elasticInit(Elastic* R, Vertex* V1, Vertex* V2, float L);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices
**/
void delElastic(Elastic* R);

/** @brief Résout la contrainte
 *
 * @param R Contrainte Elastice à résoudre
**/
void elasticResolve(Elastic* R);

/** @brief Retourne le vecteur formé par les deux points de la contrainte
 *
**/
Vec2 elasticVector(Elastic* R);

/** @brief Accesseur de V1
 *
**/
Vertex* elasticGetV1(Elastic* R);

/** @brief Accesseur de V2
 *
**/
Vertex* elasticGetV2(Elastic* R);

/** @}
**/

#endif

