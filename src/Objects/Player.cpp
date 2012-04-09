#include "Player.h"

Player* newPlayer()
{
	Player* P = (Player*) malloc(sizeof(Player));
	plInit(P);
	return P;
}

void plInit(Player* P)
{
	P->Shape = NULL;
	P->Grab = NULL;
	P->Stable = NULL;
}

void plFree(Player* P)
{
	if(P->Shape != NULL) delPolygon(P->Shape);
	P->Shape = NULL;
	if(P->Grab != NULL) delElastic(P->Grab);
	P->Grab = NULL;
	
	if (P->Stable != NULL) delVertex(P->Stable);
	P->Stable = NULL;
}

void delPlayer(Player* P)
{
	plFree(P);
	free(P);
}

Bool plGetOnGround(Player* P)
{
	return P->OnGround;
}

Vec2 plGetGroundNormal(Player* P)
{
	return P->GroundNormal;
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

void plSetOnGround(Player* P, Bool B)
{
	P->OnGround = B;
}


void plSetGroundNormal(Player* P, Vec2 N)
{
	P->GroundNormal = N;
}

void plSetPosition(Player* Pl, Vec2 Pos)
{
	if(Pl->Shape != NULL)
		vxSetPosition(polyGetVertex(Pl->Shape, 0), Pos);
}

void plSetShape(Player* P, Polygon* Shape)
{
	P->Shape = Shape;
}

void plMoveR(Player* P)
{
	if(P->OnGround) polyApplyForce(P->Shape, vec2(1.f, 0.f));
	else polyApplyForce(P->Shape, vec2(0.5f, 0.f));
}

void plMoveL(Player* P)
{
	if(P->OnGround) polyApplyForce(P->Shape, vec2(-1.f, 0.f));
	else polyApplyForce(P->Shape, vec2(-0.5f, 0.f));
}

void plJump(Player* P)
{
	if(P->OnGround) polyApplyForce(P->Shape, vec2Prod(P->GroundNormal, 20.f));
}

void plGetUp(Player* P)
{

}

void plGrab(Player* P, World* W, float MouseX, float MouseY)
{
	Vertex* tmpVx = wdGetNearest(W, MouseX, MouseY);
	if(vec2Length(vec2Sub(vxGetPosition(tmpVx), vxGetPosition(plGetVxUR(P)))) < 400.f)
	{
		P->Grab = newElastic(plGetVxUR(P), tmpVx, 40.f, 0.1f);
		wdAddElastic(W, P->Grab);
	}
}

void plRelease(Player* P, World* W)
{
	if(P->Grab == NULL) return;
	wdDelElastic(W, P->Grab);
	delElastic(P->Grab);
	P->Grab = NULL;
}

void plUpdate(Player* P)
{
	if (P->Stable==NULL) return;
	Vec2 pos = polyComputeCenter(P->Shape);
	pos.y-=20.f;
	vxSetPosition(P->Stable, pos);
}

void plCreateVertex(Player* P, World* W)
{
	P->Neck = newVertex(), P->HeadLeft = newVertex(), P->HeadRight = newVertex(), P->Base = newVertex(), P->LeftArm1 = newVertex(), P->LeftArm2 = newVertex(), P->RightArm1 = newVertex(), P->RightArm2 = newVertex(), P->LeftLeg1 = newVertex(), P->LeftLeg2 = newVertex(), P->RightLeg1 = newVertex(), P->RightLeg2 = newVertex();
	
	/*vxSetFixe(P->Base, TRUE);
	vxSetFixe(P->Neck, TRUE);
	vxSetFixe(P->HeadLeft, TRUE);
	 */
	
	wdAddVertex(W, P->Neck);
	wdAddVertex(W, P->HeadLeft);
	wdAddVertex(W, P->HeadRight);
	wdAddVertex(W, P->Base);
	wdAddVertex(W, P->LeftArm1);
	wdAddVertex(W, P->LeftArm2);
	wdAddVertex(W, P->RightArm1);
	wdAddVertex(W, P->RightArm2);
	wdAddVertex(W, P->LeftLeg1);
	wdAddVertex(W, P->LeftLeg2);
	wdAddVertex(W, P->RightLeg1);
	wdAddVertex(W, P->RightLeg2);
	
}

void plCreateRigids(Player* P, World* W)
{
	/* On met en place les parties du corps par rapport à la base */
	Vec2 B = vxGetPosition(P->Base);
	vxSetPosition(P->Neck, vec2(B.x, B.x - 90.f));
	vxSetPosition(P->HeadLeft, vec2Add(vxGetPosition(P->Neck), vec2(-30.f, -40.f)));
	vxSetPosition(P->HeadRight, vec2Add(vxGetPosition(P->Neck), vec2(30.f, -40.f)));
	vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 35.f)));
	vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->Neck), vec2(0.f, 70.f)));
	vxSetPosition(P->LeftLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));
	vxSetPosition(P->LeftLeg2, vec2Add(vxGetPosition(P->Base), vec2(0.f, 80.f)));
	vxSetPosition(P->RightLeg1, vec2Add(vxGetPosition(P->Base), vec2(0.f, 40.f)));
	vxSetPosition(P->RightLeg2, vec2Add(vxGetPosition(P->Base), vec2(0.f, 80.f)));
	
	Bool usePolys=1;
	
	if (!usePolys)
	{
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
	
	head = newPolygon(3, P->Neck, P->HeadLeft, P->HeadRight);
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
	}
	
}

void plSetPosition(Player* P, float x, float y)
{
	/* Position relative par rapport à la base des vertex */
	Vec2 N, HL, HR, LA1, LA2, RA1, RA2, LL1, LL2, RL1, RL2;
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
	
	
}
