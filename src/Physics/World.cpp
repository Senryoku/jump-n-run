#include "World.h"

World* newWorld(float Width, float Height)
{
	World* W = (World*) malloc(sizeof(World));
	wdInit(W, Width, Height);
	return W;
}

void wdInit(World* W, float Width, float Height)
{
	lstInit(&W->Vertices);
	lstInit(&W->Rigids);
	lstInit(&W->Polygons);

	W->Width = Width;
	W->Height = Height;
	W->prevdt = 1.f;
	W->dt = 1.f;
}

void wdAddVertex(World* W, Vertex* V)
{
	lstAdd(&W->Vertices, V);
}

void wdAddPolygon(World* W, Polygon* P)
{
	lstAdd(&W->Polygons, P);
}

void wdApplyForce(World* W, Vec2 Force)
{
	Node* it = lstFirst(&W->Vertices);
	while(!nodeEnd(it))
		vxApplyForce((Vertex*) nodeGetData(it), Force),
		it = nodeGetNext(it);
}

void wdResolveVextex(World* W)
{
	Vec2 curPos;
	Vec2 newPos;
	Node* it = lstFirst(&W->Vertices);
	while(!nodeEnd(it))
	{
		vxResolve((Vertex*) nodeGetData(it), W->prevdt, W->dt);
		/* Garde le Vertex dans les limites du monde */
		curPos = vxGetPosition((Vertex*) nodeGetData(it));
		if(curPos.x > W->Width)
			newPos.x = W->Width;
		else if(curPos.x < 0)
			newPos.x = 0;
		else newPos.x = curPos.x;
		if(curPos.y > W->Height)
			newPos.y = W->Height;
		else if(curPos.y < 0)
			newPos.y = 0;
		else newPos.y = curPos.y;
		vxSetPosition((Vertex*) nodeGetData(it), newPos);
		it = nodeGetNext(it);
	}
	W->prevdt = W->dt;
}

void wdResolveRigid(World* W)
{
        unsigned int i;
        Node* it = lstFirst(&W->Rigids);

        /* Parcoure les contraintes orphelines */
        while(!nodeEnd(it))
        {
                rdResolve( (Rigid*) nodeGetData(it));
                it = nodeGetNext(it);
        }

        /* Parcoure les Polygons */
        it = lstFirst(&W->Polygons);
        while(!nodeEnd(it))
        {
                /* Leurs faces */
                for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->Rigids); i++)
                        rdResolve( (Rigid*) daGet(&((Polygon*) nodeGetData(it))->Rigids, i));
                /* Leurs contraintes internes */
                for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->InternalRigids); i++)
                        rdResolve( (Rigid*) daGet(&((Polygon*) nodeGetData(it))->InternalRigids, i));

                it = nodeGetNext(it);
        }
}


void wdHandleCollision(World* W)
{
        CollisionInfo Info;
        Node* it = lstFirst(&W->Polygons);
        Node* it2;
        Vec2 CollisionVector, PosE1, PosE2;
        float PositionOnEdge, CorrectionFactor;
        while(!nodeEnd(it))
        {
                it2 = lstFirst(&W->Polygons);
                while(!nodeEnd(it2))
                {
					if(it != it2)
					{
						Info = polyCollide( (Polygon*) nodeGetData(it), (Polygon*) nodeGetData(it2));
						if(Info.P1 != NULL) /* Il y a collision */
						{
								CollisionVector = vec2Prod(Info.Normal, Info.Depth);

								PosE1 = vxGetPosition(rdGetV1(Info.Edge));
								PosE2 = vxGetPosition(rdGetV2(Info.Edge));

								/* Position du point sur la face,
								 On évite les divisions par 0 ! */
								if(abs(PosE1.x - PosE2.x) > abs(PosE1.y - PosE2.y))
								PositionOnEdge = (vxGetPosition(Info.V).x - CollisionVector.x
								- PosE1.x)/(PosE2.x - PosE1.x);
								else
								PositionOnEdge = (vxGetPosition(Info.V).y - CollisionVector.y
								- PosE1.y)/(PosE2.y - PosE1.y);

								CorrectionFactor = -1.0f/(PositionOnEdge*PositionOnEdge + (1 - PositionOnEdge)*(1 - PositionOnEdge));

								/* Correction des positions */
								vxCorrectPosition(Info.V, vec2Prod(CollisionVector, 0.5f)); /* Du vertex */
								/* De la face */
								vxCorrectPosition(rdGetV1(Info.Edge), vec2Prod(CollisionVector,
								CorrectionFactor*(1-PositionOnEdge)*0.5f));
								vxCorrectPosition(rdGetV2(Info.Edge), vec2Prod(CollisionVector,
								CorrectionFactor*(PositionOnEdge)*0.5f));
						}
					}
					it2 = nodeGetNext(it2);
                }
                it = nodeGetNext(it);
        }
}

void wdFree(World *W)
{
	/*while (!lstEmpty(&W->Circles))
	{
		delCircle((Circle*) nodeGetData(lstFirst(&W->Circles)));
		lstRem(&W->Circles, lstFirst(&W->Circles));
	}*/
	while (!lstEmpty(&W->Polygons))
	{
		delPolygon((Polygon*) nodeGetData(lstFirst(&W->Polygons)));
		lstRem(&W->Polygons, lstFirst(&W->Polygons));
	}
	while (!lstEmpty(&W->Rigids))
	{
		delRigid((Rigid*) nodeGetData(lstFirst(&W->Rigids)));
		lstRem(&W->Rigids, lstFirst(&W->Rigids));
	}
	while (!lstEmpty(&W->Vertices))
	{
		delVertex((Vertex*) nodeGetData(lstFirst(&W->Vertices)));
		lstRem(&W->Vertices, lstFirst(&W->Vertices));
	}
}

void delWorld(World *W)
{
	wdFree(W);
	free(W);
}
