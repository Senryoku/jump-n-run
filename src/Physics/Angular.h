#ifndef _ANGULAR_H
#define _ANGULAR_H

#include "Vertex.h"


typedef struct
{
	Vertex* C; /**< Centre **/
	Vertex* S; /**< Maître **/
	Vertex* M; /**< Esclave (Slave) **/
	float Lenghts; /**< produit des longueurs de CS et CM **/
	Vec2 CS;
	Vec2 CM;
	Vec2 MinPos, MaxPos;
	float MinAng; /**< Angle minimal **/
	float MaxAng; /**< Angle maximal **/
} Angular;

/** @brief Initialise la contrai nte d'angle
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
 **/
void angInit(Angular* A, Vertex* C, Vertex* M, Vertex* S, float MinAng, float MaxAng);

/** @brief Fait une mise à jour des longueurs (utilisé pour le calcul de l'angle)
 *
 **/
void angUpdateLenghts(Angular* A);

void angResolve(Angular* A);

#endif