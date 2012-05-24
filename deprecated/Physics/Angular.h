#ifndef _ANGULAR_H
#define _ANGULAR_H

#include "Vertex.h"


typedef struct
{
	Vertex* C; /**< Centre **/
	Vertex* S; /**< Maître **/
	Vertex* M; /**< Esclave (Slave) **/
	Vec2 CS;
	Vec2 MC;
	Vec2 MCOrtho;
	Vec2 MinPos, MaxPos;
	float MinAng; /**< Angle minimal **/
	float MaxAng; /**< Angle maximal **/
	float MinLenght, MaxLenght; /**< Longueur à l'équilibre **/
	float MinOrthoDot, MaxOrthoDot; /**< Produit scalaire de CS avec l'orthogonal à MC **/
	float MinDot, MaxDot;
} Angular;

/** @brief Initialise la contrai nte d'angle
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
 **/
void angInit(Angular* A, Vertex* C, Vertex* M, Vertex* S, float MinAng, float MaxAng);


void angResolve(Angular* A);

#endif