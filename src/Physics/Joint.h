#ifndef _JOINT_H
#define _JOINT_H

/**
 * @brief contrainte angulaire spécifique à une articulation telle que le bras ou la jambe: minAng=190º et maxAng=360º (dans le sens positif)
 * @param
 * @return
 */

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
	float Length;
	float MinAng; /**< Angle minimal **/
	float MaxAng; /**< Angle maximal **/
	float MinLenght, MaxLenght; /**< Longueur à l'équilibre **/
	float MinOrthoDot, MaxOrthoDot; /**< Produit scalaire de CS avec l'orthogonal à MC **/
	float MinDot, MaxDot;
} Joint;

/** @brief InitiJoint
 lise la contrai nte d'angle
 *
 * N'agit pas sur les Vertices, ils ne peuvent cependant
 * être passé en const (warnings)
 **/
void jnInit(Joint* J, Vertex* C, Vertex* M, Vertex* S);


void jnResolve(Joint* J);


#endif