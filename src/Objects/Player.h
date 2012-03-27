#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Physics/Polygon.h"

/** @defgroup Player
 *
 * Décrit un joueur
 * Préfixe des méthodes : pl
 **/

typedef struct
{
	Polygon* Shape;
	/* char* Texture */
} Player;

Player* newPlayer();

void plInit(Player* P);

void plFree(Player* P);

void delPlayer(Player* P);

void plSetPosition(Player*, Vec2);
void plSetShape(Player* P, Polygon* Shape);

void plUpdate(Player* P);

void plMoveR(Player* P);
void plMoveL(Player* P);
void plJump(Player* P);

/** @}
**/

#endif
