#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <Physics/Polygon.h>
#include <Physics/World.h>

/** @defgroup Player
 *
 * DÈcrit un joueur
 * PrÈfixe des mÈthodes : pl
 **/

#define DIR_LEFT 0x00
#define DIR_RIGHT 0x01

typedef unsigned char PlayerDirection;

typedef struct SPlayer
{
	Polygon* Shape;
	Vertex* VxUL, *VxUR, *VxDL, *VxDR;
	/* char* Texture */

	PlayerDirection Dir;

	Rigid* GrabR;
	Rigid* GrabL;
	Vertex* Neck, *HeadLeft, *HeadRight, *Base, *LeftArm1, *LeftArm2, *RightArm1, *RightArm2, *LeftLeg1, *LeftLeg2, *RightLeg1, *RightLeg2;
	Vertex* VxBalance;
	Elastic* ElBalance;

	Vec2 Speed;
	Vec2 Normal, GroundVec;
	Vec2 Center;
	Vec2 ULPos, URPos, DLPos, DRPos;
	float GroundAngle;
	Bool Jumping;
	float JumpTimer;
	Vec2 JumpVec;

	/* DÈfini si une partie de la BB est en collision */
	CollisionInfo VxULStatus, VxURStatus, VxDRStatus, VxDLStatus,
		RdUStatus, RdRStatus, RdDStatus, RdLStatus;
	Bool OnGround;
} Player;

/** @brief Constructeur
**/
Player* newPlayer(World* W);

/** @brief Initialisation
**/
void plInit(Player* P, World* W);

/** @brief LibÈration
**/
void plFree(Player* P);

/** @brief Destructeur
**/
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
void plCorrectPosition(Player*, Vec2);
void plSetShape(Player* P, Polygon* Shape);

/** @brief Mise ‡ jour du joueur (Physique, Informations...)
**/
void plUpdate(Player* P, World* W);

/** @brief DÈplacement vers la droite
**/
void plMoveR(Player* P);
/** @brief DÈplacement vers la gauche
**/
void plMoveL(Player* P);
/** @brief Rotation Droite
**/
void plRotateR(Player* P);
/** @brief Rotation Gauche
**/
void plRotateL(Player* P);
/** @brief Saut
**/
void plJump(Player* P);
/** @brief Reset du Saut
**/
void plResetJump(Player* P);
/** @brief Aide le joueur ‡ se relever
**/
void plGetUp(Player* P);

/** @brief Attrape un vertex de la "main" droite
**/
void plGrabR(Player* P, World* W, float MouseX, float MouseY);
/** @brief Rel‚che un vertex de la "main" droite
**/
void plReleaseR(Player* P, World* W);
/** @brief Attrape un vertex de la "main" gauche
**/
void plGrabL(Player* P, World* W, float MouseX, float MouseY);
/** @brief Rel‚che un vertex de la "main" gauche
**/
void plReleaseL(Player* P, World* W);


/**
 @brief CrÈÈ les vertex du corps du joueur
 @param P Player auquel s'applique la fonction
 @param W World où ajouter les vertex ainsi crées
 */
void plCreateVertex(Player* P, World* W);

/**
 @brief CrÈÈ les Rigides enre les vertices du coprs du Player

 @code
 plCreateVertex(Player, World);
 plCreateRigids(Player, World);
 @endcode
 @param P Player auquel s'applique la fonction
 @param W World où ajouter les vertex ainsi crées
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
