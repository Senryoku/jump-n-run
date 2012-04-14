#include "World.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

World* newWorld(float Width, float Height)
{
	World* W = (World*) malloc(sizeof(World));
	wdInit(W, Width, Height);
	return W;
}

void wdInit(World* W, float Width, float Height)
{
	lstInit(&W->Vertices);
	lstInit(&W->Elastics);
	lstInit(&W->Rigids);
	lstInit(&W->Polygons);

	W->Width = Width;
	W->Height = Height;
	W->prevdt = 0.5f;
	W->dt = 0.5f;
	float CellSize=128.f;
	gridInit(&W->CollisionGrid, Width/CellSize+1, Height/CellSize+1);
	gridSetCellSize(&W->CollisionGrid, CellSize);
}

void wdAddVertex(World* W, Vertex* V)
{
	lstAdd(&W->Vertices, V);
}

void wdAddPolygon(World* W, Polygon* P)
{
	lstAdd(&W->Polygons, P);
	/* Ajout du centre si nécessaire */
	if(polyGetCenter(P) != NULL)
		wdAddVertex(W, polyGetCenter(P));
}

void wdAddRigid(World* W, Rigid* R)
{
	lstAdd(&W->Rigids, R);
}

void wdAddElastic(World* W, Elastic* E)
{
	lstAdd(&W->Elastics, E);
}

void wdAddVxFromPoly(World* W, Polygon* P)
{
	unsigned int i;
	for(i = 0; i < polyGetVxCount(P); i++)
	{
		wdAddVertex(W, polyGetVertex(P, i));
	}
}

void wdAddVxFromList(World* W, List L)
{
	Node* it = lstFirst(&L);
	while(!nodeEnd(it))
	{
		wdAddVertex(W, (Vertex*) nodeGetData(it));
		it = nodeGetNext(it);
	}
}

void wdDelVertex(World* W, Vertex* V)
{
	lstDel(&W->Vertices, V);
}

void wdDelRigid(World* W, Rigid* R)
{
	lstDel(&W->Rigids, R);
}

void wdDelElastic(World* W, Elastic* E)
{
	lstDel(&W->Elastics, E);
}

void wdDelPolygon(World* W, Polygon* P)
{
	/* On retire le centre du polygon de la liste de vertices */
	if(polyGetCenter(P) != NULL) lstDel(&W->Vertices, polyGetCenter(P));
	lstDel(&W->Polygons, P);
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
		//printf("new pos : %f, %f ; curpos: %f, %f\n", newPos.x, newPos.y, curPos.x, curPos.y);
		if (curPos.y != newPos.y || curPos.x != newPos.x)
		vxSetPosition((Vertex*) nodeGetData(it), newPos);

		if (curPos.y != newPos.y || curPos.x != newPos.x)
				vxSetPosition((Vertex*) nodeGetData(it), newPos);

		it = nodeGetNext(it);
	}
	W->prevdt = W->dt;
}

void wdResolveRigid(World* W)
{
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
			polyResolve((Polygon*) nodeGetData(it));
			it = nodeGetNext(it);
        }
}

void wdResolveElastic(World* W)
{
	Node* it = lstFirst(&W->Elastics);

	while(!nodeEnd(it))
	{
			elResolve((Elastic*) nodeGetData(it));
			it = nodeGetNext(it);
	}
}

void wdHandleCollision(World* W)
{

	CollisionInfo Info;
	Node* it = lstFirst(&W->Polygons);
	Node* it2;
	//On met à FALSE tous les collide
	while(!nodeEnd(it))
	{
		polySetCollided((Polygon*)nodeGetData(it), FALSE);
		it=nodeGetNext(it);
	}

	it = lstFirst(&W->Polygons);

        while(!nodeEnd(it))
        {
			if(0)// (polyIsFixe((Polygon*) nodeGetData(it))) //On saute les collisions si le polygone est fixe
			{
				it=nodeGetNext(it);
				continue;
			}
            List LExtracted = gridGetPolygonList(&W->CollisionGrid, (Polygon*) nodeGetData(it));
			//printf("For polygon %i , list size is %u\n", nodeGetData(it), lstCount(&LExtracted));
			//it2 = lstFirst(&W->Polygons);
			it2 = lstFirst(&LExtracted);
                while(!nodeEnd(it2))
                {
					if(!polyHasCollided((Polygon*)nodeGetData(it2)))//it != it2)
					{
						Info = polyCollide( (Polygon*) nodeGetData(it), (Polygon*) nodeGetData(it2));
						if(Info.P1 != NULL) /* Il y a collision */
							polyHandleCollision(Info);
					}
					it2 = nodeGetNext(it2);
                }

			lstFree(&LExtracted);
			polySetCollided((Polygon*)nodeGetData(it), TRUE);
			it = nodeGetNext(it);
        }
}


float wdGetWidth(World* W)
{
	return W->Width;
}

float wdGetHeight(World* W)
{
	return W->Height;
}

unsigned int wdGetVxCount(World* W)
{
	return lstCount(&W->Vertices);
}

unsigned int wdGetElCount(World* W)
{
	return lstCount(&W->Elastics);
}

unsigned int wdGetRdCount(World* W)
{
	return lstCount(&W->Rigids);
}

unsigned int wdGetPolyCount(World* W)
{
	return lstCount(&W->Polygons);
}

Node* wdGetVertexIt(World* W)
{
	return lstFirst(&W->Vertices);
}

Node* wdGetElasticIt(World* W)
{
	return lstFirst(&W->Elastics);
}

Node* wdGetRigidIt(World* W)
{
	return lstFirst(&W->Rigids);
}

Node* wdGetPolyIt(World* W)
{
	return lstFirst(&W->Polygons);
}

Vertex* wdGetNearest(World* W, float X, float Y)
{
	if (lstEmpty(&W->Vertices)) return NULL;
	float Dist = INFINITY, tmpDist;
	Vertex* Nearest = NULL;

	Node* it = lstFirst(&W->Vertices);
	while(!nodeEnd(it))
	{
		Vec2 VPos = vxGetPosition((Vertex*) nodeGetData(it));
		Vec2 V = vec2(X - VPos.x, Y - VPos.y);
		tmpDist = vec2SqLength(V);
		if (tmpDist < Dist)
		{
			Dist = tmpDist;
			Nearest = (Vertex*) nodeGetData(it);
		}

		it = nodeGetNext(it);
	}

	return Nearest;
}

Elastic* wdGetNearestElastic(World* W, float X, float Y)
{
	float Dist = INFINITY, tmpDist;
	Elastic *Nearest = NULL, *tmpElastic;
	Node* it = lstFirst(&W->Elastics);
	while(!nodeEnd(it))
	{
		tmpElastic = (Elastic*) nodeGetData(it);
		Vec2 V1Pos = vxGetPosition(elGetV1(tmpElastic));
		Vec2 V1 = vec2(X - V1Pos.x, Y - V1Pos.y);
		Vec2 V2Pos = vxGetPosition(elGetV2(tmpElastic));
		Vec2 V2 = vec2(X - V2Pos.x, Y - V2Pos.y);
		tmpDist = MIN(vec2SqLength(V1), vec2SqLength(V2));
		if (tmpDist < Dist)
		{
			Dist = tmpDist;
			Nearest = tmpElastic;
		}
		it = nodeGetNext(it);
	}

	return Nearest;
}

Rigid* wdGetNearestRigid(World* W, float X, float Y)
{
	float Dist = INFINITY, tmpDist;
	Rigid *Nearest = NULL, *tmpRigid;
	Node* it = lstFirst(&W->Rigids);
	while(!nodeEnd(it))
	{
		tmpRigid = (Rigid*) nodeGetData(it);
		Vec2 V1Pos = vxGetPosition(rdGetV1(tmpRigid));
		Vec2 V1 = vec2(X - V1Pos.x, Y - V1Pos.y);
		Vec2 V2Pos = vxGetPosition(rdGetV2(tmpRigid));
		Vec2 V2 = vec2(X - V2Pos.x, Y - V2Pos.y);
		tmpDist = MIN(vec2SqLength(V1), vec2SqLength(V2));
		if (tmpDist < Dist)
		{
			Dist = tmpDist;
			Nearest = tmpRigid;
		}
		it = nodeGetNext(it);
	}

	return Nearest;
}

Polygon* wdGetNearestPoly(World* W, float X, float Y)
{
	unsigned int i;
	float Dist = INFINITY, tmpDist;
	Polygon *Nearest = NULL, *tmpPoly;
	Node* it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		tmpPoly = (Polygon*) nodeGetData(it);
		for(i = 0; i < daGetSize(&tmpPoly->Vertices); i++)
		{
			Vec2 VPos = vxGetPosition((Vertex*) daGet(&tmpPoly->Vertices, i));
			Vec2 V = vec2(X - VPos.x, Y - VPos.y);
			tmpDist = vec2SqLength(V);
			if (tmpDist < Dist)
			{
				Dist = tmpDist;
				Nearest = tmpPoly;
			}
		}
		it = nodeGetNext(it);
	}

	return Nearest;
}

void wdFree(World *W)
{
	while (!lstEmpty(&W->Polygons))
	{
		/* On retire le centre du polygon de la liste de vertices */
		if(polyGetCenter((Polygon*) nodeGetData(lstFirst(&W->Polygons))) != NULL)
		{
			lstDel(&W->Vertices, polyGetCenter((Polygon*) nodeGetData(lstFirst(&W->Polygons))));

		}
		delPolygon((Polygon*) nodeGetData(lstFirst(&W->Polygons)));
		lstRem(&W->Polygons, lstFirst(&W->Polygons));
	}
	while (!lstEmpty(&W->Elastics))
	{
		delElastic((Elastic*) nodeGetData(lstFirst(&W->Elastics)));
		lstRem(&W->Elastics, lstFirst(&W->Elastics));
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
	gridFree(&W->CollisionGrid);
}

void delWorld(World *W)
{
	wdFree(W);
	free(W);
}

void wdUpdateGrid(World *W)
{
	Node* it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		if (!polyIsFixe((Polygon*)nodeGetData(it)))
			gridUpdatePolygonPositionByBB(&W->CollisionGrid, (Polygon*)nodeGetData(it));

		it=nodeGetNext(it);
	}
}


void wdDraw(World* W, void (*vxDraw)(Vertex* V, float R, float G, float B, float A),
	void (*elDraw)(Elastic* E),
	void (*rdDraw)(Rigid* R),
	void (*polyDraw) (Polygon* P))
{
	Node* it;

	if(vxDraw != NULL)
	{
		it = lstFirst(&W->Vertices);
		while(!nodeEnd(it))
		{
			if(vxIsFixe((Vertex*) nodeGetData(it)))
				(*vxDraw)((Vertex*) nodeGetData(it), 1.f, 0.f, 0.f, 0.3f);
			else
				(*vxDraw)((Vertex*) nodeGetData(it), 0.f, 1.f, 0.f, 0.3f);
			it = nodeGetNext(it);
		}
	}

	if(elDraw != NULL)
	{
		it = lstFirst(&W->Elastics);
		while(!nodeEnd(it))
		{
			(*elDraw) ((Elastic*) nodeGetData(it));
			it = nodeGetNext(it);
		}
	}

	if(rdDraw != NULL)
	{
		it = lstFirst(&W->Rigids);
		while(!nodeEnd(it))
		{
			(*rdDraw) ((Rigid*) nodeGetData(it));
			it = nodeGetNext(it);
		}
	}

	if(polyDraw != NULL)
	{
		it = lstFirst(&W->Polygons);
		while(!nodeEnd(it))
		{
			(*polyDraw) ((Polygon*) nodeGetData(it));
			it = nodeGetNext(it);
		}
	}
}
