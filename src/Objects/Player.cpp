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
}

void plFree(Player* P)
{
	if(P->Shape != NULL) delPolygon(P->Shape);
	P->Shape = NULL;
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
	if(P->OnGround) polyApplyForce(P->Shape, vec2(5.f, 0.f));
	else polyApplyForce(P->Shape, vec2(2.f, 0.f));
}

void plMoveL(Player* P)
{
	if(P->OnGround) polyApplyForce(P->Shape, vec2(-5.f, 0.f));
	else polyApplyForce(P->Shape, vec2(-2.f, 0.f));
}

void plJump(Player* P)
{
	if(P->OnGround) polyApplyForce(P->Shape, vec2Prod(P->GroundNormal, 10.f));
}

void plGetUp(Player* P)
{
	if(vxGetPosition(plGetVxUL(P)).y <= vxGetPosition(plGetVxDR(P)).y)
	{
		vxSetPosition(plGetVxDL(P), vxGetPosition(plGetVxUL(P)));
	}
	if(vxGetPosition(plGetVxUR(P)).y <= vxGetPosition(plGetVxDL(P)).y)
	{
		vxSetPosition(plGetVxDR(P), vxGetPosition(plGetVxUR(P)));
	}
}
