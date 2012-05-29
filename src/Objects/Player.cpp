#include "Player.h"

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

	P->VxBalance = newVertex();
	vxSetPosition(P->VxBalance, P->ULPos);
	vxSetFixe(P->VxBalance, 1);
	P->ElBalance = newElastic(P->VxBalance, P->VxUR, -1.f, 0.02f);

	P->Shape = polyRectangle(P->VxUL, P->VxUR, P->VxDR, P->VxDL);
	wdAddVxFromPoly(W, P->Shape);

	P->GroundAngle = M_PI_2;

	P->Dir = DIR_RIGHT;

	/*
	vxSetPosition(P->VxUL, vec2Rotate(P->ULPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxUR, vec2Rotate(P->URPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxDL, vec2Rotate(P->DLPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxDR, vec2Rotate(P->DRPos, polyComputeCenter(P->Shape), P->GroundAngle));
	 */


	unsigned int i=0;
	for (i=0; i<polyGetVxCount(P->Shape); i++)
		vxSetFixe(polyGetVertex(P->Shape, i), FALSE);

	P->GrabR = NULL;
	P->GrabL = NULL;
	P->VxULStatus = P->VxURStatus = P->VxDRStatus = P->VxDLStatus =
		P->RdUStatus = P->RdRStatus = P->RdDStatus = P->RdLStatus = nullCollisionInfo();
	P->Speed = vec2(0.f, 0.f);
	P->State = PL_NOSTATE;
	plResetJump(P);

	/* On crée les vertices du personnage, pour l'animation et quand il meurt */
	P->Neck = newVertex(), P->HeadLeft = newVertex(), P->HeadRight = newVertex(), P->Base = newVertex(), P->LeftArm1 = newVertex(), P->LeftArm2 = newVertex(), P->RightArm1 = newVertex(), P->RightArm2 = newVertex(), P->LeftLeg1 = newVertex(), P->LeftLeg2 = newVertex(), P->RightLeg1 = newVertex(), P->RightLeg2 = newVertex();

	vxSetPosition(P->Base, vec2(0.f, 100.f));
	Vec2 B = vxGetPosition(P->Base);
	vxSetPosition(P->Neck, vec2(B.x, B.y - 60.f));
	 vxSetPosition(P->HeadLeft, vec2Add(vxGetPosition(P->Neck), vec2(-20.f, -20.f)));
	 vxSetPosition(P->HeadRight, vec2Add(vxGetPosition(P->Neck), vec2(20.f, -20.f)));
	 vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 25.f)));
	 vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 50.f)));
	 vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 25.f)));
	 vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 50.f)));
	 vxSetPosition(P->LeftLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 30.f)));
	vxSetPosition(P->LeftLeg2, vec2Add(vxGetPosition(P->Base), vec2(0.f, 60.f)));
	vxSetPosition(P->RightLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 30.f)));
	vxSetPosition(P->RightLeg2, vec2Add(vxGetPosition(P->Base), vec2(0.f, 60.f)));

	Rigid *LA1, *LA2, *RA1, *RA2, *Body, *LL1, *LL2, *RL1, *RL2, *H1, *H2, *H3;
	LA1 = newRigid(P->Neck, P->LeftArm1, -1.f);
	 LA2 = newRigid(P->LeftArm1, P->LeftArm2, -1.f);
	 RA1 = newRigid(P->Neck, P->RightArm1, -1.f);
	 RA2 = newRigid(P->RightArm1, P->RightArm2, -1.f);

	 LL1 = newRigid(P->Base, P->LeftLeg1, -1.f);
	 LL2 = newRigid(P->LeftLeg1, P->LeftLeg2, -1.f);
	 RL1 = newRigid(P->Base, P->RightLeg1, -1.f);
	 RL2 = newRigid(P->RightLeg1, P->RightLeg2, -1.f);


	Body = newRigid(P->Base, P->Neck, -1.f);

	H1 = newRigid(P->Neck, P->HeadLeft, -1.f);
	H2 = newRigid(P->Neck, P->HeadRight, -1.f);
	H3 = newRigid(P->HeadLeft, P->HeadRight, -1.f);

	wdAddRigid(W, Body);
	wdAddRigid(W, LA1);
	wdAddRigid(W, LA2);
	wdAddRigid(W, RA1);
	wdAddRigid(W, RA2);
	wdAddRigid(W, LL1);
	wdAddRigid(W, LL2);
	wdAddRigid(W, RL1);
	wdAddRigid(W, RL2);
	wdAddRigid(W, H1);
	wdAddRigid(W, H2);
	wdAddRigid(W, H3);

}

void plFree(Player* P)
{
	if(P->Shape != NULL) delPolygon(P->Shape);
	P->Shape = NULL;
	if(P->GrabR != NULL) delRigid(P->GrabR);
	if(P->GrabL != NULL) delRigid(P->GrabL);
	P->GrabR = NULL;
	P->GrabL = NULL;

	delVertex(P->VxBalance);
	delElastic(P->ElBalance);
	/* On élimne les vertex qui composent la BB du Player */
	/*
	delVertex(P->VxUL);
	delVertex(P->VxUR);
	delVertex(P->VxDL);
	delVertex(P->VxDR);
	 */

	delVertex(P->Neck);
	delVertex(P->Base);
	delVertex(P->HeadLeft);
	delVertex(P->HeadRight);
	delVertex(P->LeftArm1);
	delVertex(P->LeftArm2);
	delVertex(P->RightArm1);
	delVertex(P->RightArm2);
	delVertex(P->LeftLeg1);
	delVertex(P->LeftLeg2);
	delVertex(P->RightLeg1);
	delVertex(P->RightLeg2);

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

void plCorrectPosition(Player* Pl, Vec2 Pos)
{
	if(Pl->Shape != NULL)
	{
		vxSetPosition(Pl->VxUL, vec2Add(vxGetPosition(Pl->VxUL), Pos));
		vxSetPosition(Pl->VxUR, vec2Add(vxGetPosition(Pl->VxUR), Pos));
		vxSetPosition(Pl->VxDL, vec2Add(vxGetPosition(Pl->VxDL), Pos));
		vxSetPosition(Pl->VxDR, vec2Add(vxGetPosition(Pl->VxDR), Pos));
	}
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

void plJump(Player* P)
{
	if ((P->State & PL_ON_GROUND) && !P->Jumping)
	{
		polyApplyForce(P->Shape, vec2Prod(P->Normal, 20.f), 0);
		P->Jumping = TRUE;
		P->JumpVec = vec2Prod(P->Normal, 2.f);
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

void plUpdate(Player* P, World* W)
{
	/* Mise à jour spécifique de Player */

	P->Center = polyComputeCenter(P->Shape);

	P->RdUStatus = P->RdRStatus = P->RdDStatus =
	P->RdLStatus = P->VxURStatus = P->VxULStatus =
	P->VxDLStatus = P->VxDRStatus = nullCollisionInfo();
	polyResolve(plGetShape(P));
	List LExtracted = gridGetPolygonList(&W->CollisionGrid, P->Shape);

	Node* it;
	CollisionInfo Info;
	it = lstFirst(&LExtracted);
	P->State = PL_NOSTATE;
	P->Normal = vec2(0.f, 0.f);
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
	v = vec2Normalized(v);
	vxSetPosition(P->Base, vec2Add(P->Center, vec2Prod(v, -15.f)));
}

void plSetPosition(Player* P, float x, float y)
{
	/* Position relative par rapport à la base des vertex */
	/*Vec2 N, HL, HR, LA1, LA2, RA1, RA2, LL1, LL2, RL1, RL2;
	N =vec2Sub(vxGetPosition(P->Neck), vxGetPosition(P->Base));
	HL =vec2Sub(vxGetPosition(P->HeadLeft), vxGetPosition(P->Base));
	HR =vec2Sub(vxGetPosition(P->HeadRight), vxGetPosition(P->Base));
	LA1 =vec2Sub(vxGetPosition(P->LeftArm1), vxGetPosition(P->Base));
	LA2 =vec2Sub(vxGetPosition(P->LeftArm2), vxGetPosition(P->Base));
	RA1 =vec2Sub(vxGetPosition(P->RightArm1), vxGetPosition(P->Base));
	RA2 =vec2Sub(vxGetPosition(P->RightArm2), vxGetPosition(P->Base));
	LL1 =vec2Sub(vxGetPosition(P->LeftLeg1), vxGetPosition(P->Base));
	LL2 =vec2Sub(vxGetPosition(P->LeftLeg2), vxGetPosition(P->Base));
	RL1 =vec2Sub(vxGetPosition(P->RightLeg1), vxGetPosition(P->Base));
	RL2 =vec2Sub(vxGetPosition(P->RightLeg2), vxGetPosition(P->Base));


	vxSetPosition(P->Base, vec2(x, y));
	vxSetPosition(P->Neck, vec2(x+N.x, y+N.y));
	vxSetPosition(P->HeadLeft, vec2(x+HL.x, y+HL.y));
	vxSetPosition(P->HeadRight, vec2(x+HR.x, y+HR.y));
	vxSetPosition(P->LeftArm1, vec2(x+LA1.x, y+LA1.y));
	vxSetPosition(P->LeftArm2, vec2(x+LA2.x, y+LA2.y));
	vxSetPosition(P->RightArm1, vec2(x+RA1.x, y+RA1.y));
	vxSetPosition(P->RightArm2, vec2(x+RA2.x, y+RA2.y));
	vxSetPosition(P->LeftLeg1, vec2(x+LL1.x, y+LL1.y));
	vxSetPosition(P->LeftLeg2, vec2(x+LL2.x, y+LL2.y));
	vxSetPosition(P->RightLeg1, vec2(x+RL1.x, y+RL1.y));
	vxSetPosition(P->RightLeg2, vec2(x+RL2.x, y+RL2.y));
	 */

	Vec2 LL2, RL2;
	LL2 =vec2Sub(vxGetPosition(P->LeftLeg2), vxGetPosition(P->Base));
	RL2 =vec2Sub(vxGetPosition(P->RightLeg2), vxGetPosition(P->Base));
	vxSetPosition(P->Base, vec2(x, y));
	vxSetPosition(P->LeftLeg2, vec2(x+LL2.x, y+LL2.y));
	vxSetPosition(P->RightLeg2, vec2(x+RL2.x, y+RL2.y));


}
