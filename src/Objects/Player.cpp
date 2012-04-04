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
