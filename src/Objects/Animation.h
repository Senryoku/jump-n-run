#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <math.h>
#include <Core/DynArr.h>
#include <Core/Vec2.h>
#include <Core/Tools.h>
#include <Physics/Vertex.h>

/** @defgroup Animation Animation
 *
 * Permet d'animqer un squelette et donc le joueur.
 * Préfixe des méthodes : ani
 * @{
 **/

typedef unsigned char AnimType;
typedef unsigned short AnimTriggers; /**< Il faut utiliser les flags pour définir quels sont les parties du corps qui vont décider que l'animation doit passer à l'étape suivante **/

#define ANIM_ANGLES 0x00
#define ANIM_POSITIONS 0x01


/*#define ANIM_FREE 0x7fc00000 < Définie quand on ne tient pas en compte cette valeur. C'est un NaN. EDIT: Finalement ceci ne sert à rien car il considère que c'est un entier. De toutes façons NAN est définie dans math.h avec 
#if defined(__GNUC__)
#define NAN         __builtin_nanf("0x7fc00000") 
#else
#define NAN		__nan( )
#endif

**/

/* Flags pour décider qui va permettre l'animation d'avancer */

#define ANIM_NECK 0x0001
#define ANIM_LEFT_ARM1 0x0002
#define ANIM_LEFT_ARM2 0x0004
#define ANIM_RIGHT_ARM1 0x0008
#define ANIM_RIGHT_ARM2 0x0010
#define ANIM_LEFT_LEG1 0x0020
#define ANIM_LEFT_LEG2 0x0040
#define ANIM_RIGHT_LEG1 0x0080
#define ANIM_RIGHT_LEG2 0x0100
#define ANIM_HEAD 0x0200
#define ANIM_ALL_TRIGGERS 0xffff

enum BodyParts {
	bpNeck = 0,
	bpLeftArm1 = 1,
	bpLeftArm2 = 2,
	bpRightArm1 = 3,
	bpRightArm2 = 4,
	bpLeftLeg1 = 5,
	bpLeftLeg2 = 6,
	bpRightLeg1 = 7,
	bpRightLeg2 = 8,
	bpHeadLeft = 9, ///<c'est Head pour un polygones et headleft pour les vertex
	bpHeadRight = 10, ///< que pour les vertex
	bpBase = 11 ///<que pour les vertex
	};

typedef struct {
	Vec2 Positions[10];
	unsigned int CurrentState;
	Bool Ended;
	float Spd[20];
} AnimPositions;

typedef struct {
	float Angles[10];
	unsigned int CurrentState;
	Bool Ended;
	float Spd[10];
} AnimAngles;

void animAnglesStatesInit(AnimAngles* ang);
void animPositionsStatesInit(AnimPositions* pos);

struct SPlayer;

typedef struct {
	AnimType Type;
	AnimTriggers Triggers;
	unsigned char TriggerCount;
	DynArr* States;
	Bool Repeat;
	float Friction;
	float Force;
	float Diff; /**< Différence absolue qui permet de savoir quand on passe à l'animation suivante **/
} Animation;


void aniInit(Animation* A, AnimType Type, AnimTriggers Triggers, Bool Repeat);

void aniFree(Animation* A);

Animation* newAnimation(AnimType Type, AnimTriggers Triggers, Bool Repeat);

void delAnimation(Animation* A);

Bool aniLoadFromFile(Animation* A, const char* File);

void aniResetEnded(Animation* A);

Bool aniIsEnded(const Animation* A);

unsigned int aniGetStatesCount(const Animation* A);

unsigned int aniGetCurrentState(const Animation* A);

void aniSetCurrentState(Animation* A, unsigned int State);

void aniAddPositionState(Animation* A, Vec2 Head, Vec2 Neck, Vec2 LeftArm1, Vec2 LeftArm2, Vec2 RightArm1, Vec2 RightArm2, Vec2 LeftLeg1, Vec2 LeftLeg2, Vec2 RightLeg1, Vec2 RightLeg2);

void aniAddPositionState(Animation* A, const AnimPositions* Positions);

void aniAddAngleState(Animation* A, float Head, float Neck, float LeftArm1, float LeftArm2, float RightArm1, float RightArm2, float LeftLeg1, float LeftLeg2, float RightLeg1, float RightLeg2);

void aniAddAngleState(Animation* A, const AnimAngles* Angles);

void aniCountTriggers(Animation* A);

void aniUpdate(Animation* A, SPlayer* P, float Step);

void aniUpdateForCurrentState(Animation* A, SPlayer* P);

void aniSetForce(Animation* A, float Force);

float aniGetForce(const Animation* A);

void aniSetFriction(Animation* A, float Friction);

float aniGetFriction(const Animation* A);

//normalement cette fonction s'applique sur une animation qui a un seule état car si on modifie l'animation elle même c'ets comme si on animer le modèle...
void aniSetBodyPartAngle(Animation* A, AnimTriggers BodyPart, float Angle, unsigned int State = 0);

void aniSetBodyPartPosition(Animation* A, AnimTriggers BodyPart, Vec2 Pos, unsigned int State = 0);

///@}
 
#endif