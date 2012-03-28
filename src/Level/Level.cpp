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


Bool lvlLoad(Level* Lvl, const char* File)
{
	printf("Chargement...\n");

	FILE* f;
	f=fopen(File, "r");

	if (f==NULL)
	{
		printf("Erreur de chargement du fichier %s\n", File);
		return FALSE;
	}

	char lvl[100], description[300], read[300];
	if (fgets(lvl, 100, f)==NULL)
	{
		printf("Le fichier ne peut pas être lu\n");
		return FALSE;
	}
	if (fgets(description, 300, f)==NULL)
	{
		printf("Le fichier ne peut pas être lu\n");
		return FALSE;
	}

	float width, height;

	if (fgets(read, 300, f)==NULL)
	{
		printf("Le fichier ne peut pas être lu\n");
		return FALSE;
	}
	else
		sscanf(read, "%f, %f", &width, &height);

	//On libere le monde et on le realloue
	wdFree(Lvl->W);
	wdInit(Lvl->W, width, height);

	unsigned int item, nVertex, i;
	Bool polyFixed;

	//liste des vertex
	List* vxL = newList();

	while (fgets(read, 300, f)!=NULL)
	{
		printf("Read: %s", read);
		item=o_end;
		polyFixed=FALSE;
		sscanf(read, "%u %u %i #%.s\n", &item, &nVertex, &polyFixed);

		switch (item)
		{
			case o_poly:
				printf("Polygon with %u vertex read\n", nVertex);
				switch (nVertex)
				{
					case 0:
					case 1:
						break;
					case 2:
					case 3:
					{
					Vertex **V = (Vertex**)malloc(sizeof(Vertex)*nVertex);
					unsigned int *ID = (unsigned int*)malloc(sizeof(unsigned int)*nVertex);
					for (i=0; i<nVertex; i++)
					{
						fscanf(f, "%u\n", &ID[i]);
						V[i]=GetVertexFromID(vxL, ID[i]);
					}
					Polygon* p;
					if (nVertex==2)
						p = newPolygon(2, V[0], V[1]);
					else
						p = newPolygon(3, V[0], V[1], V[2]);
					if (polyFixed)
						polySetFixe(p, TRUE);

					wdAddPolygon(lvlGetWorld(Lvl), p);
					free(V);
					free(ID);

					break;
				}
					case 4:
					{
					Vertex *V1, *V2, *V3, *V4;
					unsigned int ID1, ID2, ID3, ID4;
					fscanf(f, "%u\n", &ID1);
					fscanf(f, "%u\n", &ID2);
					fscanf(f, "%u\n", &ID3);
					fscanf(f, "%u\n", &ID4);
					V1 = GetVertexFromID(vxL, ID1);
					V2 = GetVertexFromID(vxL, ID2);
					V3 = GetVertexFromID(vxL, ID3);
					V4 = GetVertexFromID(vxL, ID4);
					Polygon* p;
					if (!polyFixed)
						p = polyRectangle(V1, V2, V3, V4);
					else
						p = newPolygon(4, V1, V2, V3, V4), polySetFixe(p, TRUE);
					wdAddPolygon(lvlGetWorld(Lvl), p);

					break;
				}

					default:
					{
					List* LPoly = newList();
					for (i=0; i<nVertex; i++)
					{
						unsigned int ID;
						fscanf(f, "%u\n", &ID);
						lstAdd(LPoly, GetVertexFromID(vxL, ID));
					}
					Polygon* p;
					if (!polyFixed)
						p = polyNGone(*LPoly);
					else
						p = newPolygonL(*LPoly), polySetFixe(p, TRUE);
					wdAddPolygon(lvlGetWorld(Lvl), p);

					delList(LPoly);
					break;
				}
				}

				break;
			case o_rigid:
			{
				unsigned int ID1, ID2;
				Vertex *V1, *V2;
				float Lenght;
				fscanf(f, "%u %u %f\n", &ID1, &ID2, &Lenght);
				V1 = GetVertexFromID(vxL, ID1);
				V2 = GetVertexFromID(vxL, ID2);
				Rigid* r = newRigid(V1, V2, Lenght);
				wdAddRigid(lvlGetWorld(Lvl), r);
				break;
			}
			case o_elastic:
			{
				unsigned int ID1, ID2;
				Vertex *V1, *V2;
				float Lenght, Spring;
				fscanf(f, "%u %u %f %f\n", &ID1, &ID2, &Lenght, &Spring);
				V1 = GetVertexFromID(vxL, ID1);
				V2 = GetVertexFromID(vxL, ID2);
				Elastic* e = newElastic(V1, V2, Lenght, Spring);
				wdAddElastic(lvlGetWorld(Lvl), e);
				break;
			}
			case o_vertex:
			{
				printf("vertex\n");
				unsigned int vxID;
				float x, y, mass;
				Bool fixe;
				fscanf(f, "%u : %f, %f ; %f ; %i\n", &vxID, &x, &y, &mass, &fixe);
				printf("données lues: %u ; %f, %f ; %f; %i\n", vxID, x, y, mass, fixe);
				//on ajoute le vertex dans la liste
				ItemVertex *iV = (ItemVertex*)malloc(sizeof(ItemVertex));
				iV->ID=vxID;
				iV->V = newVertex();
				wdAddVertex(lvlGetWorld(Lvl), iV->V);
				vxSetPosition(iV->V, vec2(x, y));
				vxSetMass(iV->V, mass);
				vxSetFixe(iV->V, fixe);
				lstAdd(vxL, iV);


				break;
			}
			default:
				break;
		}
	}

	printf("niveau chargé: %s%sw:%f h:%f\n", lvl, description, width, height);
	fclose(f);

	//on libere la mémoire utilisée par la liste des vertex
	Node* it = lstFirst(vxL);

	while (!nodeEnd(it))
	{
		free((ItemVertex*)nodeGetData(it));
		it = nodeGetNext(it);
	}

	delList(vxL);

	return TRUE;
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
	if(Lvl->P1 != NULL) plSetOnGround(Lvl->P1, 0);

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
		if(Lvl->P1 != NULL)
		{
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
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}


Vertex* GetVertexFromID(List* L, unsigned int ID)
{
	Node* it = lstFirst(L);

	while (!nodeEnd(it))
	{
		if (((ItemVertex*)nodeGetData(it))->ID==ID)
			return ((ItemVertex*)nodeGetData(it))->V;
		it = nodeGetNext(it);
	}

	return NULL;
}
