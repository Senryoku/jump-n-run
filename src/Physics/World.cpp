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
	for(i = 0; i < daGetSize(&P->Vertices); i++)
	{
		wdAddVertex(W, (Vertex*) daGet(&P->Vertices, i));
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) W->dt=0.2f; else W->dt=1.f;

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
        Vec2 CollisionVector, PosE1, PosE2;
        float PositionOnEdge, CorrectionFactor;
        while(!nodeEnd(it))
        {
            List LExtracted = gridGetPolygonList(&W->CollisionGrid, (Polygon*) nodeGetData(it));
			//it2 = lstFirst(&W->Polygons);
			it2 = lstFirst(&LExtracted);
                while(!nodeEnd(it2))
                {
					if(1)//it != it2)
					{
						Info = polyCollide( (Polygon*) nodeGetData(it), (Polygon*) nodeGetData(it2));
						if(Info.P1 != NULL) /* Il y a collision */
						{
							if (vec2Dot(Info.Normal, vec2Sub(polyComputeCenter(Info.P2), polyComputeCenter(Info.P1))) < 0.f)
								Info.Normal=vec2Prod(Info.Normal, -1.f);

							CollisionVector = vec2Prod(Info.Normal, Info.Depth);

							PosE1 = vxGetPosition(rdGetV1(Info.Edge));
							PosE2 = vxGetPosition(rdGetV2(Info.Edge));

							/* Position du point sur la face,
							 On évite les divisions par 0 ! */
							if(fabs(PosE1.x - PosE2.x) > fabs(PosE1.y - PosE2.y))
								PositionOnEdge = (vxGetPosition(Info.V).x - CollisionVector.x
							- PosE1.x)/(PosE2.x - PosE1.x);
							else
								PositionOnEdge = (vxGetPosition(Info.V).y - CollisionVector.y
							- PosE1.y)/(PosE2.y - PosE1.y);

							/* DEBUG !
							if (DebugDraw)
							{
								if(PositionOnEdge > 1.f || PositionOnEdge < 0.f)
								{
									if(fabs(PosE1.x - PosE2.x) > fabs(PosE1.y - PosE2.y))
										printf("#ERROR#\n vxGetPosition(Info.V).x : %f \n CollisionVector.x : %f \n PosE1.x : %f \n PosE2.x : %f \n PositionOnEdge : %f \n", vxGetPosition(Info.V).x, CollisionVector.x, PosE1.x, PosE2.x, PositionOnEdge);
									else
										printf("#ERROR#\n vxGetPosition(Info.V).y : %f \n CollisionVector.y : %f \n PosE1.y : %f \n PosE2.y : %f \n PositionOnEdge : %f \n", vxGetPosition(Info.V).y, CollisionVector.y, PosE1.y, PosE2.y, PositionOnEdge);
							 it2 = nodeGetNext(it2); continue; // Mesure temporaire, si la collision n'est pas valide,
									on l'ignore, bien sur, il faudrait que ça ne puisse pas arriver...
								}

								//On dessine un peu de Debug
								//le vertice en collision
								glColor3f(1.f, 0.f, 0.f);
								glBegin(GL_LINES);
								glVertex2f(PosE1.x, PosE1.y);
								glVertex2f(PosE2.x, PosE2.y);
								glEnd();


								//Le vertex concerné
								glBegin(GL_TRIANGLE_FAN);
								glVertex2f(vxGetPosition(Info.V).x, vxGetPosition(Info.V).y);
								for (int i=0; i<=16; i++)
								{
									glVertex2f(1*5.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(Info.V).x,
											   1*5.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(Info.V).y);
								}
								glEnd();
							}
							*/

							CorrectionFactor = -1.0f/(PositionOnEdge*PositionOnEdge
								+ (1.f - PositionOnEdge)*(1.f - PositionOnEdge));

							/* Correction des positions
							 Du vertex */
							vxCorrectPosition(Info.V, vec2Prod(CollisionVector, 0.5f));
							/* De la face */
							vxCorrectPosition(rdGetV1(Info.Edge), vec2Prod(CollisionVector,
																		   CorrectionFactor*(1.f-PositionOnEdge)*0.5f));
							vxCorrectPosition(rdGetV2(Info.Edge), vec2Prod(CollisionVector,
																		   CorrectionFactor*PositionOnEdge*0.5f));
						}
					}
					it2 = nodeGetNext(it2);
                }

			lstFree(&LExtracted);
			it = nodeGetNext(it);
        }
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
	//printf("Size before deleting: %u\n", lstCount(W->CollisionGri))
	gridRemovePolygons(&W->CollisionGrid);

	unsigned int counta=0, countm=0;
	Node* it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		if (!polyIsFixe((Polygon*)nodeGetData(it)))
			gridAddPolygonByBB(&W->CollisionGrid, (Polygon*)nodeGetData(it)), counta++;
		else countm++;

		it=nodeGetNext(it);
	}
	//printf("Missed : %u, added %u\n", countm, counta);
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
