#include "World.h"

void wdResolveRigid(World* W)
{
	unsigned int i;
	Node* it = lstFirst(&W->Rigids);

	/* Parcoure les contraintes orphelines */
	while(it != NULL)
	{
		rdResolve(nodeGetData(it));
		it = nodeGetNext(it);
	}

	/* Parcoure les Polygons */
	it = lstFirst(&W->Polygons);
	while(it != NULL)
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

}
