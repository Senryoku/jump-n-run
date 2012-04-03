#include "LevelEditor.h"

void lvledInit(LevelEditor *Led, float Width, float Height)
{
	Led->Lvl = newLevel(Width, Height);
	/* Pas besoin d'initialiser les listes */
 	Led->Grab = NULL;
 	Led->GrabEl = NULL;
 	Led->Mouse = newVertex();
 	Led->GrabElastic = newElastic(Led->Mouse, NULL, 30.f, 0.05f);
	Led->tmpElastic1 = Led->tmpElastic2 = Led->tmpRigid1 = Led->tmpRigid2 =
		Led->tmpBox1 = Led->tmpBox2 = Led->tmpBox3 = Led->tmpBox4 = NULL;
	Led->vxDraw = NULL;
	Led->elDraw = NULL;
	Led->rdDraw = NULL;
	Led->polyDraw = NULL;
	
	Led->backPath[0]='\0';
	Led->layer1Path[0]='\0';
	Led->layer2Path[0]='\0';
	Led->forePath[0]='\0';
}

void lvledFree(LevelEditor *Led)
{
	delLevel(Led->Lvl);
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

	if(Led->tmpBox1 != NULL)
	{
		(*Led->lineDraw) (vxGetPosition(Led->tmpBox1).x, vxGetPosition(Led->tmpBox1).y,
			vxGetPosition(Led->tmpBox2).x, vxGetPosition(Led->tmpBox2).y,
			0.f, 1.f, 1.f, 1.f);
		(*Led->lineDraw) (vxGetPosition(Led->tmpBox2).x, vxGetPosition(Led->tmpBox2).y,
			vxGetPosition(Led->tmpBox3).x, vxGetPosition(Led->tmpBox3).y,
			0.f, 1.f, 1.f, 1.f);
		(*Led->lineDraw) (vxGetPosition(Led->tmpBox3).x, vxGetPosition(Led->tmpBox3).y,
			vxGetPosition(Led->tmpBox2).x, vxGetPosition(Led->tmpBox2).y,
			0.f, 1.f, 1.f, 1.f);
		(*Led->lineDraw) (vxGetPosition(Led->tmpBox4).x, vxGetPosition(Led->tmpBox4).y,
			vxGetPosition(Led->tmpBox1).x, vxGetPosition(Led->tmpBox1).y,
			0.f, 1.f, 1.f, 1.f);
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
	if(tmpVertex != NULL) vxSetFixe(tmpVertex, !vxIsFixe(tmpVertex));
}

void lvledToogleNearestPolyFixe(LevelEditor *Led)
{
	Polygon* tmpPoly = wdGetNearestPoly(lvlGetWorld(Led->Lvl), vxGetPosition(Led->Mouse).x, vxGetPosition(Led->Mouse).y);
	if(tmpPoly != NULL) polySetFixe(tmpPoly, !polyIsFixe(tmpPoly));
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
			printf("list size is %u\n", lstCount(&Led->tmpLstDyn));
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

void lvledNewBoxInit(LevelEditor *Led)
{
	if(Led->tmpBox1 != NULL)
	{
		delVertex(Led->tmpBox1);
		delVertex(Led->tmpBox2);
		delVertex(Led->tmpBox3);
		delVertex(Led->tmpBox4);
		Led->tmpBox1 = Led->tmpBox2 = Led->tmpBox3 = Led->tmpBox4 = NULL;
	}
	Led->tmpBox1 = newVertex();
	vxSetPosition(Led->tmpBox1, vxGetPosition(Led->Mouse));
	Led->tmpBox2 = newVertex();
	vxSetPosition(Led->tmpBox2, vxGetPosition(Led->Mouse));
	Led->tmpBox3 = newVertex();
	vxSetPosition(Led->tmpBox3, vxGetPosition(Led->Mouse));
	Led->tmpBox4 = newVertex();
	vxSetPosition(Led->tmpBox4, vxGetPosition(Led->Mouse));
}

void lvledNewBoxUpdate(LevelEditor *Led)
{
	if(Led->tmpBox1 == NULL) return;
	Vec2 Pos1 = vxGetPosition(Led->tmpBox1);
	Vec2 Mouse = vxGetPosition(Led->Mouse);
	vxSetPosition(Led->tmpBox3, Mouse);
	vxSetPosition(Led->tmpBox2, vec2(Mouse.x, Pos1.y));
	vxSetPosition(Led->tmpBox4, vec2(Pos1.x, Mouse.y));
}

void lvledNewBoxCreate(LevelEditor *Led)
{
	if(Led->tmpBox1 == NULL) return;
	wdAddVertex(lvlGetWorld(Led->Lvl), Led->tmpBox1); wdAddVertex(lvlGetWorld(Led->Lvl), Led->tmpBox2);
	wdAddVertex(lvlGetWorld(Led->Lvl), Led->tmpBox3); wdAddVertex(lvlGetWorld(Led->Lvl), Led->tmpBox4);
	Polygon* tmpRect = polyRectangle(Led->tmpBox1, Led->tmpBox2, Led->tmpBox3, Led->tmpBox4);
	wdAddPolygon(lvlGetWorld(Led->Lvl), tmpRect);
	Led->tmpBox1 = Led->tmpBox2 = Led->tmpBox3 = Led->tmpBox4 = NULL;
}

void lvledTestLevel(LevelEditor *Led)
{
	lvledSave(Led, "levels/TempTest.lvl~");
	Game G;
	gmInit(&G);
	gmLoadLvl(&G, "levels/TempTest.lvl~");
	gmPlay(&G);
	gmFree(&G);
}

Bool lvledLoad(LevelEditor *Led, const char* File)
{
	//on lit les paths des images fixes
	FILE* f;
	f=fopen(File, "r");
	
	if (f==NULL)
	{
		printf("Erreur de lecture du fichier %s\n", File);
		return FALSE;
	}
	//on ignore les premieres lignes
	char read[300];
	unsigned int i;
	for (i=0; i<3; i++)
		fgets(read, 300, f);
	fgets(Led->backPath, 255, f);
	fgets(Led->layer1Path, 255, f);
	fgets(Led->layer2Path, 255, f);
	fgets(Led->forePath, 255, f);
	*strstr(Led->backPath, "\n") = '\0';
	*strstr(Led->layer1Path, "\n") = '\0';
	*strstr(Led->layer2Path, "\n") = '\0';
	*strstr(Led->forePath, "\n") = '\0';
	
	fclose(f);
	
	return lvlLoad(Led->Lvl, File);
}



Bool lvledSave(LevelEditor *Led, const char* File)
{
	printf("Sauvegarde...\n");
	FILE* f;
	f=fopen(File, "w");

	if (f==NULL)
	{
		printf("Erreur de sauvegarde du fichier %s\n", File);
		return FALSE;
	}

	/* Entete du fichier*/
	char lvl[100]="Niveau Test", description[300]="Ceci est la description du niveau";
	fprintf(f, "%s\n%s\n%f, %f\n", lvl, description, lvlGetWorld(Led->Lvl)->Width, lvlGetWorld(Led->Lvl)->Height);
	fprintf(f, "%s\n%s\n%s\n%s\n", Led->backPath, Led->layer1Path, Led->layer2Path, Led->forePath);

	List* L;
	Node* it;
	DynArr *Vx = newDynArr();

	//On stocke les vertex qui representent le centre des polygones de plus de 4 cotes pour eviter de les mettre dans le fichier
	L = &lvlGetWorld(Led->Lvl)->Polygons;
	it = lstFirst(L);

	List* LCenter = newList();

	//on écrit les polygones dans la dynarr pour leur associer une ID
	while (!nodeEnd(it))
	{
		Polygon* p = (Polygon*)nodeGetData(it);
		//c'est &lu pour les machines de x64 et %u pour les x86
		if (daGetSize(&p->Vertices)>4 && polyGetCenter(p)!=NULL)
			lstAdd(LCenter, polyGetCenter(p)), printf("vertex ignored: %lu\n", (size_t)polyGetCenter(p));
		

		it = nodeGetNext(it);
	}

	//on écrit les vertex
	L  = &lvlGetWorld(Led->Lvl)->Vertices;
	it = lstFirst(L);

	while (!nodeEnd(it))
	{
		Vertex* V =  (Vertex*)nodeGetData(it);
		if (!lstHaveElem(LCenter, V))
		{
			fprintf(f, "%u #Vertex\n", o_vertex);
			fprintf(f, "%f, %f ; %f ; %i\n", vxGetPosition(V).x, vxGetPosition(V).y, vxGetMass(V), (int) vxIsFixe(V));
			daAdd(Vx, V);
		}


		it = nodeGetNext(it);
	}

	L = &lvlGetWorld(Led->Lvl)->Polygons;
	it = lstFirst(L);

	//on écrit les polygones
	while (!nodeEnd(it))
	{
		Polygon* p = (Polygon*)nodeGetData(it);
		unsigned int nVertex = daGetSize(&p->Vertices), i;
		//on écrit un identifiant pour dire qu'on lit un polygone
		fprintf(f, "%u %u %i #Polygon\n", o_poly, nVertex, (int) polyIsFixe(p));
		//On écrit les vertex du polygone
		for (i=0; i<nVertex; i++)
		{
			Vertex* V =  (Vertex*)daGet(&p->Vertices, i);
			fprintf(f, "%u\n", daGetID(Vx, V));
		}
		//fprintf(f, "%u ;End\n", o_end);

		it = nodeGetNext(it);
	}

	L = &lvlGetWorld(Led->Lvl)->Rigids;
	it = lstFirst(L);

	//on écrit les rigides
	while (!nodeEnd(it))
	{
		Rigid* r = (Rigid*)nodeGetData(it);
		fprintf(f, "%u #Rigid\n", o_rigid);
		fprintf(f, "%u %u %f\n", daGetID(Vx, rdGetV1(r)), daGetID(Vx, rdGetV2(r)), rdGetLength(r));
		//fprintf(f, "%u ;End\n", o_end);

		it = nodeGetNext(it);
	}

	L = &lvlGetWorld(Led->Lvl)->Elastics;
	it = lstFirst(L);

	//on écrit les elastiques
	while (!nodeEnd(it))
	{
		Elastic* e = (Elastic*)nodeGetData(it);
		fprintf(f, "%u #Elastic\n", o_elastic);
		fprintf(f, "%u %u %f %f\n", daGetID(Vx, elGetV1(e)), daGetID(Vx, elGetV2(e)), elGetLength(e), elGetSpring(e));
		//fprintf(f, "%u ;End\n", o_end);

		it = nodeGetNext(it);
	}



	fclose(f);

	delList(LCenter);
	delDynArr(Vx);

	return TRUE;
}



