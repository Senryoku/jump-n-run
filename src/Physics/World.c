#include "World.h"

void wdResolveRigid(World* W)
{
	unsigned int i;
	Node* it = lstFirst(&W->Rigids);

	/* Parcoure les contraintes orphelines */
	while(!nodeEnd(it))
	{
		rdResolve(nodeGetData(it));
		it = nodeGetNext(it);
	}

	/* Parcoure les Polygons */
	it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		/* Leurs faces */
 		for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->Rigids); i++)
			rdResolve(daGet(&((Polygon*) nodeGetData(it))->Rigids, i));
		/* Leurs contraintes internes */
		for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->InternalRigids); i++)
			rdResolve(daGet(&((Polygon*) nodeGetData(it))->InternalRigids, i));

		it = nodeGetNext(it);
	}
}


void wdHandleCollision(World* W)
{
	CollisionInfo Info;
	Node* it = lstFirst(&W->Polygons);
	Node* it2;
	while(!nodeEnd(it))
	{
		it2 = lstFirst(&W->Polygons);
		while(!nodeEnd(it2))
		{
			Info = polyCollide();
		}
	}
}
