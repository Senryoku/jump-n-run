#include "Animation.h"
#include <Objects/Player.h>

void animAnglesStatesInit(AnimAngles* ang)
{
	for (int i=0; i<10; i++)
		ang->Angles[i] = 0.f;
	ang->Ended = FALSE;
	ang->CurrentState = 0;
	for (int i=0; i<10; i++)
		ang->Spd[i] = 0.f;
}

void animPositionsStatesInit(AnimPositions* pos)
{
	for (int i=0; i<10; i++)
		pos->Positions[i] = vec2(0.f, 0.f);
	pos->Ended = FALSE;
	pos->CurrentState = 0;
	for (int i=0; i<20; i++)
		pos->Spd[i] = 0.f;
}

AnimType aniGetType(const Animation* A)
{
	return A->Type;
}

void aniInit(Animation* A, AnimType Type, AnimTriggers Triggers, Bool Repeat)
{
	A->States = newDynArr();
	A->Repeat = Repeat;
	A->Force = 0.65f;
	A->Friction = 0.6f;
	A->Type = Type;
	A->Diff = 1.f;
	assert(Triggers != 0);
	A->Triggers = Triggers;
	aniCountTriggers(A);
	
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
	
	//printf("Triggers counted: %u\n",A->TriggerCount);
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
		if (fscanf(f, "%u %u %u %u\n", &count, &type, &triggers, &repeat) !=4) ERROR();
		A->Type = (AnimType)type;
		A->Triggers = (AnimTriggers)triggers;
		A->Repeat = (Bool)repeat;
		
		//printf("Animation read; type: %u, triggers: %u, nb states: %u\n", type, triggers, count);
		
		while (count>0)
		{
			if (A->Type == ANIM_POSITIONS)
			{
				AnimPositions Pos; unsigned int free, free2;
				
				Vec2* vec;
				char read[100];
				
				for (int i=0; i<10; i++)
				{
					if (fgets(read, 100, f) == NULL) ERROR();
					vec = &Pos.Positions[i];
					sscanf(read, "%u %f %u %f", &free, &vec->x, &free2, &vec->y);
					if (free)
						vec->x = NAN;
					if (free2)
						vec->y = NAN;
				}
				
				aniAddPositionState(A, &Pos);
			}
			else
			{
				AnimAngles Ang; unsigned int free;
				
				float* a;
				char read[100];
				
				for (int i=0; i<10; i++)
				{
					if (fgets(read, 100, f) == NULL) ERROR();
					a = &Ang.Angles[i];
					sscanf(read, "%u %f", &free, a);
					if (free)
						*a = NAN;
				}
				
				aniAddAngleState(A, &Ang);

			}
			
			count--;
		}
		
	}
	else
	{
		printf("Erreur de lecture pour le fichier %s\n", File);
		return TRUE;
	}
		
	
	fclose(f);
	aniCountTriggers(A);
	return FALSE; /* No Error */
}


unsigned int aniGetStatesCount(const Animation* A)
{
	return daGetSize(A->States);
}

void aniAddPositionState(Animation* A, const AnimPositions* Positions)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Pos;
	Pos = (AnimPositions*) malloc(sizeof(AnimPositions));
	for (int i=0; i<10; i++)
		Pos->Positions[i] = Positions->Positions[i];
	daAdd(A->States, Pos);
}

void aniAddAngleState(Animation* A, const AnimAngles* Angles)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Ang;
	Ang = (AnimAngles*) malloc(sizeof(AnimAngles));
	for (int i=0; i<10; i++)
		Ang->Angles[i] = Angles->Angles[i];
	daAdd(A->States, Ang);
}

void aniUpdate(Animation* A, SPlayer* P, float Step)
{
	unsigned char TriggerCount = 0;
	AnimTriggers Triggering = ANIM_NECK; // = 1
	float prevdt = 0.5f, dt = 0.5f;
	/**
	 @todo la tête on s'en fout un peu vu que l'on dessine quelquechose dessus, elle n'est importante que lorsque l'on fait un ragdoll, et dans ce cas l'animation n'affecte plus le joueur 
	 */
	
	/* Mise à jour des angles ou des positions */
	if (A->Type == ANIM_POSITIONS)
	{
		AnimPositions* Pos;
		if (plGetAnimPositionsState(P)->CurrentState >= aniGetStatesCount(A))
			plGetAnimPositionsState(P)->CurrentState = 0;
		Pos = (AnimPositions*) daGet(A->States, plGetAnimPositionsState(P)->CurrentState);
		
		Bool VxResolved;
		
		for (int i=0; i<10; i++)
		{
			VxResolved = FALSE;
			if (!ISNAN(Pos->Positions[i].x))
			{
				Wobble(&P->Positions.Positions[i].x, Pos->Positions[i].x, A->Force*Step, A->Friction*(1.f-Step), &plGetAnimPositionsState(P)->Spd[i]);
				if (i<bpHeadLeft)
					vxSetX(P->vxBodyParts[i], P->Positions.Positions[i].x);
			}
			else
			{
				if (i<bpHeadLeft)
					P->Positions.Positions[i].x = vxGetPosition(P->vxBodyParts[i]).x;
				vxResolve(P->vxBodyParts[i], prevdt, dt);
				vxApplyForce(P->vxBodyParts[i], vec2(0.f, 0.6f), 1.f);
				if (i==bpHeadLeft) //Tête
				{
					vxResolve(P->vxBodyParts[i+1], prevdt, dt);
					vxApplyForce(P->vxBodyParts[i+1], vec2(0.f, 0.6f), 1.f);
				}
				VxResolved = TRUE;
			}
			
			if (!ISNAN(Pos->Positions[i].y))
			{
				Wobble(&P->Positions.Positions[i].y, Pos->Positions[i].y, A->Force*Step, A->Friction*(1.f-Step), &plGetAnimPositionsState(P)->Spd[i*2]);
				if (i<bpHeadLeft)
					vxSetY(P->vxBodyParts[i], P->Positions.Positions[i].y);
			}
			else
			{
				if (i<bpHeadLeft)
					P->Positions.Positions[i].y = vxGetPosition(P->vxBodyParts[i]).y;
				if (!VxResolved)
				{
					vxResolve(P->vxBodyParts[i], prevdt, dt);
					vxApplyForce(P->vxBodyParts[i], vec2(0.f, 0.6f), 1.f);
					if (i==bpHeadLeft) //Tête
					{
						vxResolve(P->vxBodyParts[i+1], prevdt, dt);
						vxApplyForce(P->vxBodyParts[i+1], vec2(0.f, 0.6f), 1.f);
					}
				}
			}
			
			if (A->Triggers & Triggering)
				if ((ISNAN(Pos->Positions[i].x) || ABS(P->Positions.Positions[i].x-Pos->Positions[i].x) < A->Diff) && (ISNAN(Pos->Positions[i].y) || ABS(P->Positions.Positions[i].y-Pos->Positions[i].y)))
					TriggerCount++;
			Triggering <<= 1; // *= 2
		}
		
		if (TriggerCount>=A->TriggerCount)
		{
			plGetAnimPositionsState(P)->CurrentState++;
			
			if (plGetAnimPositionsState(P)->CurrentState >= daGetSize(A->States))
			{
				plGetAnimPositionsState(P)->Ended = TRUE;
				if (A->Repeat)
					plGetAnimPositionsState(P)->CurrentState = 0;
				else
					plGetAnimPositionsState(P)->CurrentState--;
			}
		}
		
	}
	else
	{
		AnimAngles* Ang;
		if (plGetAnimAnglesState(P)->CurrentState >= aniGetStatesCount(A))
			plGetAnimAnglesState(P)->CurrentState = 0;
		Ang = (AnimAngles*) daGet(A->States, plGetAnimAnglesState(P)->CurrentState);
		
		
		/* On change la position des vertex du joueur */
		for (int i=0; i<10; i++)
		{
			if (!ISNAN(Ang->Angles[i]))
			{
				Wobble(&P->Angles.Angles[i], Ang->Angles[i], A->Force*Step, A->Friction*MAX(1.f,1.f-Step), &plGetAnimAnglesState(P)->Spd[i]);
				if (i<bpHeadLeft)
				{
					Vertex* from;
					Vec2 size;
					
					switch (i) {
						case bpNeck:
							from = P->vxBodyParts[bpBase];
							size = vec2(60.f, 0.f);
							break;
						case bpLeftArm1:
						case bpRightArm1:
							from = P->vxBodyParts[bpNeck];
							size = vec2(-25.f, 0.f);
							break;
						case bpLeftArm2:
						case bpRightArm2:
							from = P->vxBodyParts[i-1];
							size = vec2(-25.f, 0.f);
							break;
						case bpLeftLeg1:
						case bpRightLeg1:
							from = P->vxBodyParts[bpBase];
							size = vec2(-30.f, 0.f);
							break;
						case bpLeftLeg2:
						case bpRightLeg2:
							from = P->vxBodyParts[i-1];
							size = vec2(-30.f, 0.f);
							break;
							
						default:
							break;
					}
				 vxSetPosition(P->vxBodyParts[i], vec2Add(vxGetPosition(from), vec2Rotate(size, vec2(0.f, 0.f), -DEG2RAD(P->Angles.Angles[i]))));
				}
			}
			else
			{
				///@todo On pourrait ici obtenir l'angle du membre pour mettre à jour l'état du state mais
				if (i<bpHeadLeft)
				{
					Vertex* from;
					switch (i) {
						case bpNeck:
							from = P->vxBodyParts[bpBase];
							break;
						case bpLeftArm1:
						case bpRightArm1:
							from = P->vxBodyParts[bpNeck];
							break;
						case bpLeftArm2:
						case bpRightArm2:
						case bpLeftLeg2:
						case bpRightLeg2:
							from = P->vxBodyParts[i-1];
							break;
						case bpLeftLeg1:
						case bpRightLeg1:
							from = P->vxBodyParts[bpBase];
							break;
							
						default:
							break;
					}
					
					P->Angles.Angles[i] = RAD2DEG(vec2Angle(vec2Sub(vxGetPosition(P->vxBodyParts[i]), vxGetPosition(from))));
				}
				
				vxResolve(P->vxBodyParts[i], prevdt, dt);
				vxApplyForce(P->vxBodyParts[i], vec2(0.f, 0.6f), 1.f);
				if (i==9) //Tête
				{
					vxResolve(P->vxBodyParts[i+1], prevdt, dt);
					vxApplyForce(P->vxBodyParts[i+1], vec2(0.f, 0.6f), 1.f);
				}
			}
			
			
			if (A->Triggers & Triggering)
				if (ISNAN(Ang->Angles[i]) || ABS(P->Angles.Angles[i]-Ang->Angles[i]) < A->Diff)
					TriggerCount++;
			Triggering <<= 1; // *= 2
		}
		
		if (TriggerCount>=A->TriggerCount)
		{
			plGetAnimAnglesState(P)->CurrentState++;
			
			if (plGetAnimAnglesState(P)->CurrentState >= daGetSize(A->States))
			{
				plGetAnimAnglesState(P)->Ended = TRUE;
				if (A->Repeat)
					plGetAnimAnglesState(P)->CurrentState = 0;
				else
					plGetAnimAnglesState(P)->CurrentState--;
			}
		}
		
	}	
	
	
}

void aniUpdateForCurrentState(Animation* A, SPlayer* P)
{
	int i;
	
	
	if (A->Type == ANIM_POSITIONS)
	{
		//On fait un reset sur les changement de valeurs;
		for (i=0; i<20; i++)
			plGetAnimPositionsState(P)->Spd[i] = 0.f;
		
		AnimPositions* Pos;
		Pos = (AnimPositions*) daGet(A->States, plGetAnimPositionsState(P)->CurrentState);
		
		for (int i=0; i<10; i++)
		{
			if (!ISNAN(Pos->Positions[i].x))
				P->Positions.Positions[i].x = Pos->Positions[i].x;
			if (!ISNAN(Pos->Positions[i].y))
				P->Positions.Positions[i].y = Pos->Positions[i].y;
		}
	}
	else
	{
		//On fait un reset sur les changement de valeurs;
		for (i=0; i<10; i++)
			plGetAnimAnglesState(P)->Spd[i] = 0.f;
		AnimAngles* Ang;
		Ang = (AnimAngles*) daGet(A->States, plGetAnimAnglesState(P)->CurrentState);
		
		for (int i=0; i<10; i++)
			if (!ISNAN(Ang->Angles[i]))
				plGetAnimAnglesState(P)->Angles[i] = Ang->Angles[i];
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
	AnimTriggers Triggering = ANIM_NECK;
	
	AnimAngles* Angles = (AnimAngles*)daGet(A->States, State);
	
	for (int i=0; i<9; i++)
	{
		if (BodyPart & Triggering)
			Angles->Angles[i] = Angle;
		
		Triggering <<= 1;
	}
		
}

void aniSetBodyPartPosition(Animation* A, AnimTriggers BodyPart, Vec2 Pos, unsigned int State)
{
	assert(A->Type == ANIM_POSITIONS);
	AnimTriggers Triggering = ANIM_NECK;
	
	AnimPositions* Positions = (AnimPositions*)daGet(A->States, State);
	
	
	for (int i=0; i<9; i++)
	{
		if (BodyPart & Triggering)
			Positions->Positions[i] = Pos;
		
		Triggering <<= 1;
	}
}