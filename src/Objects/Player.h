#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Physics/Polygon.h"
#include "Physics/World.h"

/** @defgroup Player
 *
 * D�crit un joueur
 * Pr�fixe des m�thodes : pl
 **/

typedef struct
{
	Polygon* Shape;
	Vertex* Stable;
	/* char* Texture */

	Elastic* Grab;
	Vertex* Neck, *HeadLeft, *HeadRight, *Base, *LeftArm1, *LeftArm2, *RightArm1, *RightArm2, *LeftLeg1, *LeftLeg2, *RightLeg1, *RightLeg2;

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

void plGrab(Player* P, World* W, float MouseX, float MouseY);
void plRelease(Player* P, World* W);

/**
 @brief Cr�e les vertex du corps du joueur
 @param P Player auquel s'applique la fonction
 @param W World o� ajouter les vertex ainsi cr�es
 */
void plCreateVertex(Player* P, World* W);

/**
 @brief Cr�e les Rigides enre les vertices du coprs du Player
 
 @code
 plCreateVertex(Player, World);
 plCreateRigids(Player, World);
 @endcode
 @param P Player auquel s'applique la fonction
 @param W World o� ajouter les vertex ainsi cr�es
 */
void plCreateRigids(Player* P, World* W);

/**
 @brief Change la position du corps du joueur
 @param P Player auquel s'applique la fonction
 @param x position x
 @param y position y
 */
void plSetPosition(Player* P, float x, float y);

/** @}
**/

#endif
