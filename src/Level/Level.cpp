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
	Lvl->P1 = NULL;
	Lvl->Spawn = Lvl->Goal = vec2(0.f, 0.f);
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
	CollisionInfo Info;
	Node* it;

	/* Mise à jour spécifique de Player */
	plSetOnGround(Lvl->P1, 0);

	/* Mise à jour du World */

	wdApplyForce(lvlGetWorld(Lvl), vec2(0.f, 0.6f));
	wdResolveVextex(lvlGetWorld(Lvl));

	wdUpdateGrid(lvlGetWorld(Lvl));
	for(i = 0; i < 4; i++) /* Augmenter Imax pour augmenter la précision */
	{
		wdResolveRigid(lvlGetWorld(Lvl));
		wdResolveElastic(lvlGetWorld(Lvl));
		wdHandleCollision(lvlGetWorld(Lvl));

		/* Mise à jour spécifique de Player */
		polyResolve(plGetShape(Lvl->P1));
		it = wdGetPolyIt(Lvl->W);
		while(!nodeEnd(it))
		{
			Info = polyCollide(plGetShape(Lvl->P1), (Polygon*) nodeGetData(it));
			if(Info.P1 != NULL)
			{
				/* Test des propriétés de la collision */
				if(Info.Edge == plGetRdD(Lvl->P1) || Info.V == plGetVxDL(Lvl->P1) || Info.V == plGetVxDR(Lvl->P1))
					plSetOnGround(Lvl->P1, 1),
					plSetGroundNormal(Lvl->P1, Info.Normal);
				polyHandleCollision(Info);
			}
			it = nodeGetNext(it);
		}
	}
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}
