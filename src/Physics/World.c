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
			if(it == it2) continue;
			Info = polyCollide(nodeGetData(it), nodeGetData(it2));
			if(Info.P1 != NULL) /* Il y a collision */
			{
				Vec2 CollisionVector = vec2Prod(Info.Normal, Info.Depth);

				Vec2 PosE1 = vxGetPosition(rdGetV1(Info.Edge));
				Vec2 PosE2 = vxGetPosition(rdGetV2(Info.Edge));

				float PositionOnEdge; /* Position du point sur la face
				 On évite les divisions par 0 ! */
				if(abs(PosE1.x - PosE2.x) > abs(PosE1.y - PosE2.y))
				PositionOnEdge = (vxGetPosition(Info.V).x - CollisionVector.x
				- PosE1.x)/(PosE2.x - PosE1.x);
				else
				PositionOnEdge = (vxGetPosition(Info.V).y - CollisionVector.y
				- PosE1.y)/(PosE2.y - PosE1.y);

				float CorrectionFactor = -1.0f/(PositionOnEdge*PositionOnEdge + (1 - PositionOnEdge)*(1 - PositionOnEdge));

				/* Correction des positions */
				vxCorrectPosition(Info.V, vec2Prod(CollisionVector, 0.5f)); /* Du vertex */
				/* De la face */
				vxCorrectPosition(rdGetV1(Info.Edge), vec2Prod(CollisionVector,
				CorrectionFactor*(1-PositionOnEdge)*0.5f));
				vxCorrectPosition(rdGetV2(Info.Edge), vec2Prod(CollisionVector,
				CorrectionFactor*(PositionOnEdge)*0.5f));
			}
		}
	}
}
