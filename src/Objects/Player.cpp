#include "Player.h"

Player* newPlayer(World* W)
{
	Player* P = (Player*) malloc(sizeof(Player));
	plInit(P, W);
	return P;
}

void plInit(Player* P, World *W)
{

	P->ULPos = vec2(-20.f, -50.f);
	P->URPos = vec2(20.f, -50.f);
	P->DLPos = vec2(-35.f, 50.f);
	P->DRPos = vec2(35.f, 50.f);


	P->VxUL = newVertex();
	vxSetPosition(P->VxUL, P->ULPos);
	P->VxUR = newVertex();
	vxSetPosition(P->VxUR, P->URPos);
	P->VxDR = newVertex();
	vxSetPosition(P->VxDR, P->DRPos);
	P->VxDL = newVertex();
	vxSetPosition(P->VxDL, P->DLPos);

	vxSetMass(P->VxUL, 0.1f);
	vxSetMass(P->VxUR, 0.1f);
	vxSetMass(P->VxDL, 2.f);
	vxSetMass(P->VxDR, 2.f);

	P->VxBalance = newVertex();
	vxSetPosition(P->VxBalance, P->ULPos);
	vxSetFixe(P->VxBalance, 1);
	P->ElBalance = newElastic(P->VxBalance, P->VxUR, -1.f, 0.02f);

	P->Shape = polyRectangle(P->VxUL, P->VxUR, P->VxDR, P->VxDL);

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

	P->Grab = NULL;
	P->VxULStatus = P->VxURStatus = P->VxDRStatus = P->VxDLStatus =
		P->RdUStatus = P->RdRStatus = P->RdDStatus = P->RdLStatus = nullCollisionInfo();
	P->Speed = vec2(0.f, 0.f);
	P->OnGround = FALSE;
	plResetJump(P);

	/* On crée les vertices du personnage, pour l'animation et quand il meurt */
	P->Neck = newVertex(), P->HeadLeft = newVertex(), P->HeadRight = newVertex(), P->Base = newVertex(), P->LeftArm1 = newVertex(), P->LeftArm2 = newVertex(), P->RightArm1 = newVertex(), P->RightArm2 = newVertex(), P->LeftLeg1 = newVertex(), P->LeftLeg2 = newVertex(), P->RightLeg1 = newVertex(), P->RightLeg2 = newVertex();

	vxSetPosition(P->Base, vec2(0.f, 130.f));
	Vec2 B = vxGetPosition(P->Base);
	vxSetPosition(P->Neck, vec2(B.x, B.y - 90.f));
	 vxSetPosition(P->HeadLeft, vec2Add(vxGetPosition(P->Neck), vec2(-30.f, -40.f)));
	 vxSetPosition(P->HeadRight, vec2Add(vxGetPosition(P->Neck), vec2(30.f, -40.f)));
	 vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	 vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	 vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	 vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	 vxSetPosition(P->LeftLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));
	vxSetPosition(P->LeftLeg2, vec2Add(vxGetPosition(P->Base), vec2(10.f, 80.f)));
	vxSetPosition(P->RightLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));
	vxSetPosition(P->RightLeg2, vec2Add(vxGetPosition(P->Base), vec2(-10.f, 80.f)));

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
	if(P->Grab != NULL) delRigid(P->Grab);
	P->Grab = NULL;

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

void plSetPosition(Player* Pl, Vec2 Pos)
{
	if(Pl->Shape != NULL)
		vxSetPosition(polyGetVertex(Pl->Shape, 0), Pos);
}

void plSetShape(Player* P, Polygon* Shape)
{
	P->Shape = Shape;
	/*unsigned int i=0;
	for (i=0; i<polyGetVxCount(P->Shape); i++)
		vxSetFixe(polyGetVertex(P->Shape, i), TRUE);
	 */
}

void plMoveR(Player* P)
{
	if(vec2Sub(vxGetPosition(P->VxUR), vxGetOldPos(P->VxUR)).x > 20.f) return; // Limite de Vitesse

	if(P->VxDLStatus.P1 != NULL || P->VxDRStatus.P1 != NULL
		|| P->RdDStatus.P1 != NULL)
	{
		//P->Speed.x+=1.5f;
		polyApplyForce(P->Shape, vec2Prod(P->GroundVec, 1.5f), 0);
	}
	else
	{
		//P->Speed.x+=0.5f;
		polyApplyForce(P->Shape, vec2Prod(P->GroundVec, 1.5f), 0);
	}
}

void plMoveL(Player* P)
{
	if(vec2Sub(vxGetPosition(P->VxUR), vxGetOldPos(P->VxUR)).x < -20.f) return;

	if(P->VxDLStatus.P1 != NULL || P->VxDRStatus.P1 != NULL
		|| P->RdDStatus.P1 != NULL)
	{
		//P->Speed.x-=1.5f;
		polyApplyForce(P->Shape, vec2(-1.5f, 0.f), 0);
	}
	else
	{
		//P->Speed.x-=0.5f;
		polyApplyForce(P->Shape, vec2(-0.5f, 0.f), 0);
	}
}

void plJump(Player* P)
{
	if (P->OnGround && !P->Jumping)
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
	//printf("Onground: %u, Normal : %f, %f\n", P->OnGround, P->Normal.x, P->Normal.y);
}

void plResetJump(Player* P)
{
	P->Jumping = FALSE;
	P->JumpTimer = 10.f;
}

void plGetUp(Player* P)
{

	P->Center = polyComputeCenter(P->Shape);

	if (0)//P->OnGround)
	{
		vxChangePosition(P->VxUL, vec2Add(P->Center, vec2Rotate(P->ULPos, P->Center, P->GroundAngle)));
		vxChangePosition(P->VxUR, vec2Add(P->Center, vec2Rotate(P->URPos, P->Center, P->GroundAngle)));
		vxChangePosition(P->VxDL, vec2Add(P->Center, vec2Rotate(P->DLPos, P->Center, P->GroundAngle)));
		vxChangePosition(P->VxDR, vec2Add(P->Center, vec2Rotate(P->DRPos, P->Center, P->GroundAngle)));

	}
	else
	{
		vxChangePosition(P->VxUL, vec2Add(P->Center, P->ULPos));
		vxChangePosition(P->VxUR, vec2Add(P->Center, P->URPos));
		vxChangePosition(P->VxDL, vec2Add(P->Center, P->DLPos));
		vxChangePosition(P->VxDR, vec2Add(P->Center, P->DRPos));
	}
	//printf("pos correc: %f, %f; with angle: %f, %f\n", vec2Add(P->Center, P->ULPos).x,  vec2Add(P->Center, P->ULPos).y, vec2Rotate(P->ULPos, P->Center, P->GroundAngle).x, vec2Rotate(P->ULPos, P->Center, P->GroundAngle).y);
	//vxApplyForce(P->VxUR, vec2(0.f, -15.f), 1.f);
	/*if (1)//P->OnGround)
	{
		if (P->Normal.x!=0.f || P->Normal.y!=0.f)
		{
			vxSetPosition(P->VxBalance, vec2Add(vxGetPosition(P->VxDL), vec2Prod(P->Normal, 210.f)));
			//printf("Normal: %f, %f\n", Normal.x, Normal.y);
			//printf("P pos: %f, %f; vxpos: %f, %f\n", vxGetPosition(P->VxUL).x, vxGetPosition(P->VxUL).y, vxGetPosition(P->VxBalance).x, vxGetPosition(P->VxBalance).y);
			for (int i=0; i<4; i++)
				elResolve(P->ElBalance);
		}
	}*/
}

void plGrab(Player* P, World* W, float MouseX, float MouseY)
{
	Vertex* tmpVx = wdGetNearest(W, MouseX, MouseY);
	if(vec2Length(vec2Sub(vxGetPosition(tmpVx), vxGetPosition(plGetVxUR(P)))) < 400.f)
	{
		P->Grab = newRigid(plGetVxUR(P), tmpVx, 3.f);//, 0.1f);
		wdAddRigid(W, P->Grab);
		//wdAddElastic(W, P->Grab);
	}
}

void plRelease(Player* P, World* W)
{
	if(P->Grab == NULL) return;
	wdDelRigid(W, P->Grab);
	delRigid(P->Grab);
	//wdDelElastic(W, P->Grab);
	//delElastic(P->Grab);
	P->Grab = NULL;
}

void plUpdate(Player* P, World* W)
{
	/*
	vxSetPosition(P->VxUL, vec2Rotate(P->ULPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxUR, vec2Rotate(P->URPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxDL, vec2Rotate(P->DLPos, polyComputeCenter(P->Shape), P->GroundAngle));
	vxSetPosition(P->VxDR, vec2Rotate(P->DRPos, polyComputeCenter(P->Shape), P->GroundAngle));
	 */

	//printf("Onground: %u; speed: %f, %f\n", P->OnGround, P->Speed.x, P->Speed.y);



	/*
	for (i=0; i<polyGetVxCount(P->Shape); i++)
	{
		V = polyGetVertex(P->Shape, i);
		vxMove(V, P->Speed);
	}
	 */


	//FAAAAIL!
	P->Center = polyComputeCenter(P->Shape);
	

	/* Mise à jour spécifique de Player */


	P->RdUStatus = P->RdRStatus = P->RdDStatus =
	P->RdLStatus = P->VxURStatus = P->VxULStatus =
	P->VxDLStatus = P->VxDRStatus = nullCollisionInfo();
	polyResolve(plGetShape(P));
	List LExtracted = gridGetPolygonList(&W->CollisionGrid, P->Shape);

	Node* it;
	CollisionInfo Info;
	it = lstFirst(&LExtracted);
	P->OnGround = (P->VxDLStatus.P1 != NULL || P->VxDRStatus.P1 != NULL || P->RdDStatus.P1 != NULL);
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
				P->OnGround = TRUE;
			} else if(Info.V == plGetVxDL(P) || Info.V == plGetVxDR(P))	{
				P->GroundAngle = vec2Angle(Info.Normal)-M_PI_2;
				P->Normal = Info.Normal;
				P->OnGround = TRUE; // printf("Grouuuuuuund; normal angle : %f\n", RAD2DEG(P->GroundAngle));
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
	//FAAAAIL!
//	P->Center = polyComputeCenter(P->Shape);
//
//	vxChangePosition(P->VxUL, vec2Add(P->Center, P->ULPos));
//	vxChangePosition(P->VxUR, vec2Add(P->Center, P->URPos));
//	vxChangePosition(P->VxDL, vec2Add(P->Center, P->DLPos));
//	vxChangePosition(P->VxDR, vec2Add(P->Center, P->DRPos));


	Vec2 v = vec2Sub(vxGetPosition(P->VxUL), vxGetPosition(P->VxDL));
	v = vec2Normalized(v);
	vxSetPosition(P->Base, vec2Add(P->Center, vec2Prod(v, -25.f)));

	//P->OnGround = (P->VxDLStatus.P1 != NULL || P->VxDRStatus.P1 != NULL || P->RdDStatus.P1 != NULL);


	if (P->OnGround)
	{
		float Force = 0.03f;
		float Diff = vxGetPosition(P->VxUL).y - vxGetPosition(P->VxUR).y;
		if(abs(Diff) < 10.f) Diff = 0;
		if(Diff > 0)
			vxApplyForce(P->VxUL, vec2(0.f, -Force*(Diff)), 0),
			vxApplyForce(P->VxUR, vec2(-Force*(Diff), 0), 0),
			vxApplyForce(P->VxDL, vec2(Force*(Diff), 0), 0),
			vxApplyForce(P->VxDR, vec2(0.f, Force*(Diff)), 0);
		if(Diff < 0)
			vxApplyForce(P->VxUR, vec2(0.f, Force*(Diff)), 0),
			vxApplyForce(P->VxUL, vec2(-Force*(Diff), 0), 0),
			vxApplyForce(P->VxDR, vec2(Force*(Diff), 0), 0),
			vxApplyForce(P->VxDL, vec2(0.f, -Force*(Diff)), 0);
	}

}

void plCreateVertex(Player* P, World* W)
{
	/*P->Neck = newVertex(), P->HeadLeft = newVertex(), P->HeadRight = newVertex(), P->Base = newVertex(), P->LeftArm1 = newVertex(), P->LeftArm2 = newVertex(), P->RightArm1 = newVertex(), P->RightArm2 = newVertex(), P->LeftLeg1 = newVertex(), P->LeftLeg2 = newVertex(), P->RightLeg1 = newVertex(), P->RightLeg2 = newVertex();*/

	P->Base = newVertex();
	P->LeftLeg2 = newVertex();
	P->RightLeg2 = newVertex();

	/*vxSetFixe(P->Base, TRUE);
	vxSetFixe(P->Neck, TRUE);
	vxSetFixe(P->HeadLeft, TRUE);
	 */

	/*wdAddVertex(W, P->Neck);
	wdAddVertex(W, P->HeadLeft);
	wdAddVertex(W, P->HeadRight);
	 */
	wdAddVertex(W, P->Base);
	/*wdAddVertex(W, P->LeftArm1);
	wdAddVertex(W, P->LeftArm2);
	wdAddVertex(W, P->RightArm1);
	wdAddVertex(W, P->RightArm2);
	wdAddVertex(W, P->LeftLeg1);*/
	wdAddVertex(W, P->LeftLeg2);
	//wdAddVertex(W, P->RightLeg1);
	wdAddVertex(W, P->RightLeg2);

}

void plCreateRigids(Player* P, World* W)
{
	/* On met en place les parties du corps par rapport à la base */
	Vec2 B = vxGetPosition(P->Base);
	/*vxSetPosition(P->Neck, vec2(B.x, B.x - 90.f));
	vxSetPosition(P->HeadLeft, vec2Add(vxGetPosition(P->Neck), vec2(-30.f, -40.f)));
	vxSetPosition(P->HeadRight, vec2Add(vxGetPosition(P->Neck), vec2(30.f, -40.f)));
	vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	vxSetPosition(P->LeftLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));*/
	vxSetPosition(P->LeftLeg2, vec2Add(vxGetPosition(P->Base), vec2(10.f, 80.f)));
	//vxSetPosition(P->RightLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));
	vxSetPosition(P->RightLeg2, vec2Add(vxGetPosition(P->Base), vec2(-10.f, 80.f)));

	Bool usePolys=1;

	if (!usePolys)
	{
		Rigid *LA1, *LA2, *RA1, *RA2, *Body, *LL1, *LL2, *RL1, *RL2, *H1, *H2, *H3;
		/*LA1 = newRigid(P->Neck, P->LeftArm1, -1.f);
		LA2 = newRigid(P->LeftArm1, P->LeftArm2, -1.f);
		RA1 = newRigid(P->Neck, P->RightArm1, -1.f);
		RA2 = newRigid(P->RightArm1, P->RightArm2, -1.f);

		LL1 = newRigid(P->Base, P->LeftLeg1, -1.f);
		LL2 = newRigid(P->LeftLeg1, P->LeftLeg2, -1.f);
		RL1 = newRigid(P->Base, P->RightLeg1, -1.f);
		RL2 = newRigid(P->RightLeg1, P->RightLeg2, -1.f);*/


		Body = newRigid(P->Base, P->Neck, -1.f);

		H1 = newRigid(P->Base, P->HeadLeft, -1.f);
		H2 = newRigid(P->Base, P->HeadRight, -1.f);
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
	else
	{
		Polygon *LA1, *LA2, *RA1, *RA2, *Body, *LL1, *LL2, *RL1, *RL2, *head;

		/*head = newPolygon(3, P->Neck, P->HeadLeft, P->HeadRight);
		wdAddPolygon(W, head);

		LA1 = newPolygon(2, P->Neck, P->LeftArm1);
		LA2 = newPolygon(2, P->LeftArm1, P->LeftArm2);
		RA1 = newPolygon(2, P->Neck, P->RightArm1);
		RA2 = newPolygon(2, P->RightArm1, P->RightArm2);

		LL1 = newPolygon(2, P->Base, P->LeftLeg1);
		LL2 = newPolygon(2, P->LeftLeg1, P->LeftLeg2);
		RL1 = newPolygon(2, P->Base, P->RightLeg1);
		RL2 = newPolygon(2, P->RightLeg1, P->RightLeg2);

		Body = newPolygon(2, P->Base, P->Neck);

		wdAddPolygon(W, Body);
		wdAddPolygon(W, LA1);
		wdAddPolygon(W, LA2);
		wdAddPolygon(W, RA1);
		wdAddPolygon(W, RA2);
		wdAddPolygon(W, LL1);
		wdAddPolygon(W, LL2);
		wdAddPolygon(W, RL1);
		wdAddPolygon(W, RL2);
		 */
		LL2 = newPolygon(2, P->Base, P->LeftLeg2);
		RL2 = newPolygon(2, P->Base, P->RightLeg2);

		wdAddPolygon(W, LL2);
		wdAddPolygon(W, RL2);

		Elastic* E;
		E = newElastic(P->LeftLeg2, P->RightLeg2, -1.f, 1.f);
		wdAddElastic(W, E);
	}

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
