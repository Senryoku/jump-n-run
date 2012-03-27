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

void plSetPosition(Player* Pl, Vec2 Pos)
{
	if(Pl->Shape != NULL)
		vxSetPosition(polyGetVertex(Pl->Shape, 0), Pos);
}

void plSetShape(Player* P, Polygon* Shape)
{
	P->Shape = Shape;
}

void plUpdate(Player* P)
{
	/* polyCollision, rdResolve */
}

void plMoveR(Player* P)
{
	polyApplyForce(P->Shape, vec2(5.f, 0.f));
}

void plMoveL(Player* P)
{
	polyApplyForce(P->Shape, vec2(-5.f, 0.f));
}

void plJump(Player* P)
{
	polyApplyForce(P->Shape, vec2(0.f, -10.f));
}
