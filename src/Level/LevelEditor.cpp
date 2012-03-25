#include "LevelEditor.h"

void lvledInit(LevelEditor *Led, float Width, float Height)
{
	Led->Lvl = newLevel(Width, Height);
	/* Pas besoin d'initialiser les listes */
 	Led->Grab = NULL;
 	Led->GrabEl = NULL;
 	Led->Mouse = newVertex();
 	Led->GrabElastic = newElastic(Led->Mouse, NULL, 30.f, 0.2f);
	Led->tmpElastic1 = Led->tmpElastic2 = Led->tmpRigid1 = Led->tmpRigid2 = NULL;
	Led->vxDraw = NULL;
	Led->elDraw = NULL;
	Led->rdDraw = NULL;
	Led->polyDraw = NULL;
	Led->Testing = 0;
}

void lvledFree(LevelEditor *Led)
{
	lvlFree(Led->Lvl);
	delVertex(Led->Mouse);
	delElastic(Led->GrabElastic);
}

void lvledSetLineDraw(LevelEditor* Led, void (*lineDraw) (float X1, float Y1, float X2, float Y2, float R, float G, float B, float A))
{
	Led->lineDraw = lineDraw;
}

void lvledSetVxDraw(LevelEditor* Led, void (*vxDraw)(Vertex* V, float R, float G, float B, float A))
{
	Led->vxDraw = vxDraw;
}

void lvledSetElDraw(LevelEditor* Led, void (*elDraw)(Elastic* E))
{
	Led->elDraw = elDraw;
}

void lvledSetRdDraw(LevelEditor* Led, void (*rdDraw)(Rigid* R))
{
	Led->rdDraw = rdDraw;
}

void lvledSetPolyDraw(LevelEditor* Led, void (*polyDraw)(Polygon* P))
{
	Led->polyDraw = polyDraw;
}

void lvledDraw(const LevelEditor* Led, char flag)
{
	wdDraw(lvlGetWorld(Led->Lvl), Led->vxDraw, Led->elDraw, Led->rdDraw, Led->polyDraw);

	if((flag & LVLED_RULE) && (Led->lineDraw != NULL))
	{
		(*Led->lineDraw) (0.f , vxGetPosition(Led->Mouse).y,
			lvlGetWorld(Led->Lvl)->Width , vxGetPosition(Led->Mouse).y,
			0.f, 0.f, 1.f, 0.5f);

		for (float i = 0.f; i < lvlGetWorld(Led->Lvl)->Width; i+=10.f)
		{
			if (static_cast<int>(i)%100 == 0)
				(*Led->lineDraw) (i , vxGetPosition(Led->Mouse).y,
					i, vxGetPosition(Led->Mouse).y - 5.f,
					0.f, 0.f, 1.f, 0.5f);
			else if (static_cast<int>(i)%50 == 0)
				(*Led->lineDraw) (i , vxGetPosition(Led->Mouse).y,
					i, vxGetPosition(Led->Mouse).y - 3.5f,
					0.f, 0.f, 1.f, 0.5f);
			else
				(*Led->lineDraw) (i , vxGetPosition(Led->Mouse).y,
					i, vxGetPosition(Led->Mouse).y - 2.5f,
					0.f, 0.f, 1.f, 0.5f);
		}

		(*Led->lineDraw) (vxGetPosition(Led->Mouse).x, 0.f,
			vxGetPosition(Led->Mouse).x, lvlGetWorld(Led->Lvl)->Height,
			0.f, 0.f, 1.f, 0.5f);

		for (float i = 0.f; i < lvlGetWorld(Led->Lvl)->Height; i+=10.f)
		{
			if (static_cast<int>(i)%100 == 0)
				(*Led->lineDraw) (vxGetPosition(Led->Mouse).x, i,
					vxGetPosition(Led->Mouse).x + 5.f, i,
					0.f, 0.f, 1.f, 0.5f);
			else if (static_cast<int>(i)%50 == 0)
				(*Led->lineDraw) (vxGetPosition(Led->Mouse).x, i,
					vxGetPosition(Led->Mouse).x + 3.5f, i,
					0.f, 0.f, 1.f, 0.5f);
			else
				(*Led->lineDraw) (vxGetPosition(Led->Mouse).x, i,
					vxGetPosition(Led->Mouse).x + 2.5f, i,
					0.f, 0.f, 1.f, 0.5f);
		}
	}

	if(flag & LVLED_LIMITS && (Led->lineDraw != NULL))
	{
		(*Led->lineDraw) (0.f, 1.f,
			lvlGetWorld(Led->Lvl)->Width - 1.f, 0.f,
			0.5f, 0.5f, 0.5f, 0.5f);
		(*Led->lineDraw) (lvlGetWorld(Led->Lvl)->Width - 1.f, 0.f,
			lvlGetWorld(Led->Lvl)->Width - 1.f, lvlGetWorld(Led->Lvl)->Height,
			0.5f, 0.5f, 0.5f, 0.5f);
		(*Led->lineDraw) (lvlGetWorld(Led->Lvl)->Width - 1.f, lvlGetWorld(Led->Lvl)->Height,
			0.f, lvlGetWorld(Led->Lvl)->Height,
			0.5f, 0.5f, 0.5f, 0.5f);
		(*Led->lineDraw) (0.f, lvlGetWorld(Led->Lvl)->Height,
			0.f, 1.f,
			0.5f, 0.5f, 0.5f, 0.5f);
	}
}

void lvledGrabUpdate(LevelEditor *Led)
{
	if(Led->Grab != NULL) vxSetPosition(Led->Grab, vxGetPosition(Led->Mouse));
}

void lvledGrab(LevelEditor *Led)
{
	Led->Grab = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
}

void lvledRelease(LevelEditor *Led)
{
	Led->Grab = NULL;
}

void lvledGrabEl(LevelEditor *Led)
{
	Led->GrabEl = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(Led->GrabEl != NULL)
	{
		elSetV2(Led->GrabElastic, Led->GrabEl);
		wdAddElastic(lvlGetWorld(Led->Lvl), Led->GrabElastic);
	}
}

void lvledReleaseEl(LevelEditor *Led)
{
	Led->GrabEl = NULL;
	wdDelElastic(lvlGetWorld(Led->Lvl), Led->GrabElastic);
}

void lvledToogleNearestFixe(LevelEditor *Led)
{
	Vertex* tmpVertex = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	vxSetFixe(tmpVertex, !vxIsFixe(tmpVertex));
}

void lvledToogleNearestPolyFixe(LevelEditor *Led)
{
	Polygon* tmpPoly = wdGetNearestPoly(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	polySetFixe(tmpPoly, !polyIsFixe(tmpPoly));
}

void lvledSetMousePosition(LevelEditor *Led, float x, float y)
{
	vxSetPosition(Led->Mouse, vec2(x, y));
}

void lvledDelPoly(LevelEditor *Led)
{
	Polygon* tmpPoly;
	tmpPoly = wdGetNearestPoly(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(tmpPoly != NULL)
	{
		wdDelPolygon(lvlGetWorld(Led->Lvl), tmpPoly);
		delPolygon(tmpPoly);
	}
}

void lvledDelElastic(LevelEditor *Led)
{
	Elastic* tmpElastic = wdGetNearestElastic(lvlGetWorld(Led->Lvl),
			vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(tmpElastic != NULL)
	{
		wdDelElastic(lvlGetWorld(Led->Lvl), tmpElastic);
		delElastic(tmpElastic);
	}
}

void lvledDelRigid(LevelEditor *Led)
{
	unsigned int i;
	Rigid* tmpRigid = wdGetNearestRigid(lvlGetWorld(Led->Lvl),
			vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(tmpRigid != NULL)
	{
		Node* it = lstFirst(&lvlGetWorld(Led->Lvl)->Polygons);
		while(!nodeEnd(it))
		{
			for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->Rigids); i++)
			{
				if(daGet(&(((Polygon*) nodeGetData(it))->Rigids), i) == tmpRigid)
					return;
			}
			for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->InternalRigids); i++)
			{
				if(daGet(&(((Polygon*) nodeGetData(it))->InternalRigids), i) == tmpRigid)
					return;
			}
			it = nodeGetNext(it);
		}
		wdDelRigid(lvlGetWorld(Led->Lvl), tmpRigid);
		delRigid(tmpRigid);
	}
}

void lvledDelVertex(LevelEditor *Led)
{
	Bool IsLonely = 1;
	unsigned int i;
	Vertex* tmpVertex = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x,
									vxGetPosition(Led->Mouse).y);
	if(tmpVertex != NULL)
	{
		/* On s'assure que le Vertex ne fait partie d'aucune structure plus grande */
		Node* it = lstFirst(&lvlGetWorld(Led->Lvl)->Rigids);
		while(!nodeEnd(it))
		{
			if(rdGetV1((Rigid*) nodeGetData(it)) == tmpVertex ||
				rdGetV2((Rigid*) nodeGetData(it)) == tmpVertex)
				IsLonely = 0;
			it = nodeGetNext(it);
		}
		it = lstFirst(&lvlGetWorld(Led->Lvl)->Elastics);
		while(!nodeEnd(it))
		{
			if(elGetV1((Elastic*) nodeGetData(it)) == tmpVertex ||
				elGetV2((Elastic*) nodeGetData(it)) == tmpVertex)
				IsLonely = 0;
			it = nodeGetNext(it);
		}
		it = lstFirst(&lvlGetWorld(Led->Lvl)->Polygons);
		while(!nodeEnd(it))
		{
			for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->Vertices); i++)
			{
				if(daGet(&((Polygon*) nodeGetData(it))->Vertices, i) == tmpVertex)
					IsLonely = 0;
			}
			it = nodeGetNext(it);
		}

		if(IsLonely)
			wdDelVertex(lvlGetWorld(Led->Lvl), tmpVertex),
			delVertex(tmpVertex);
	}
}

void lvledNewPolyInit(LevelEditor *Led)
{
	lstInit(&Led->tmpLstDyn);
}

void lvledNewPolyAddV(LevelEditor *Led)
{
	Vertex* tmpVertex = newVertex();
	vxSetPosition(tmpVertex, vxGetPosition(Led->Mouse));
	vxSetFixe(tmpVertex, 1);
	lstAdd(&Led->tmpLstDyn, tmpVertex);
}

void lvledNewPolyCreate(LevelEditor *Led)
{
	Polygon* tmpPoly;
	if(lstCount(&Led->tmpLstDyn) > 0)
	{
		wdAddVxFromList(lvlGetWorld(Led->Lvl), Led->tmpLstDyn);

		if(lstCount(&Led->tmpLstDyn) == 4)
		{
			tmpPoly = polyRectangleL(Led->tmpLstDyn);
		}
		else if (lstCount(&Led->tmpLstDyn) <= 3)
		{
			tmpPoly = newPolygonL(Led->tmpLstDyn);
		}
		else
		{
			tmpPoly = polyNGone(Led->tmpLstDyn);
		}
		polySetFixe(tmpPoly, 0);
		wdAddPolygon(lvlGetWorld(Led->Lvl), tmpPoly);
	}
	lstFree(&Led->tmpLstDyn);
}

void lvledNewPolyFixeInit(LevelEditor *Led)
{
	lstInit(&Led->tmpLstFixe);
}

void lvledNewPolyFixeAddV(LevelEditor *Led)
{
	Vertex* tmpVertex = newVertex();
	vxSetPosition(tmpVertex, vxGetPosition(Led->Mouse));
	vxSetFixe(tmpVertex, 1);
	lstAdd(&Led->tmpLstFixe, tmpVertex);
}

void lvledNewPolyFixeCreate(LevelEditor *Led)
{
	Polygon* tmpPoly;
	if(lstCount(&Led->tmpLstFixe) > 0)
	{
		wdAddVxFromList(lvlGetWorld(Led->Lvl), Led->tmpLstFixe);
		tmpPoly = newPolygonL(Led->tmpLstFixe);
		polySetFixe(tmpPoly, 1);
		wdAddPolygon(lvlGetWorld(Led->Lvl), tmpPoly);
		//TEMPORAIRE!!!
		gridAddPolygonByBB(&lvlGetWorld(Led->Lvl)->CollisionGrid, tmpPoly);
	}
	lstFree(&Led->tmpLstFixe);
}

void lvledNewElasticAddV(LevelEditor *Led)
{
	if(Led->tmpElastic1 == NULL) {
		Led->tmpElastic1 = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	} else if(Led->tmpElastic2 == NULL) {
		Led->tmpElastic2 = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	}
}

void lvledNewElasticCreate(LevelEditor *Led)
{
	if(Led->tmpElastic1 != NULL && Led->tmpElastic2 != NULL
		&& Led->tmpElastic1 != Led->tmpElastic2)
		wdAddElastic(lvlGetWorld(Led->Lvl), newElastic(Led->tmpElastic1, Led->tmpElastic2,
								-1.f, 0.1f));
	Led->tmpElastic1 = Led->tmpElastic2 = NULL;
}

void lvledNewRigidAddV(LevelEditor *Led)
{
	if(Led->tmpRigid1 == NULL) {
		Led->tmpRigid1 = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	} else if(Led->tmpElastic2 == NULL) {
		Led->tmpRigid2 = wdGetNearest(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	}
}

void lvledNewRigidCreate(LevelEditor *Led)
{
	if(Led->tmpRigid1 != NULL && Led->tmpRigid2 != NULL
		&& Led->tmpRigid1 != Led->tmpRigid2)
		wdAddRigid(lvlGetWorld(Led->Lvl), newRigid(Led->tmpRigid1, Led->tmpRigid2, -1.f));
	Led->tmpRigid1 = Led->tmpRigid2 = NULL;
}

void lvledTestLevel(LevelEditor *Led)
{
	printf("Boucle de Test !");
}
