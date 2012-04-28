#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <math.h>
#include <Core/DynArr.h>
#include <Core/Vec2.h>
#include <Core/Tools.h>
#include <Physics/Vertex.h>

/**
 * @brief 
 * @param
 * @return
 */

typedef unsigned char AnimType;
typedef unsigned short AnimTriggers; /**< Il faut utiliser les flags pour définir quels sont les parties du corps qui vont décider que l'animation doit passer à l'étape suivante **/

#define ANIM_ANGLES 0x00
#define ANIM_POSITIONS 0x01


#define ANIM_FREE 0xFF000000 /**< Définie quand on ne tient pas en compte cette valeur. C'est un NaN **/

/* Flags pour décider qui va permettre l'animation d'avancer */

#define ANIM_HEAD 0x0001
#define ANIM_NECK 0x0002
#define ANIM_LEFT_ARM1 0x0004
#define ANIM_LEFT_ARM2 0x0008
#define ANIM_RIGHT_ARM1 0x0010
#define ANIM_RIGHT_ARM2 0x0020
#define ANIM_LEFT_LEG1 0x0040
#define ANIM_LEFT_LEG2 0x0080
#define ANIM_RIGHT_LEG1 0x0100
#define ANIM_RIGHT_LEG2 0x0200
#define ANIM_ALL_TRIGGERS 0xffff

typedef struct {
	Vec2 Head, Neck, LeftArm1, LeftArm2, RightArm1, RightArm2, LeftLeg1, LeftLeg2, RightLeg1, RightLeg2;
} AnimPositions;

typedef struct {
	float Head, Neck, LeftArm1, LeftArm2, RightArm1, RightArm2, LeftLeg1, LeftLeg2, RightLeg1, RightLeg2;
} AnimAngles;

struct SPlayer;

typedef struct {
	AnimType Type;
	AnimTriggers Triggers;
	AnimPositions Positions;
	AnimAngles Angles;
	unsigned char TriggerCount;
	DynArr* States;
	Bool Ended;
	Bool Repeat;
	unsigned int CurrentState;
	float Spd[20];
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

void aniAddPositionState(Animation* A, Vec2 Head, Vec2 Neck, Vec2 LeftArm1, Vec2 LeftArm2, Vec2 RightArm1, Vec2 RightArm2, Vec2 LeftLeg1, Vec2 LeftLeg2, Vec2 RightLeg1, Vec2 RightLeg2);

void aniAddPositionState(Animation* A, const AnimPositions* Positions);

void aniAddAngleState(Animation* A, float Head, float Neck, float LeftArm1, float LeftArm2, float RightArm1, float RightArm2, float LeftLeg1, float LeftLeg2, float RightLeg1, float RightLeg2);

void aniAddAngleState(Animation* A, const AnimAngles* Angles);


void aniUpdate(Animation* A, SPlayer* P, float Step);
 
#endif