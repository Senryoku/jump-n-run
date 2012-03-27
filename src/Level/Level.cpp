#include "Level.h"

Level* newLevel(float Width, float Height)
{
	Level* lvl = (Level*) malloc(sizeof(Level));
	lvlInit(lvl, Width, Height);
	return lvl;
}

void lvlInit(Level* Lvl, float Width, float Height)
{
	Lvl->W = newWorld(Width, Height);
	Lvl->P1 = newPlayer();
}

void lvlFree(Level* Lvl)
{
	if(Lvl->P1 != NULL) delPlayer(Lvl->P1);
	delWorld(Lvl->W);
}

void delLevel(Level* lvl)
{
	lvlFree(lvl);
	free(lvl);
}

void lvlLoadedInit(Level* Lvl)
{
	/* A modifier !!!!! Valeurs de test temporaires */
	Vertex* V1 = newVertex();
	vxSetPosition(V1, vec2(0.f, 0.f));
	Vertex* V2 = newVertex();
	vxSetPosition(V2, vec2(50.f, 0.f));
	Vertex* V3 = newVertex();
	vxSetPosition(V3, vec2(50.f, 100.f));
	Vertex* V4 = newVertex();
	vxSetPosition(V4, vec2(0.f, 100.f));
	Polygon* Shape = polyRectangle(V1, V2, V3, V4);
	Lvl->P1 = newPlayer();
	plSetShape(Lvl->P1, Shape);
	plSetPosition(Lvl->P1, Lvl->Spawn);
	wdAddVxFromPoly(Lvl->W, Shape);
}

void lvlUpdate(Level* Lvl)
{
	unsigned int i;

	wdApplyForce(lvlGetWorld(Lvl), vec2(0.f, 0.6f));
	wdResolveVextex(lvlGetWorld(Lvl));

	if(Lvl->P1 != NULL)
		plUpdate(Lvl->P1);

	wdUpdateGrid(lvlGetWorld(Lvl));
	for(i = 0; i < 4; i++)
	{
		wdResolveRigid(lvlGetWorld(Lvl));
		wdResolveElastic(lvlGetWorld(Lvl));
		wdHandleCollision(lvlGetWorld(Lvl));
	}
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}
