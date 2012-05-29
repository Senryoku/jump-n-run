#include "Animation.h"
#include <Objects/Player.h>


void aniInit(Animation* A, AnimType Type, AnimTriggers Triggers, Bool Repeat)
{
	int i;
	for (i=0; i<20; i++)
		A->Spd[i] = 0.f;
	A->States = newDynArr();
	A->Ended = FALSE;
	A->Repeat = Repeat;
	A->CurrentState = 0;
	A->Force = 0.65f;
	A->Friction = 0.6f;
	A->Type = Type;
	A->Diff = 1.f;
	assert(Triggers != 0);
	A->Triggers = Triggers;
	aniCountTriggers(A);
	
	A->Angles.Head = 0.f;
	A->Angles.Neck = 0.f;
	A->Angles.LeftArm1 = 0.f;
	A->Angles.LeftArm2 = 0.f;
	A->Angles.RightArm1 = 0.f;
	A->Angles.RightArm2 = 0.f;
	A->Angles.LeftLeg1 = 0.f;
	A->Angles.LeftLeg2 = 0.f;
	A->Angles.RightLeg1 = 0.f;
	A->Angles.RightLeg2 = 0.f;
	
	A->Positions.Head = vec2(0.f, 0.f);
	A->Positions.Neck = vec2(0.f, 0.f);
	A->Positions.LeftArm1 = vec2(0.f, 0.f);
	A->Positions.LeftArm2 = vec2(0.f, 0.f);
	A->Positions.RightArm1 = vec2(0.f, 0.f);
	A->Positions.RightArm2 = vec2(0.f, 0.f);
	A->Positions.LeftLeg1 = vec2(0.f, 0.f);
	A->Positions.LeftLeg2 = vec2(0.f, 0.f);
	A->Positions.RightLeg1 = vec2(0.f, 0.f);
	A->Positions.RightLeg2 = vec2(0.f, 0.f);
	
	
}

void aniCountTriggers(Animation* A)
{
	AnimTriggers Triggers = A->Triggers;
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

void aniFree(Animation* A)
{
	unsigned int i;
	for (i=0; i<daGetSize(A->States); i++)
		free(daGet(A->States, i));
	
	delDynArr(A->States);
}

Animation* newAnimation(AnimType Type, AnimTriggers Triggers, Bool Repeat)
{
	Animation* A;
	A = (Animation*) malloc(sizeof(Animation));
	aniInit(A, Type, Triggers, Repeat);
	return A;
}

void delAnimation(Animation* A)
{
	aniFree(A);
	free(A);
}


Bool aniLoadFromFile(Animation* A, const char* File)
{
	FILE* f;
	f = fopen(File, "r");
	if (f != NULL)
	{
		//[nb of states] [AnimType] [Triggers] [repeat]
		unsigned int type, count, triggers, repeat;
		fscanf(f, "%u %u %u %u\n", &count, &type, &triggers, &repeat);
		A->Type = (AnimType)type;
		A->Triggers = (AnimTriggers)triggers;
		A->Repeat = (Bool)repeat;
		
		//printf("type: %u, triggers: %u, nb states: %u\n", type, triggers, count);
		
		while (count>0)
		{
			if (A->Type == ANIM_POSITIONS)
			{
				AnimPositions Pos; unsigned int free, free2;
				
				Vec2* vec;
				
				vec = &Pos.Neck;
				fscanf(f, "%u %f %u %f #Neck\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.Head;
				fscanf(f, "%u %f %u %f #Head\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.LeftArm1;
				fscanf(f, "%u %f %u %f #LeftArm1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.LeftArm2;
				fscanf(f, "%u %f %u %f #LeftArm2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.RightArm1;
				fscanf(f, "%u %f %u %f #RightArm1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.RightArm2;
				fscanf(f, "%u %f %u %f #RightArm2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.LeftLeg1;
				fscanf(f, "%u %f %u %f #LeftLeg1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.LeftLeg2;
				fscanf(f, "%u %f %u %f #LeftLeg2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.RightLeg1;
				fscanf(f, "%u %f %u %f #RightLeg1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				vec = &Pos.RightLeg2;
				fscanf(f, "%u %f %u %f #RightLeg2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = NAN;
				if (free2)
					vec->y = NAN;
				
				aniAddPositionState(A, &Pos);
			}
			else
			{
				AnimAngles Ang; unsigned int free;
				
				float* a;
				
				a = &Ang.Neck;
				fscanf(f, "%u %f #Neck\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.Head;
				fscanf(f, "%u %f #Head\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.LeftArm1;
				fscanf(f, "%u %f #LeftArm1\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.LeftArm2;
				fscanf(f, "%u %f #LeftArm2\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.RightArm1;
				fscanf(f, "%u %f #RightArm1\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.RightArm2;
				fscanf(f, "%u %f #RightArm2\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.LeftLeg1;
				fscanf(f, "%u %f #LeftLeg1\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.LeftLeg2;
				fscanf(f, "%u %f #LeftLeg2\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.RightLeg1;
				fscanf(f, "%u %f #RightLeg1\n",&free, a);
				if (free)
					*a = NAN;
				
				a = &Ang.RightLeg2;
				fscanf(f, "%u %f #RightLeg2\n",&free, a);
				if (free)
					*a = NAN;
				
				aniAddAngleState(A, &Ang);
			}
			
			count--;
		}
		
	}
	else
		return TRUE;
	
	fclose(f);
	aniCountTriggers(A);
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

void aniSetCurrentState(Animation* A, unsigned int State)
{
	A->CurrentState = State;
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

void aniAddPositionState(Animation* A, const AnimPositions* Positions)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Pos;
	Pos = (AnimPositions*) malloc(sizeof(AnimPositions));
	Pos->Head = Positions->Head;
	Pos->Neck = Positions->Neck;
	Pos->LeftArm1 = Positions->LeftArm1;
	Pos->LeftArm2 = Positions->LeftArm2;
	Pos->RightArm1 = Positions->RightArm1;
	Pos->RightArm2 = Positions->RightArm2;
	Pos->LeftLeg1 = Positions->LeftLeg1;
	Pos->LeftLeg2 = Positions->LeftLeg2;
	Pos->RightLeg1 = Positions->RightLeg1;
	Pos->RightLeg2 = Positions->RightLeg2;
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

void aniAddAngleState(Animation* A, const AnimAngles* Angles)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Ang;
	Ang = (AnimAngles*) malloc(sizeof(AnimAngles));
	Ang->Head = Angles->Head;
	Ang->Neck = Angles->Neck;
	Ang->LeftArm1 = Angles->LeftArm1;
	Ang->LeftArm2 = Angles->LeftArm2;
	Ang->RightArm1 = Angles->RightArm1;
	Ang->RightArm2 = Angles->RightArm2;
	Ang->LeftLeg1 = Angles->LeftLeg1;
	Ang->LeftLeg2 = Angles->LeftLeg2;
	Ang->RightLeg1 = Angles->RightLeg1;
	Ang->RightLeg2 = Angles->RightLeg2;
	daAdd(A->States, Ang);
}

void aniUpdate(Animation* A, SPlayer* P, float Step)
{
	unsigned char TriggerCount = 0;
	float prevdt = 0.5f, dt = 0.5f;
	/**
	 @todo la tête on s'en fout un peu vu que l'on dessine quelquechose dessus, elle n'est importante que lorsque l'on fait un ragdoll, et dans ce cas l'animation n'affecte plus le joueur 
	 */
	
	/* Mise à jour des angles ou des positions */
	if (A->Type == ANIM_POSITIONS)
	{
		AnimPositions* Pos;
		Pos = (AnimPositions*) daGet(A->States, A->CurrentState);
		
		Bool VxResolved = FALSE;
		
		if (!ISNAN(Pos->Head.x))
		{
			Wobble(&A->Positions.Head.x, Pos->Head.x, A->Force, A->Friction, &A->Spd[0]);
			//vxsetpos
		}
		else
		{
			vxResolve(P->HeadLeft, prevdt, dt);
			vxResolve(P->HeadRight, prevdt, dt);
			vxApplyForce(P->HeadLeft, vec2(0.f, 0.6f), 1.f);
			vxApplyForce(P->HeadRight, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
			
			
		if (!ISNAN(Pos->Head.y))
		{
			Wobble(&A->Positions.Head.y, Pos->Head.y, A->Force, A->Friction, &A->Spd[1]);
		}
		else if (!VxResolved)
		{
			vxResolve(P->HeadLeft, prevdt, dt);
			vxResolve(P->HeadRight, prevdt, dt);
			vxApplyForce(P->HeadLeft, vec2(0.f, 0.6f), 1.f);
			vxApplyForce(P->HeadRight, vec2(0.f, 0.6f), 1.f);
		}
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->Neck.x))
		{
			Wobble(&A->Positions.Neck.x, Pos->Neck.x, A->Force, A->Friction, &A->Spd[2]);
			vxSetX(P->Neck, A->Positions.Neck.x);
		}
		else
		{
			vxResolve(P->Neck, prevdt, dt);
			vxApplyForce(P->Neck, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}

		if (!ISNAN(Pos->Neck.y))
		{
			Wobble(&A->Positions.Neck.y, Pos->Neck.y, A->Force, A->Friction, &A->Spd[3]);
			vxSetY(P->Neck, A->Positions.Neck.y);
		}
		else if (!VxResolved)
			vxResolve(P->Neck, prevdt, dt), vxApplyForce(P->Neck, vec2(0.f, 0.6f), 1.f);;
		
		////////////
		VxResolved = FALSE;
		if (!ISNAN(Pos->LeftArm1.x))
		{
			Wobble(&A->Positions.LeftArm1.x, Pos->LeftArm1.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->LeftArm1, A->Positions.LeftArm1.x);
		}
		else
		{
			vxResolve(P->LeftArm1, prevdt, dt);
			vxApplyForce(P->LeftArm1, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->LeftArm1.y))
		{
			Wobble(&A->Positions.LeftArm1.y, Pos->LeftArm1.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->LeftArm1, A->Positions.LeftArm1.y);
		}
		else if (!VxResolved)
			vxResolve(P->LeftArm1, prevdt, dt), vxApplyForce(P->LeftArm1, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->LeftArm2.x))
		{
			Wobble(&A->Positions.LeftArm2.x, Pos->LeftArm2.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->LeftArm2, A->Positions.LeftArm2.x);
		}
		else
		{
			vxResolve(P->LeftArm2, prevdt, dt);
			vxApplyForce(P->LeftArm2, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->LeftArm2.y))
		{
			Wobble(&A->Positions.LeftArm2.y, Pos->LeftArm2.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->LeftArm2, A->Positions.LeftArm2.y);
		}
		else if (!VxResolved)
			vxResolve(P->LeftArm2, prevdt, dt), vxApplyForce(P->LeftArm2, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->RightArm1.x))
		{
			Wobble(&A->Positions.RightArm1.x, Pos->RightArm1.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->RightArm1, A->Positions.RightArm1.x);
		}
		else
		{
			vxResolve(P->RightArm1, prevdt, dt);
			vxApplyForce(P->RightArm1, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->RightArm1.y))
		{
			Wobble(&A->Positions.RightArm1.y, Pos->RightArm1.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->RightArm1, A->Positions.RightArm1.y);
		}
		else if (!VxResolved)
			vxResolve(P->RightArm1, prevdt, dt), vxApplyForce(P->RightArm1, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->RightArm2.x))
		{
			Wobble(&A->Positions.RightArm2.x, Pos->RightArm2.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->RightArm2, A->Positions.RightArm2.x);
		}
		else
		{
			vxResolve(P->RightArm2, prevdt, dt);
			vxApplyForce(P->RightArm2, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->RightArm2.y))
		{
			Wobble(&A->Positions.RightArm2.y, Pos->RightArm2.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->RightArm2, A->Positions.RightArm2.y);
		}
		else if (!VxResolved)
			vxResolve(P->RightArm2, prevdt, dt), vxApplyForce(P->RightArm2, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->LeftLeg1.x))
		{
			Wobble(&A->Positions.LeftLeg1.x, Pos->LeftLeg1.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->LeftLeg1, A->Positions.LeftLeg1.x);
		}
		else
		{
			vxResolve(P->LeftLeg1, prevdt, dt);
			vxApplyForce(P->LeftLeg1, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->LeftLeg1.y))
		{
			Wobble(&A->Positions.LeftLeg1.y, Pos->LeftLeg1.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->LeftLeg1, A->Positions.LeftLeg1.y);
		}
		else if (!VxResolved)
			vxResolve(P->LeftLeg1, prevdt, dt), vxApplyForce(P->LeftLeg1, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->LeftLeg2.x))
		{
			Wobble(&A->Positions.LeftLeg2.x, Pos->LeftLeg2.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->LeftLeg2, A->Positions.LeftLeg2.x);
		}
		else
		{
			vxResolve(P->LeftLeg2, prevdt, dt);
			vxApplyForce(P->LeftLeg2, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->LeftLeg2.y))
		{
			Wobble(&A->Positions.LeftLeg2.y, Pos->LeftLeg2.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->LeftLeg2, A->Positions.LeftLeg2.y);
		}
		else if (!VxResolved)
			vxResolve(P->LeftLeg2, prevdt, dt), vxApplyForce(P->LeftLeg2, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->RightLeg1.x))
		{
			Wobble(&A->Positions.RightLeg1.x, Pos->RightLeg1.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->RightLeg1, A->Positions.RightLeg1.x);
		}
		else
		{
			vxResolve(P->RightLeg1, prevdt, dt);
			vxApplyForce(P->RightLeg1, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->RightLeg1.y))
		{
			Wobble(&A->Positions.RightLeg1.y, Pos->RightLeg1.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->RightLeg1, A->Positions.RightLeg1.y);
		}
		else if (!VxResolved)
			vxResolve(P->RightLeg1, prevdt, dt), vxApplyForce(P->RightLeg1, vec2(0.f, 0.6f), 1.f);
		
		
		VxResolved = FALSE;
		if (!ISNAN(Pos->RightLeg2.x))
		{
			Wobble(&A->Positions.RightLeg2.x, Pos->RightLeg2.x, A->Force, A->Friction, &A->Spd[4]);
			vxSetX(P->RightLeg2, A->Positions.RightLeg2.x);
		}
		else
		{
			vxResolve(P->RightLeg2, prevdt, dt);
			vxApplyForce(P->RightLeg2, vec2(0.f, 0.6f), 1.f);
			VxResolved = TRUE;
		}
		
		if (!ISNAN(Pos->RightLeg2.y))
		{
			Wobble(&A->Positions.RightLeg2.y, Pos->RightLeg2.y, A->Force, A->Friction, &A->Spd[5]);
			vxSetY(P->RightLeg2, A->Positions.RightLeg2.y);
		}
		else if (!VxResolved)
			vxResolve(P->RightLeg2, prevdt, dt), vxApplyForce(P->RightLeg2, vec2(0.f, 0.6f), 1.f);
		
		
		/* Vérification de l'état de l'animation pour passer à létat suivant */
		if (A->Triggers & ANIM_LEFT_LEG2)
			if ((Pos->LeftLeg2.x!=Pos->LeftLeg2.x || ABS(A->Positions.LeftLeg2.x-Pos->LeftLeg2.x) < A->Diff) && (Pos->LeftLeg2.y!=Pos->LeftLeg2.y || ABS(A->Positions.LeftLeg2.y-Pos->LeftLeg2.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG2)
			if ((Pos->RightLeg2.x!=Pos->RightLeg2.x || ABS(A->Positions.RightLeg2.x-Pos->RightLeg2.x)) < A->Diff && (Pos->RightLeg2.y!=Pos->RightLeg2.y || ABS(A->Positions.RightLeg2.y-Pos->RightLeg2.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG1)
			if ((Pos->RightLeg1.x!=Pos->RightLeg1.x || ABS(A->Positions.RightLeg1.x-Pos->RightLeg1.x) < A->Diff) && (Pos->RightLeg1.y!=Pos->RightLeg1.y || ABS(A->Positions.RightLeg1.y-Pos->RightLeg1.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_LEG1)
			if ((Pos->LeftLeg1.x!=Pos->LeftLeg1.x || ABS(A->Positions.LeftLeg1.x-Pos->LeftLeg1.x) < A->Diff) && (Pos->LeftLeg1.y!=Pos->LeftLeg1.y || ABS(A->Positions.LeftLeg1.y-Pos->LeftLeg1.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM1)
			if ((Pos->LeftArm1.x!=Pos->LeftArm1.x || ABS(A->Positions.LeftArm1.x-Pos->LeftArm1.x) < A->Diff) && (Pos->LeftArm1.y!=Pos->LeftArm1.y || ABS(A->Positions.LeftArm1.y-Pos->LeftArm1.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM2)
			if ((Pos->LeftArm2.x!=Pos->LeftArm2.x || ABS(A->Positions.LeftArm2.x-Pos->LeftArm2.x) < A->Diff) && (Pos->LeftArm2.y!=Pos->LeftArm2.y || ABS(A->Positions.LeftArm2.y-Pos->LeftArm2.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM1)
			if ((Pos->RightArm1.x!=Pos->RightArm1.x || ABS(A->Positions.RightArm1.x-Pos->RightArm1.x) < A->Diff) && (Pos->RightArm1.y!=Pos->RightArm1.y || ABS(A->Positions.RightArm1.y-Pos->RightArm1.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM2)
			if ((Pos->RightArm2.x!=Pos->RightArm2.x || ABS(A->Positions.RightArm2.x-Pos->RightArm2.x) < A->Diff) && (Pos->RightArm2.y!=Pos->RightArm2.y || ABS(A->Positions.RightArm2.y-Pos->RightArm2.y) < A->Diff))
				TriggerCount++;
		
		if (A->Triggers & ANIM_NECK)
			if ((Pos->Neck.x!=Pos->Neck.x || ABS(A->Positions.Neck.x-Pos->Neck.x) < A->Diff) && (Pos->Neck.y!=Pos->Neck.y || ABS(A->Positions.Neck.y-Pos->Neck.y) < A->Diff))
				TriggerCount++;
		if (A->Triggers & ANIM_HEAD)
			if ((Pos->Head.x!=Pos->Head.x || ABS(A->Positions.Head.x-Pos->Head.x) < A->Diff) && (Pos->Head.y!=Pos->Head.y || ABS(A->Positions.Head.y-Pos->Head.y) < A->Diff))
				TriggerCount++;
		
		if (TriggerCount>=A->TriggerCount)
		{
			A->CurrentState++;
			
			if (A->CurrentState >= daGetSize(A->States))
			{
				A->Ended = TRUE;
				if (A->Repeat)
					A->CurrentState = 0;
				else
					A->CurrentState--;
			}
		}
		
	}
	else
	{
		AnimAngles* Ang;
		Ang = (AnimAngles*) daGet(A->States, A->CurrentState);
		
		/* On change la position des vertex du joueur */
		
		if (!ISNAN(Ang->Neck))
		{
			Wobble(&A->Angles.Neck, Ang->Neck, A->Force, A->Friction, &A->Spd[1]);
			vxSetPosition(P->Neck, vec2Add(vxGetPosition(P->Base), vec2Rotate(vec2(60.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.Neck))));
		}
		else
			vxResolve(P->Neck, prevdt, dt), vxApplyForce(P->Neck, vec2(0.f, 0.6f), 1.f);;
			
		if (!ISNAN(Ang->Head))
		{
			Wobble(&A->Angles.Head, Ang->Head, A->Force, A->Friction, &A->Spd[0]);
			//la tête est un peu spéciale car on a l'angle du
		}
		else
		{
			vxResolve(P->HeadLeft, prevdt, dt), vxApplyForce(P->HeadLeft, vec2(0.f, 0.6f), 1.f);;
			vxResolve(P->HeadRight, prevdt, dt), vxApplyForce(P->HeadRight, vec2(0.f, 0.6f), 1.f);;
		}
			
		
		
				
		if (!ISNAN(Ang->LeftArm1))
		{
			Wobble(&A->Angles.LeftArm1, Ang->LeftArm1, A->Force, A->Friction, &A->Spd[2]);
			vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2Rotate(vec2(-25.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftArm1))));
		}
		else
			vxResolve(P->LeftArm1, prevdt, dt), vxApplyForce(P->LeftArm1, vec2(0.f, 0.6f), 1.f);
			
		if (!ISNAN(Ang->LeftArm2))
		{
			Wobble(&A->Angles.LeftArm2, Ang->LeftArm2, A->Force, A->Friction, &A->Spd[3]);
			vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->LeftArm1), vec2Rotate(vec2(-25.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftArm2))));
		}
		else
			vxResolve(P->LeftArm2, prevdt, dt), vxApplyForce(P->LeftArm2, vec2(0.f, 0.6f), 1.f);;
			
		
		if (!ISNAN(Ang->RightArm1))
		{
			Wobble(&A->Angles.RightArm1, Ang->RightArm1, A->Force, A->Friction, &A->Spd[4]);
			vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2Rotate(vec2(-25.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightArm1))));
		}
		else
			vxResolve(P->RightArm1, prevdt, dt), vxApplyForce(P->RightArm1, vec2(0.f, 0.6f), 1.f);;
			
		if (Ang->RightArm2 == Ang->RightArm2)
		{
			Wobble(&A->Angles.RightArm2, Ang->RightArm2, A->Force, A->Friction, &A->Spd[5]);
			vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->RightArm1), vec2Rotate(vec2(-25.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightArm2))));
		}
		else
			vxResolve(P->RightArm2, prevdt, dt), vxApplyForce(P->RightArm2, vec2(0.f, 0.6f), 1.f);;
			
				
		if (!ISNAN(Ang->LeftLeg1))
		{
			Wobble(&A->Angles.LeftLeg1, Ang->LeftLeg1, A->Force, A->Friction, &A->Spd[6]);
			vxSetPosition(P->LeftLeg1, vec2Add(vxGetPosition(P->Base), vec2Rotate(vec2(-30.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftLeg1))));
		}
		else
			vxResolve(P->LeftLeg1, prevdt, dt), vxApplyForce(P->LeftLeg1, vec2(0.f, 0.6f), 1.f);;
			
		if (!ISNAN(Ang->LeftLeg2))
		{
			Wobble(&A->Angles.LeftLeg2, Ang->LeftLeg2, A->Force, A->Friction, &A->Spd[7]);
			vxSetPosition(P->LeftLeg2, vec2Add(vxGetPosition(P->LeftLeg1), vec2Rotate(vec2(-30.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftLeg2))));
		}
		else
			vxResolve(P->LeftLeg2, prevdt, dt), vxApplyForce(P->LeftLeg2, vec2(0.f, 0.6f), 1.f);;
			
		
		if (!ISNAN(Ang->RightLeg1))
		{
			Wobble(&A->Angles.RightLeg1, Ang->RightLeg1, A->Force, A->Friction, &A->Spd[8]);
			vxSetPosition(P->RightLeg1, vec2Add(vxGetPosition(P->Base), vec2Rotate(vec2(-30.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightLeg1))));
		}
		else
			vxResolve(P->RightLeg1, prevdt, dt), vxApplyForce(P->RightLeg1, vec2(0.f, 0.6f), 1.f);;
			
		if (!ISNAN(Ang->RightLeg2))
		{
			Wobble(&A->Angles.RightLeg2, Ang->RightLeg2, A->Force, A->Friction, &A->Spd[9]);
			vxSetPosition(P->RightLeg2, vec2Add(vxGetPosition(P->RightLeg1), vec2Rotate(vec2(-30.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightLeg2))));
		}
		else
			vxResolve(P->RightLeg2, prevdt, dt), vxApplyForce(P->RightLeg2, vec2(0.f, 0.6f), 1.f);;
			
		
		
		
		
		/* Vérification de l'état de l'animation pour passer à létat suivant */
		/* Ang->LeftLeg2!=Ang->LeftLeg2 teste le NaN donc NAN (le vertex n'est pas à tenir en compte) */
		if (A->Triggers & ANIM_LEFT_LEG2)
			if (ISNAN(Ang->LeftLeg2) || ABS(A->Angles.LeftLeg2-Ang->LeftLeg2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG2)
			if (ISNAN(Ang->RightLeg2) || ABS(A->Angles.RightLeg2-Ang->RightLeg2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG1)
			if (ISNAN(Ang->RightLeg1) || ABS(A->Angles.RightLeg1-Ang->RightLeg1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_LEG1)
			if (ISNAN(Ang->LeftLeg1) || ABS(A->Angles.LeftLeg1-Ang->LeftLeg1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM1)
			if (ISNAN(Ang->LeftArm1) || ABS(A->Angles.LeftArm1-Ang->LeftArm1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM2)
			if (ISNAN(Ang->LeftArm2) || ABS(A->Angles.LeftArm2-Ang->LeftArm2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM1)
			if (ISNAN(Ang->RightArm1) || ABS(A->Angles.RightArm1-Ang->RightArm1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM2)
			if (ISNAN(Ang->RightArm2) || ABS(A->Angles.RightArm2-Ang->RightArm2) < A->Diff)
				TriggerCount++;
		
		if (A->Triggers & ANIM_NECK)
			if (ISNAN(Ang->Neck) || ABS(A->Angles.Neck-Ang->Neck) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_HEAD)
			if (ISNAN(Ang->Head) || ABS(A->Angles.Head-Ang->Head) < A->Diff)
				TriggerCount++;
		
		if (TriggerCount >= A->TriggerCount)
		{
			A->CurrentState++;
			if (A->CurrentState >= daGetSize(A->States))
			{
				A->Ended = TRUE;
				if (A->Repeat)
					A->CurrentState = 0;
				else
					A->CurrentState--;
			}
		}				
				
		
	}	
	
	
}

void aniUpdateForCurrentState(Animation* A)
{
	int i;
	//On fait un reset sur les changement de valeurs;
	for (i=0; i<20; i++)
		A->Spd[i] = 0.f;
	
	if (A->Type == ANIM_POSITIONS)
	{
		AnimPositions* Pos;
		Vec2* Vsrc, *Vdest;
		Pos = (AnimPositions*) daGet(A->States, A->CurrentState);
		
		Vsrc = &Pos->Neck;
		Vdest = &A->Positions.Neck;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->Head;
		Vdest = &A->Positions.Head;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->LeftArm1;
		Vdest = &A->Positions.LeftArm1;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->LeftArm2;
		Vdest = &A->Positions.LeftArm2;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->RightArm1;
		Vdest = &A->Positions.RightArm1;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->RightArm2;
		Vdest = &A->Positions.RightArm2;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->LeftLeg1;
		Vdest = &A->Positions.LeftLeg1;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->LeftLeg2;
		Vdest = &A->Positions.LeftLeg2;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->RightLeg1;
		Vdest = &A->Positions.RightLeg1;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
		Vsrc = &Pos->RightLeg2;
		Vdest = &A->Positions.RightLeg2;
		if (Vsrc->x == Vsrc->x)
			Vdest->x = Vsrc->x;
		if (Vsrc->y == Vsrc->y)
			Vdest->y = Vsrc->y;
		
	}
	else
	{
		AnimAngles* Ang;
		Ang = (AnimAngles*) daGet(A->States, A->CurrentState);
		
		if (Ang->Neck == Ang->Neck)
			A->Angles.Neck = Ang->Neck;
		if (Ang->LeftArm1 == Ang->LeftArm1)
			A->Angles.LeftArm1 = Ang->LeftArm1;
		if (Ang->LeftArm2 == Ang->LeftArm2)
			A->Angles.LeftArm2 = Ang->LeftArm2;
		if (Ang->RightArm1 == Ang->RightArm1)
			A->Angles.RightArm1 = Ang->RightArm1;
		if (Ang->RightArm2 == Ang->RightArm2)
			A->Angles.RightArm2 = Ang->RightArm2;
		if (Ang->LeftLeg1 == Ang->LeftLeg1)
			A->Angles.LeftLeg1 = Ang->LeftLeg1;
		if (Ang->LeftLeg2 == Ang->LeftLeg2)
			A->Angles.LeftLeg2 = Ang->LeftLeg2;
		if (Ang->RightLeg1 == Ang->RightLeg1)
			A->Angles.RightLeg1 = Ang->RightLeg1;
		if (Ang->RightLeg2 == Ang->RightLeg2)
			A->Angles.RightLeg2 = Ang->RightLeg2;
		if (Ang->Head == Ang->Head)
			A->Angles.Head = Ang->Head;
		
	}
}


void aniSetForce(Animation* A, float Force)
{
	A->Force = Force;
}

float aniGetForce(const Animation* A)
{
	return A->Force;
}

void aniSetFriction(Animation* A, float Friction)
{
	A->Friction = Friction;
}

float aniGetFriction(const Animation* A)
{
	return A->Friction;
}

void aniSetBodyPartAngle(Animation* A, AnimTriggers BodyPart, float Angle, unsigned int State)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Angles = (AnimAngles*)daGet(A->States, State);
	
	if (BodyPart & ANIM_LEFT_ARM1)
		Angles->LeftArm1 = Angle;
	
	if (BodyPart & ANIM_LEFT_ARM2)
		Angles->LeftArm2 = Angle;
	
	if (BodyPart & ANIM_RIGHT_ARM1)
		Angles->RightArm1 = Angle;
	
	if (BodyPart & ANIM_RIGHT_ARM2)
		Angles->RightArm2 = Angle;
	
	if (BodyPart & ANIM_LEFT_LEG1)
		Angles->LeftLeg1 = Angle;
	
	if (BodyPart & ANIM_LEFT_LEG2)
		Angles->LeftLeg2 = Angle;
	
	if (BodyPart & ANIM_RIGHT_LEG1)
		Angles->RightLeg1 = Angle;
	
	if (BodyPart & ANIM_RIGHT_LEG2)
		Angles->RightLeg2 = Angle;
	
	if (BodyPart & ANIM_NECK)
		Angles->Neck = Angle;
		
}

void aniSetBodyPartPosition(Animation* A, AnimTriggers BodyPart, Vec2 Pos, unsigned int State)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Positions = (AnimPositions*)daGet(A->States, State);
	
	if (BodyPart & ANIM_LEFT_ARM1)
		Positions->LeftArm1 = Pos;
	
	if (BodyPart & ANIM_LEFT_ARM2)
		Positions->LeftArm2 = Pos;
	
	if (BodyPart & ANIM_RIGHT_ARM1)
		Positions->RightArm1 = Pos;
	
	if (BodyPart & ANIM_RIGHT_ARM2)
		Positions->RightArm2 = Pos;
	
	if (BodyPart & ANIM_LEFT_LEG1)
		Positions->LeftLeg1 = Pos;
	
	if (BodyPart & ANIM_LEFT_LEG2)
		Positions->LeftLeg2 = Pos;
	
	if (BodyPart & ANIM_RIGHT_LEG1)
		Positions->RightLeg1 = Pos;
	
	if (BodyPart & ANIM_RIGHT_LEG2)
		Positions->RightLeg2 = Pos;
	
	if (BodyPart & ANIM_NECK)
		Positions->Neck = Pos;
}
// 1000 lignes! HA!