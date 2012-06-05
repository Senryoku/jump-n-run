#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <Physics/Polygon.h>
#include <Physics/World.h>
#include "Animation.h"
#include <SFML/System/Clock.hpp>

/** @defgroup Player Player
 *
 * Décrit un joueur
 * Préfixe des méthodes : pl
  * @{
 **/

#define DIR_LEFT 0x00
#define DIR_RIGHT 0x01

#define PL_NOSTATE		0x00000000
#define PL_ON_GROUND	0x00000001
#define PL_HAS_SUPPORT	0x00000002
#define PL_FALLING_R	0x00000004
#define PL_FALLING_L	0x00000008
#define PL_FACEPLANT	0x00000010
#define PL_FALLING 		0x00000020
#define PL_GRABL		0x00000040
#define PL_GRABR		0x00000080
#define PL_GOING_R		0x00000100
#define PL_GOING_L		0x00000200
#define PL_UPSIDEDOWN	0x00000400

typedef unsigned char PlayerDirection;

struct s_SharedResources;

typedef struct SPlayer
{
	Polygon* Shape;
	Vertex* VxUL, *VxUR, *VxDL, *VxDR;
	/* char* Texture */

	PlayerDirection Dir;
	
	sf::Clock timer;

	Rigid* GrabR;
	Rigid* GrabL;
	//Vertex* Neck, *HeadLeft, *HeadRight, *Base, *LeftArm1, *LeftArm2, *RightArm1, *RightArm2, *LeftLeg1, *LeftLeg2, *RightLeg1, *RightLeg2;
	Vertex* vxBodyParts[12];
	Polygon *BodyPolygons[10];
	Rigid* BodyRigids[12];
	Vertex* VxBalance;
	Elastic* ElBalance;

	Vec2 Speed;
	Vec2 Normal, GroundVec;
	Vec2 Center, PrevCenter;
	Vec2 ULPos, URPos, DLPos, DRPos;
	float GroundAngle;
	Bool Jumping;
	float JumpTimer;
	Vec2 JumpVec;

	/* DÈfini si une partie de la BB est en collision */
	CollisionInfo VxULStatus, VxURStatus, VxDRStatus, VxDLStatus,
		RdUStatus, RdRStatus, RdDStatus, RdLStatus;
	unsigned int State;
	
	Bool IsFree; ///< c'est un ragdoll ou pas
	AnimPositions Positions;
	AnimAngles Angles;
	Animation* aniJump, *aniRun, *aniFall, *aniHello, *aniStand;
	char SndFoot[2][20];
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
/** @brief Corrige la position du joueur
 *
 * Ne modifie pas la vitesse actuelle
 * @param P Joueur
 * @param V Vec2, Vecteur de dÈplacement
**/
void plCorrectPosition(Player* P, Vec2 V);
/** @brief Affecte une forme au joueur
 *
 * @param P Joueur
 * @param Shape Polygon servant aux collisions
**/
void plSetShape(Player* P, Polygon* Shape);

/** @brief Mise à jour du joueur (animations)
 *
 * @param P Joueur
 * @param SR Resources partagées
**/
void plUpdate(Player* P, s_SharedResources* SR);

/** @brief Mise à jour du joueur (Physique)
 *
 * @param P Joueur
 * @param W Monde physique dans lequel Èvolue le joueur
 **/
void plPhysics(Player* P, World* W);

/** @brief DÈplacement vers la droite
 *
 * @param P Joueur
**/
void plMoveR(Player* P);
/** @brief DÈplacement vers la gauche
 *
 * @param P Joueur
**/
void plMoveL(Player* P);
/** @brief Rotation Droite
 *
 * @param P Joueur
**/
void plRotateR(Player* P);
/** @brief Rotation Gauche
 *
 * @param P Joueur
**/
void plRotateL(Player* P);
/** @brief Saut
 *
 * @param P Joueur
 * @param[in,out] SR Resources partagées
**/
void plJump(Player* P, s_SharedResources* SR);

/** @brief Reset du Saut
 *
 * @param P Joueur
**/
void plResetJump(Player* P);
/** @brief Aide le joueur ‡ se relever
 *
 * @param P Joueur
**/
void plGetUp(Player* P);

/** @brief Attrape un vertex de la "main" droite
 *
 * @param P Joueur
 * @param W Monde physique dans lequel Èvolue le joueur
 * @param MouseX Position de la souris dans le Monde
 * @param MouseY Position de la souris dans le Monde
**/
void plGrabR(Player* P, World* W, float MouseX, float MouseY);
/** @brief Rel‚che un vertex de la "main" droite
 *
 * @param P Joueur
 * @param W Monde physique dans lequel Èvolue le joueur
**/
void plReleaseR(Player* P, World* W);
/** @brief Attrape un vertex de la "main" gauche
 *
 * @param P Joueur
 * @param W Monde physique dans lequel Èvolue le joueur
 * @param MouseX Position de la souris dans le Monde
 * @param MouseY Position de la souris dans le Monde
**/
void plGrabL(Player* P, World* W, float MouseX, float MouseY);
/** @brief Rel‚che un vertex de la "main" gauche
 *
 * @param P Joueur
 * @param W Monde physique dans lequel Èvolue le joueur
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

AnimAngles* plGetAnimAnglesState(Player* P);

AnimPositions* plGetAnimPositionsState(Player* P);

/** @}
**/

#endif
