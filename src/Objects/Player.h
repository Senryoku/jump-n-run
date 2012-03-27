#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Physics/Polygon.h"

/** @defgroup Player
 *
 * D�crit un joueur
 * Pr�fixe des m�thodes : pl
 **/

typedef struct
{
	Polygon* Shape;
	/* char* Texture */

	Bool OnGround;
	Vec2 GroundNormal;
} Player;

Player* newPlayer();

void plInit(Player* P);

void plFree(Player* P);

void delPlayer(Player* P);

/* Accesseurs */
Bool plGetOnGround(Player*);
Vec2 plGetGroundNormal(Player*);
Polygon* plGetShape(Player* P);
Vertex* plGetVxUL(Player* P);
Vertex* plGetVxUR(Player* P);
Vertex* plGetVxDR(Player* P);
Vertex* plGetVxDL(Player* P);
Rigid* plGetRdU(Player* P);
Rigid* plGetRdR(Player* P);
Rigid* plGetRdD(Player* P);
Rigid* plGetRdL(Player* P);

/* Mutateurs */
void plSetOnGround(Player*, Bool);
void plSetGroundNormal(Player*, Vec2);
void plSetPosition(Player*, Vec2);
void plSetShape(Player* P, Polygon* Shape);

void plUpdate(Player* P);

void plMoveR(Player* P);
void plMoveL(Player* P);
void plJump(Player* P);
void plGetUp(Player* P);

/** @}
**/

#endif
