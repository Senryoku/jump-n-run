#include "LevelEditor.h"

void lvledInit(LevelEditor *Led, float Width, float Height)
{
	lvlInit(Led->Lvl, Width, Height);
	/* Pas besoin d'initialiser les listes */
 	Led->Grab = NULL;
 	Led->GrabEl = NULL;
 	Led->Mouse = newVertex();
 	Led->GrabElastic = newElastic(Led->Mouse, NULL, 30.f, 0.2f);
}

void lvledSetMousePosition(LevelEditor *Led, float x, float y)
{
	vxSetPosition(Led->Mouse, vec2(x, y));
}

void lvledDelPoly(LevelEditor *Led, Polygon*)
{
	Polygon* tmpPoly;
	tmpPoly = wdGetNearestPoly(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(tmpPoly != NULL)
	{
		wdDelPolygon(lvlGetWorld(Led->Lvl), tmpPoly);
		delPolygon(tmpPoly);
	}
}

void lvledDelVertex(LevelEditor *Led, Vertex*)
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
			if(elasticGetV1((Elastic*) nodeGetData(it)) == tmpVertex ||
				elasticGetV2((Elastic*) nodeGetData(it)) == tmpVertex)
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

void lvledNewPolyAddV(LevelEditor *Led, Vertex*)
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

void lvledNewPolyFixeAddV(LevelEditor *Led, Vertex* V)
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
	}
	lstFree(&Led->tmpLstFixe);
}
