#include "Player.h"
#include <Game/SharedResources.h>

Player* newPlayer(World* W)
{
	Player* P = (Player*) malloc(sizeof(Player));
	plInit(P, W);
	return P;
}

void plInit(Player* P, World *W)
{
	P->ULPos = vec2(-20.f, -70.f);
	P->URPos = vec2(20.f, -70.f);
	P->DLPos = vec2(-35.f, 70.f);
	P->DRPos = vec2(35.f, 70.f);

	strcpy(P->SndFoot[0], "snd_step1");
	strcpy(P->SndFoot[1], "snd_step2");

	P->timer.restart();

	P->VxUL = newVertex();
	vxSetPosition(P->VxUL, P->ULPos);
	P->VxUR = newVertex();
	vxSetPosition(P->VxUR, P->URPos);
	P->VxDR = newVertex();
	vxSetPosition(P->VxDR, P->DRPos);
	P->VxDL = newVertex();
	vxSetPosition(P->VxDL, P->DLPos);

	vxSetMass(P->VxUL, 0.01f);
	vxSetMass(P->VxUR, 0.01f);
	vxSetMass(P->VxDL, 1.5f);
	vxSetMass(P->VxDR, 1.5f);

	P->Shape = polyRectangle(P->VxUL, P->VxUR, P->VxDR, P->VxDL);
	wdAddVxFromPoly(W, P->Shape);

	P->GroundAngle = M_PI_2;

	P->Dir = DIR_RIGHT;

	animAnglesStatesInit(&P->Angles);
	animPositionsStatesInit(&P->Positions);

	P->aniRun = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(P->aniRun, "data/anims/animRun.ani");
	//aniSetForce(P->aniRun, 0.65f);

	P->aniJump = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(P->aniJump, "data/anims/animJump.ani");
	aniSetForce(P->aniJump, 0.65f);

	P->aniFall = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(P->aniFall, "data/anims/animFall.ani");

	P->aniHello = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(P->aniHello, "data/anims/animHello.ani");

	P->aniStand = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(P->aniStand, "data/anims/animStand.ani");

	unsigned int i=0;
	for (i=0; i<polyGetVxCount(P->Shape); i++)
		vxSetFixed(polyGetVertex(P->Shape, i), FALSE);

	P->GrabR = NULL;
	P->GrabL = NULL;
	P->VxULStatus = P->VxURStatus = P->VxDRStatus = P->VxDLStatus =
		P->RdUStatus = P->RdRStatus = P->RdDStatus = P->RdLStatus = nullCollisionInfo();
	P->State = PL_NOSTATE;
	plResetJump(P);

	/* On crée les vertices du personnage, pour l'animation et quand il meurt */
	for (int i=0; i<12; i++)
		P->vxBodyParts[i] = newVertex();

	vxSetPosition(P->vxBodyParts[bpBase], vec2(0.f, 100.f));
	Vec2 B = vxGetPosition(P->vxBodyParts[bpBase]);
	vxSetPosition(P->vxBodyParts[bpNeck], vec2(B.x, B.y - 60.f));
	 vxSetPosition(P->vxBodyParts[bpHeadLeft], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(-20.f, -20.f)));
	 vxSetPosition(P->vxBodyParts[bpHeadRight], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(20.f, -20.f)));
	 vxSetPosition(P->vxBodyParts[bpLeftArm1], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(0.f, 25.f)));
	 vxSetPosition(P->vxBodyParts[bpLeftArm2], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(0.f, 50.f)));
	 vxSetPosition(P->vxBodyParts[bpRightArm1], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(0.f, 25.f)));
	 vxSetPosition(P->vxBodyParts[bpRightArm2], vec2Add(vxGetPosition(P->vxBodyParts[bpNeck]), vec2(0.f, 50.f)));
	 vxSetPosition(P->vxBodyParts[bpLeftLeg1], vec2Add(vxGetPosition(P->vxBodyParts[bpBase]), vec2(0.f, 30.f)));
	vxSetPosition(P->vxBodyParts[bpLeftLeg2], vec2Add(vxGetPosition(P->vxBodyParts[bpBase]), vec2(0.f, 60.f)));
	vxSetPosition(P->vxBodyParts[bpRightLeg1], vec2Add(vxGetPosition(P->vxBodyParts[bpBase]), vec2(0.f, 30.f)));
	vxSetPosition(P->vxBodyParts[bpRightLeg2], vec2Add(vxGetPosition(P->vxBodyParts[bpBase]), vec2(0.f, 60.f)));

	for (int i=0; i<12; i++)
	{
		switch (i) {
			case bpNeck:
			case bpLeftLeg1:
			case bpRightLeg1:
				P->BodyRigids[i] = newRigid(P->vxBodyParts[i], P->vxBodyParts[bpBase], -1.f);
				break;
			case bpLeftArm1:
			case bpRightArm1:
			case bpHeadLeft:
			case bpHeadRight:
				P->BodyRigids[i] = newRigid(P->vxBodyParts[i], P->vxBodyParts[bpNeck], -1.f);
				break;
			case bpLeftArm2:
			case bpRightArm2:
			case bpLeftLeg2:
			case bpRightLeg2:
				P->BodyRigids[i] = newRigid(P->vxBodyParts[i], P->vxBodyParts[i-1], -1.f);
				break;
			case bpBase:
				P->BodyRigids[i] = newRigid(P->vxBodyParts[bpHeadLeft], P->vxBodyParts[bpHeadRight], -1.f);
				break;
			default:
				break;
		}
	}

	P->CurrentAnim = P->aniStand;
	aniUpdateForCurrentState(P->CurrentAnim, P);
	aniUpdate(P->CurrentAnim, P, 1.f);

}

void plFree(Player* P)
{
	if(P->Shape != NULL) delPolygon(P->Shape);
	P->Shape = NULL;
	if(P->GrabR != NULL) delRigid(P->GrabR);
	if(P->GrabL != NULL) delRigid(P->GrabL);
	P->GrabR = NULL;
	P->GrabL = NULL;

	for (int i=0; i<12; i++)
		delVertex(P->vxBodyParts[i]),
		delRigid(P->BodyRigids[i]);

	delAnimation(P->aniFall);
	delAnimation(P->aniJump);
	delAnimation(P->aniRun);
	delAnimation(P->aniHello);
	delAnimation(P->aniStand);
}

void delPlayer(Player* P)
{
	plFree(P);
	free(P);
}

Polygon* plGetShape(Player* P)
{
	return P->Shape;
}

Vertex* plGetVxUL(Player* P)
{
	return polyGetVertex(P->Shape, 0);
}

Vertex* plGetVxUR(Player* P)
{
	return polyGetVertex(P->Shape, 1);
}

Vertex* plGetVxDR(Player* P)
{
	return polyGetVertex(P->Shape, 2);
}

Vertex* plGetVxDL(Player* P)
{
	return polyGetVertex(P->Shape, 3);
}

Rigid* plGetRdU(Player* P)
{
	return polyGetRigid(P->Shape, 0);
}

Rigid* plGetRdR(Player* P)
{
	return polyGetRigid(P->Shape, 1);
}

Rigid* plGetRdD(Player* P)
{
	return polyGetRigid(P->Shape, 2);
}

Rigid* plGetRdL(Player* P)
{
	return polyGetRigid(P->Shape, 3);
}

void plCorrectPosition(Player* P, Vec2 Pos)
{
	if(P->Shape != NULL)
	{
		vxSetPosition(P->VxUL, vec2Add(vxGetPosition(P->VxUL), Pos));
		vxSetPosition(P->VxUR, vec2Add(vxGetPosition(P->VxUR), Pos));
		vxSetPosition(P->VxDL, vec2Add(vxGetPosition(P->VxDL), Pos));
		vxSetPosition(P->VxDR, vec2Add(vxGetPosition(P->VxDR), Pos));
	}

	Vec2 v = vec2Sub(vxGetPosition(P->VxUL), vxGetPosition(P->VxDL));
	P->PrevCenter = P->Center;
	P->Center = polyComputeCenter(P->Shape);
	v = vec2Normalized(v);
	vxSetPosition(P->vxBodyParts[bpBase], vec2Add(P->Center, vec2Prod(v, -15.f)));

	aniUpdateForCurrentState(P->aniStand, P);
	aniUpdate(P->aniStand, P, 1.f);
}

void plSetShape(Player* P, Polygon* Shape)
{
	P->Shape = Shape;
}

void plMoveR(Player* P)
{
	if(vec2Sub(vxGetPosition(P->VxUR), vxGetOldPos(P->VxUR)).x > 20.f) return; // Limite de Vitesse

	if(P->State & PL_HAS_SUPPORT)
	{
		if(!(P->State & PL_FALLING))
		{
			polyApplyForce(P->Shape, vec2(2.f, 0.f), 0);
		}
	} else {
		polyApplyForce(P->Shape, vec2(0.5f, 0.f), 0);
	}
}

void plMoveL(Player* P)
{
	if(vec2Sub(vxGetPosition(P->VxUR), vxGetOldPos(P->VxUR)).x < -20.f) return;

	if(P->State & PL_HAS_SUPPORT)
	{
		if(!(P->State & PL_FALLING))
		{
			polyApplyForce(P->Shape, vec2(-2.f, 0.f), 0);
		}
	}
	else
	{
		polyApplyForce(P->Shape, vec2(-0.5f, 0.f), 0);
	}
}

void plRotateR(Player* P)
{
	float Force = 0.2f;
	vxApplyForce(P->VxUL, vec2(Force, -Force), 0);
	vxApplyForce(P->VxUR, vec2(Force, Force), 0);
	vxApplyForce(P->VxDL, vec2(-Force, -Force), 0);
	vxApplyForce(P->VxDR, vec2(-Force, Force), 0);
}

void plRotateL(Player* P)
{
	float Force = 0.2f;
	vxApplyForce(P->VxUR, vec2(-Force, -Force), 0);
	vxApplyForce(P->VxUL, vec2(-Force, Force), 0);
	vxApplyForce(P->VxDR, vec2(Force, -Force), 0);
	vxApplyForce(P->VxDL, vec2(Force, Force), 0);
}

void plJump(Player* P, s_SharedResources* SR)
{
	if ((P->State & PL_ON_GROUND) && !P->Jumping)
	{
		polyApplyForce(P->Shape, vec2Prod(P->Normal, 20.f), 0);
		P->Jumping = TRUE;
		P->JumpVec = vec2Prod(P->Normal, 2.f);
		sndmPlay(shSoundManager(SR), "snd_step1");
	}
	else if (P->Jumping)
	{
		if (P->JumpTimer>0.f)
		{
			P->JumpTimer-=1.f;
			polyApplyForce(P->Shape, P->JumpVec, 0);
		}
		else
		{
			plResetJump(P);
		}
	}
}

void plResetJump(Player* P)
{
	P->Jumping = FALSE;
	P->JumpTimer = 10.f;
}

void plGetUp(Player* P)
{
	if((fabs((vec2Sub(vxGetPosition(P->VxDR), vxGetPosition(P->VxDL))).y) > 30.f) && (P->State & PL_ON_GROUND))
	{
		vxApplyForce(P->VxUL, vec2(0.f, -0.2f), 0);
		vxApplyForce(P->VxUR, vec2(0.f, -0.2f), 0);
	}
}

void plGrabR(Player* P, World* W, float MouseX, float MouseY)
{
	Vertex* tmpVx = wdGetNearest(W, MouseX, MouseY);
	if(tmpVx != NULL && vec2Length(vec2Sub(vxGetPosition(tmpVx), vxGetPosition(plGetVxUR(P)))) < 60.f)
	{
		P->GrabR = newRigid(plGetVxUR(P), tmpVx, 50.f);
		wdAddRigid(W, P->GrabR);
	}
}

void plGrabL(Player* P, World* W, float MouseX, float MouseY)
{
	Vertex* tmpVx = wdGetNearest(W, MouseX, MouseY);
	if(tmpVx != NULL && vec2Length(vec2Sub(vxGetPosition(tmpVx), vxGetPosition(plGetVxUL(P)))) < 60.f)
	{
		P->GrabL = newRigid(plGetVxUL(P), tmpVx, 50.f);
		wdAddRigid(W, P->GrabL);
	}
}

void plReleaseR(Player* P, World* W)
{
	if(P->GrabR == NULL) return;
	wdDelRigid(W, P->GrabR);
	delRigid(P->GrabR);
	//wdDelElastic(W, P->Grab);
	//delElastic(P->Grab);
	P->GrabR = NULL;
}

void plReleaseL(Player* P, World* W)
{
	if(P->GrabL == NULL) return;
	wdDelRigid(W, P->GrabL);
	delRigid(P->GrabL);
	//wdDelElastic(W, P->Grab);
	//delElastic(P->Grab);
	P->GrabL = NULL;
}

void plUpdate(Player* P, s_SharedResources* SR)
{

	P->GroundVec = vec2Ortho(P->Normal);

	// Mise à jour quelques States
	Vec2 RdBottom = vec2Normalized(vec2Sub(vxGetPosition(P->VxDR), vxGetPosition(P->VxDL)));

	if(RdBottom.x < 0.f)
	{
		P->State = P->State | PL_UPSIDEDOWN;
		P->State = P->State | PL_FALLING;
	} else {
		if(RdBottom.y < -0.5f)
		{
			P->State = P->State | PL_FALLING;
			P->State = P->State | PL_FALLING_L;
		} else if(RdBottom.y > 0.5f) {
			P->State = P->State | PL_FALLING;
			P->State = P->State | PL_FALLING_R;
		}
	}

	if(P->GrabL != NULL) P->State = P->State | PL_GRABL;
	if(P->GrabR != NULL) P->State = P->State | PL_GRABR;

	Vec2 v = vec2Sub(vxGetPosition(P->VxUL), vxGetPosition(P->VxDL));
	P->PrevCenter = P->Center;
	P->Center = polyComputeCenter(P->Shape);
	v = vec2Normalized(v);
	vxSetPosition(P->vxBodyParts[bpBase], vec2Add(P->Center, vec2Prod(v, -15.f)));


	Vec2 speed = vec2Sub(P->Center, P->PrevCenter);
	//printf("speed:%f,%f\n",speed.x, speed.y);


	if (!(P->State & PL_ON_GROUND) && ABS(speed.y) > 0.1f)
	{
		if (speed.y<2.f)
			P->CurrentAnim = P->aniJump;
		else
			P->CurrentAnim = P->aniFall;
		P->timer.restart();
	}
	else
	{
		if (ABS(speed.x) > 1.f)
			P->CurrentAnim = P->aniRun, P->timer.restart();
		else if (P->timer.getElapsedTime().asSeconds() < 5.f)
			P->CurrentAnim = P->aniStand;
		else
			P->CurrentAnim = P->aniHello;
	}

	if (P->timer.getElapsedTime().asSeconds() > 20.f)
		P->timer.restart();

	unsigned int LastState;

	if (aniGetType(P->CurrentAnim) == ANIM_POSITIONS)
		LastState = P->Positions.CurrentState;
	else
		LastState = P->Angles.CurrentState;


	if (ABS(speed.x) > 1.f && P->CurrentAnim == P->aniRun)
	{
		float spp = vec2Length(speed);
		aniUpdate(P->CurrentAnim, P, MAX(0.25f, spp/10.f));
	}
	else
		aniUpdate(P->CurrentAnim, P, 1.f);
		

	if (aniGetType(P->CurrentAnim) == ANIM_POSITIONS)
	{
		if (P->CurrentAnim == P->aniRun && LastState != P->Positions.CurrentState)
			sndmPlay(shSoundManager(SR), P->SndFoot[LastState]);
	}
	else
	{
		if (P->CurrentAnim == P->aniRun && LastState != P->Angles.CurrentState)
			sndmPlay(shSoundManager(SR), P->SndFoot[LastState]);
	}

}

float plGetCurrentStateAngle(Player* P, BodyParts bp)
{
	assert(aniGetType(P->CurrentAnim) == ANIM_ANGLES);
	AnimAngles* A =(AnimAngles*)daGet(P->CurrentAnim->States, P->Angles.CurrentState);
	return A->Angles[bp];
}

void plPhysics(Player* P, World* W)
{
	/* Mise à jour spécifique de Player */


	P->RdUStatus = P->RdRStatus = P->RdDStatus =
	P->RdLStatus = P->VxURStatus = P->VxULStatus =
	P->VxDLStatus = P->VxDRStatus = nullCollisionInfo();
	polyResolve(plGetShape(P));

	//for (i=0; i<10; i++) polyResolve(P->BodyPolygons[i]);
	for (int i=0; i<12; i++)
	{
		vxResolve(P->vxBodyParts[i], 0.5f, 0.5f);
		rdResolve(P->BodyRigids[i]);
	}

	float dif = vxGetPosition(P->VxDL).x - vxGetOldPos(P->VxDL).x;
	if (dif >= 0.f && ABS(dif) > 0.2f)
		P->Dir = DIR_RIGHT;
	else
		P->Dir = DIR_LEFT;

	List LExtracted = gridGetPolygonList(&W->CollisionGrid, P->Shape);

	Node* it;
	CollisionInfo Info;

	P->State = PL_NOSTATE;
	P->Normal = vec2(0.f, 0.f);

	it = lstFirst(&LExtracted);
	while(!nodeEnd(it))
	{

		Info = polyCollide(plGetShape(P), (Polygon*) nodeGetData(it));
		if(Info.P1 != NULL)
		{
			//printf("Collision\n");
			if (Info.Edge == plGetRdD(P))
			{
				P->GroundAngle = vec2Angle(Info.Normal)-M_PI_2;
				P->Normal = vec2Prod(Info.Normal, -1.f);
				P->State = P->State | PL_HAS_SUPPORT;
				if(P->Normal.y < -0.5f) P->State = P->State | PL_ON_GROUND;
			} else if(Info.V == plGetVxDL(P) || Info.V == plGetVxDR(P))	{
				P->GroundAngle = vec2Angle(Info.Normal)-M_PI_2;
				P->Normal = Info.Normal;
				P->State = P->State | PL_HAS_SUPPORT;
				if(P->Normal.y < -0.5f) P->State = P->State | PL_ON_GROUND;
			}


			/* Test des propriétés de la collision */
			if(Info.Edge == plGetRdU(P)) P->RdUStatus = Info;
			else if(Info.Edge == plGetRdR(P)) P->RdRStatus = Info;
			else if(Info.Edge == plGetRdD(P)) P->RdDStatus = Info;
			else if(Info.Edge == plGetRdL(P)) P->RdLStatus = Info;
			if(Info.V == plGetVxUL(P)) P->VxULStatus = Info;
			else if(Info.V == plGetVxUR(P)) P->VxURStatus = Info;
			else if(Info.V == plGetVxDR(P)) P->VxDRStatus = Info;
			else if(Info.V == plGetVxDL(P)) P->VxDLStatus = Info;


			polyHandleCollision(Info);
		}
		it = nodeGetNext(it);
	}
	lstFree(&LExtracted);
}


AnimAngles* plGetAnimAnglesState(Player* P)
{
	return &P->Angles;
}

AnimPositions* plGetAnimPositionsState(Player* P)
{
	return &P->Positions;
}
