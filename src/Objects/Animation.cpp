#include "Animation.h"

void aniInit(Animation* A)
{
	A->States = newDynArr();
	A->Ended = FALSE;
	A->Type = ANIM_ANGLES;
	A->Triggers = ANIM_ALL_TRIGGERS;
	A->TriggerCount = 10;
	A->CurrentState = 0;
	A->Force = 0.75f;
	A->Friction = 0.6f;
}

void aniFree(Animation* A)
{
	unsigned int i;
	for (i=0; i<daGetSize(A->States); i++)
		free(daGet(A->States, i));
	
	delDynArr(A->States);
}

Animation* newAnimation()
{
	Animation* A;
	A = (Animation*) malloc(sizeof(Animation));
	aniInit(A);
	return A;
}

void delAnimation(Animation* A)
{
	aniFree(A);
	free(A);
}

void aniSetOptions(Animation* A, AnimType Type, AnimTriggers Triggers)
{
	A->Type = Type;
	assert(Triggers != 0);
	A->Triggers = Triggers;
	if (A->Triggers == ANIM_ALL_TRIGGERS)
		A->TriggerCount = 10;
	else
	{
		unsigned char i =0;
		while (Triggers>0)
		{
			if (Triggers%2 == 1)
				i++;
			Triggers>>=1;
		}
		A->TriggerCount = i;
	}
}

Bool aniLoadFromFile(Animation* A)
{
	return FALSE; /* No Error */
}

void aniResetEnded(Animation* A)
{
	A->Ended = FALSE;
}

Bool aniIsEnded(const Animation* A)
{
	return A->Ended;
}

unsigned int aniGetStatesCount(const Animation* A)
{
	return daGetSize(A->States);
}

unsigned int aniGetCurrentState(const Animation* A)
{
	return A->CurrentState;
}

void aniAddPositionState(Animation* A, Vec2 Head, Vec2 Neck, Vec2 LeftArm1, Vec2 LeftArm2, Vec2 RightArm1, Vec2 RightArm2, Vec2 LeftLeg1, Vec2 LeftLeg2, Vec2 RightLeg1, Vec2 RightLeg2)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Pos;
	Pos = (AnimPositions*) malloc(sizeof(AnimPositions));
	Pos->Head = Head;
	Pos->Neck = Neck;
	Pos->LeftArm1 = LeftArm1;
	Pos->LeftArm2 = LeftArm2;
	Pos->RightArm1 = RightArm1;
	Pos->RightArm2 = RightArm2;
	Pos->LeftLeg1 = LeftLeg1;
	Pos->LeftLeg2 = LeftLeg2;
	Pos->RightLeg1 = RightLeg1;
	Pos->RightLeg2 = RightLeg2;
	daAdd(A->States, Pos);
}

void aniAddAngleState(Animation* A, float Head, float Neck, float LeftArm1, float LeftArm2, float RightArm1, float RightArm2, float LeftLeg1, float LeftLeg2, float RightLeg1, float RightLeg2)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Ang;
	Ang = (AnimAngles*) malloc(sizeof(AnimAngles));
	Ang->Head = Head;
	Ang->Neck = Neck;
	Ang->LeftArm1 = LeftArm1;
	Ang->LeftArm2 = LeftArm2;
	Ang->RightArm1 = RightArm1;
	Ang->RightArm2 = RightArm2;
	Ang->LeftLeg1 = LeftLeg1;
	Ang->LeftLeg2 = LeftLeg2;
	Ang->RightLeg1 = RightLeg1;
	Ang->RightLeg2 = RightLeg2;
	daAdd(A->States, Ang);
}

void aniUpdate(Animation* A, float Step)
{
	if (A->Type == ANIM_POSITIONS)
	{
		AnimPositions* Pos;
		Pos = (AnimPositions*) daGet(A->States, A->CurrentState);
		
		if (Pos->Head.x != ANIM_FREE)
			Wobble(&A->Positions.Head.x, Pos->Head.x, A->Force, A->Friction, &A->Spd[0]);
		if (Pos->Head.y != ANIM_FREE)
			Wobble(&A->Positions.Head.y, Pos->Head.y, A->Force, A->Friction, &A->Spd[1]);
		if (Pos->Neck.x != ANIM_FREE)
			Wobble(&A->Positions.Neck.x, Pos->Neck.x, A->Force, A->Friction, &A->Spd[2]);
		if (Pos->Neck.y != ANIM_FREE)
			Wobble(&A->Positions.Neck.y, Pos->Neck.y, A->Force, A->Friction, &A->Spd[3]);
		
		if (Pos->LeftArm1.x != ANIM_FREE)
			Wobble(&A->Positions.LeftArm1.x, Pos->LeftArm1.x, A->Force, A->Friction, &A->Spd[4]);
		if (Pos->LeftArm1.y != ANIM_FREE)
			Wobble(&A->Positions.LeftArm1.y, Pos->LeftArm1.y, A->Force, A->Friction, &A->Spd[5]);
		if (Pos->LeftArm2.x != ANIM_FREE)
			Wobble(&A->Positions.LeftArm2.x, Pos->LeftArm2.x, A->Force, A->Friction, &A->Spd[6]);
		if (Pos->LeftArm2.y != ANIM_FREE)
			Wobble(&A->Positions.LeftArm2.y, Pos->LeftArm2.y, A->Force, A->Friction, &A->Spd[7]);
		if (Pos->RightArm1.x != ANIM_FREE)
			Wobble(&A->Positions.RightArm1.x, Pos->RightArm1.x, A->Force, A->Friction, &A->Spd[8]);
		if (Pos->RightArm1.y != ANIM_FREE)
			Wobble(&A->Positions.RightArm1.y, Pos->RightArm1.y, A->Force, A->Friction, &A->Spd[9]);
		if (Pos->RightArm2.x != ANIM_FREE)
			Wobble(&A->Positions.RightArm2.x, Pos->RightArm2.x, A->Force, A->Friction, &A->Spd[10]);
		if (Pos->RightArm2.y != ANIM_FREE)
			Wobble(&A->Positions.RightArm2.y, Pos->RightArm2.y, A->Force, A->Friction, &A->Spd[11]);
		
		if (Pos->LeftLeg1.x != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg1.x, Pos->LeftLeg1.x, A->Force, A->Friction, &A->Spd[12]);
		if (Pos->LeftLeg1.y != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg1.y, Pos->LeftLeg1.y, A->Force, A->Friction, &A->Spd[13]);
		if (Pos->LeftLeg2.x != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg2.x, Pos->LeftLeg2.x, A->Force, A->Friction, &A->Spd[14]);
		if (Pos->LeftLeg2.y != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg2.y, Pos->LeftLeg2.y, A->Force, A->Friction, &A->Spd[15]);
		if (Pos->RightLeg1.x != ANIM_FREE)
			Wobble(&A->Positions.RightLeg1.x, Pos->RightLeg1.x, A->Force, A->Friction, &A->Spd[16]);
		if (Pos->RightLeg1.y != ANIM_FREE)
			Wobble(&A->Positions.RightLeg1.y, Pos->RightLeg1.y, A->Force, A->Friction, &A->Spd[17]);
		if (Pos->RightLeg2.x != ANIM_FREE)
			Wobble(&A->Positions.RightLeg2.x, Pos->RightLeg2.x, A->Force, A->Friction, &A->Spd[18]);
		if (Pos->RightLeg2.y != ANIM_FREE)
			Wobble(&A->Positions.RightLeg2.y, Pos->RightLeg2.y, A->Force, A->Friction, &A->Spd[19]);
		
		
	}
	else
	{
		
	}
}
