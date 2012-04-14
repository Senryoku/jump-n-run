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
	daInit(&Lvl->Textures);
	daInit(&Lvl->Objects);
	Lvl->lvlTexLoad = &glTexLoad;
	Lvl->lvlTexFree = &glTexFree;
	Lvl->lvlDisplayTex = &glDisplayTex;
}

void lvlFree(Level* Lvl)
{
	unsigned int i;
	if(Lvl->P1 != NULL) delPlayer(Lvl->P1);
	(*Lvl->lvlTexFree)(Lvl->Background);
	(*Lvl->lvlTexFree)(Lvl->Layer1);
	(*Lvl->lvlTexFree)(Lvl->Layer2);
	(*Lvl->lvlTexFree)(Lvl->Foreground);
	delWorld(Lvl->W);
	for(i = 0; i < daGetSize(&Lvl->Textures); i++)
		Lvl->lvlTexFree(*((Texture*) daGet(&Lvl->Textures, i))),
		free((Texture*) daGet(&Lvl->Textures, i));
	daFree(&Lvl->Objects);
	daFree(&Lvl->Textures);
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

	//on ignore 4 lignes pour des images car elles sont lues dans leveleditor
	unsigned int item, nVertex, i;
	Bool polyFixed; int booly;

	//back
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	if (strcmp(read, ""))
		Lvl->Background = (*Lvl->lvlTexLoad)(read);

	//layer 1
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	if (strcmp(read, ""))
		Lvl->Layer1 = (*Lvl->lvlTexLoad)(read);
	//layer 2
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	if (strcmp(read, ""))
		Lvl->Layer2 = (*Lvl->lvlTexLoad)(read);

	//foreground
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	if (strcmp(read, ""))
		Lvl->Foreground = (*Lvl->lvlTexLoad)(read);



	//On libere le monde et on le realloue
	wdFree(Lvl->W);
	wdInit(Lvl->W, width, height);



	//liste des vertex
	DynArr* Vx = newDynArr();

	while (fgets(read, 300, f)!=NULL)
	{
		//printf("Read: %s", read);
		item=o_end;
		polyFixed=FALSE;
		sscanf(read, "%u %u %i #\n", &item, &nVertex, &booly);
		polyFixed = (Bool)booly;

		switch (item)
		{
			case o_poly:
				//printf("Polygon with %u vertex read\n", nVertex);
				switch (nVertex)
				{
					case 0:
					case 1:
						break;
					case 2:
					case 3:
					{
					Vertex **V = (Vertex**)malloc(sizeof(Vertex)*nVertex);
					unsigned int ID;
					for (i=0; i<nVertex; i++)
					{
						fscanf(f, "%u\n", &ID);
						V[i]=(Vertex*)daGet(Vx, ID);
					}
					Polygon* p;
					if (nVertex==2)
						p = newPolygon(2, V[0], V[1]);
					else
						p = newPolygon(3, V[0], V[1], V[2]);
					if (polyFixed)
					{
						polySetFixe(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p); ///@todo accesseur
					}

					wdAddPolygon(lvlGetWorld(Lvl), p);
					free(V);

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
						V1 = (Vertex*)daGet(Vx, ID1);
					V2 = (Vertex*)daGet(Vx, ID2);
					V3 = (Vertex*)daGet(Vx, ID3);
					V4 = (Vertex*)daGet(Vx, ID4);
					Polygon* p;
					if (!polyFixed)
						p = polyRectangle(V1, V2, V3, V4);
					else
					{
						p = newPolygon(4, V1, V2, V3, V4), polySetFixe(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p);
					}
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
						lstAdd(LPoly, (Vertex*)daGet(Vx, ID));
					}
					Polygon* p;
					if (!polyFixed)
						p = polyNGone(*LPoly);
					else
					{
						p = newPolygonL(*LPoly), polySetFixe(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p);
					}

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
				V1 = (Vertex*)daGet(Vx, ID1);
				V2 = (Vertex*)daGet(Vx, ID2);
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
				printf("elastic entre %u et %u\n", ID1, ID2);
				V1 = (Vertex*)daGet(Vx, ID1);
				V2 = (Vertex*)daGet(Vx, ID2);
				Elastic* e = newElastic(V1, V2, Lenght, Spring);
				wdAddElastic(lvlGetWorld(Lvl), e);
				break;
			}
			case o_vertex:
			{
				//printf("vertex\n");
				float x, y, mass;
				int booly;
				Bool fixe;
				fscanf(f, "%f, %f ; %f ; %i\n", &x, &y, &mass, &booly);
				fixe=(Bool)booly;
				//printf("données lues: %f, %f ; %f; %i\n", x, y, mass, booly);
				//on ajoute le vertex dans la liste
				Vertex* V = newVertex();
 				wdAddVertex(lvlGetWorld(Lvl), V);
				vxSetPosition(V, vec2(x, y));
				vxSetMass(V, mass);
				vxSetFixe(V, fixe);
				daAdd(Vx, V);


				break;
			}
			case o_texture:
			{
				char path[255];
				printf("Texture lue \n");
				fscanf(f, "%s\n", path);
				printf("Chargement de %s", path);
				Texture* ptrTex = (Texture*) malloc(sizeof(Texture));
				*ptrTex = Lvl->lvlTexLoad(path);
				daAdd(&Lvl->Textures, ptrTex);
				break;
			}
			case o_object:
				printf("Objet : \n");
				break;
			default:
				break;
		}
	}

	printf("niveau chargé: %s%sw:%f h:%f\n", lvl, description, width, height);
	fclose(f);


	delDynArr(Vx);

	return TRUE;
}

void lvlLoadedInit(Level* Lvl)
{
	/**@todo A modifier !!!!! Valeurs de test temporaires. Maintenant que j'y pense, il n'y aurai pas des leaks à cause des newqqchose ? car ils allouent la structure mais cette structure n'est pas liberée, non? on en au peut etre fait qqpart, meme si world il fait des del. car je n'ai pas fait tres attention xDD */
	Vertex* V1 = newVertex();
	vxSetPosition(V1, vec2(0.f, 0.f));
	Vertex* V2 = newVertex();
	vxSetPosition(V2, vec2(50.f, 0.f));
	Vertex* V3 = newVertex();
	vxSetPosition(V3, vec2(50.f, 210.f));
	Vertex* V4 = newVertex();
	vxSetPosition(V4, vec2(0.f, 210.f));
	
	Polygon* Shape = polyRectangle(V1, V2, V3, V4);
	Lvl->P1 = newPlayer();
	plSetShape(Lvl->P1, Shape);
	
	Vertex* Stable = newVertex();
	vxSetFixe(Stable, 1);
	Vec2 pos = polyComputeCenter(Lvl->P1->Shape);
	pos.y-=20.f;
	 
	vxSetPosition(Stable, pos);
	
	/*Elastic *E1 = newElastic(V1, Stable, -1.f, 1.f),
	*E2 = newElastic(V2, Stable, -1.f, 1.f);
	wdAddElastic(Lvl->W, E1);
	wdAddElastic(Lvl->W, E2);
	 Lvl->P1->Stable=Stable;*/
	 
	
	plSetPosition(Lvl->P1, Lvl->Spawn);
	wdAddVxFromPoly(Lvl->W, Shape);
	
	plCreateVertex(Lvl->P1, lvlGetWorld(Lvl));
	plCreateRigids(Lvl->P1, lvlGetWorld(Lvl));
	plSetPosition(Lvl->P1, Lvl->Spawn.x+100.f, Lvl->Spawn.y+150.f);
	
	/*Elastic *ENeck, *EBase;
	ENeck = newElastic(Lvl->P1->Neck, Stable, -1.f, 1.f);
	EBase = newElastic(Lvl->P1->Base, Stable, -1.f, 1.f);
	
	wdAddElastic(Lvl->W, ENeck);
	wdAddElastic(Lvl->W, EBase);
	 */
}

void lvlUpdate(Level* Lvl)
{
	unsigned int i;
	CollisionInfo Info;
	Node* it;

	/* Mise à jour spécifique de Player */
	if(Lvl->P1 != NULL) plSetOnGround(Lvl->P1, 0);

	/* Mise à jour du World */
	//if(Lvl->P1 != NULL) vxSetFixe(Lvl->P1->Stable, 0);

	wdApplyForce(lvlGetWorld(Lvl), vec2(0.f, 0.6f));
	wdResolveVextex(lvlGetWorld(Lvl));

	wdUpdateGrid(lvlGetWorld(Lvl));
	for(i = 0; i < 4; i++) /* Augmenter Imax pour augmenter la précision */
	{
		wdResolveRigid(lvlGetWorld(Lvl));
		//if(Lvl->P1 != NULL) vxSetFixe(Lvl->P1->Stable, 1);
		wdResolveElastic(lvlGetWorld(Lvl));
		//if(Lvl->P1 != NULL) vxSetFixe(Lvl->P1->Stable, 0);
		wdHandleCollision(lvlGetWorld(Lvl));

		/* Mise à jour spécifique de Player */
		if(Lvl->P1 != NULL)
		{
			polyResolve(plGetShape(Lvl->P1));
			List LExtracted = gridGetPolygonList(&Lvl->W->CollisionGrid, Lvl->P1->Shape);
			it = lstFirst(&LExtracted);
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
			lstFree(&LExtracted);
		}
	}
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}

void lvlDisplayL1(Level* Lvl)
{
	(*Lvl->lvlDisplayTex)(Lvl->Layer1, vec2(0, 0), vec2(1,0), vec2(1,1), vec2(0,1),
						vec2(0, 0), vec2(wdGetWidth(lvlGetWorld(Lvl)), 0),
						vec2(wdGetWidth(lvlGetWorld(Lvl)), wdGetHeight(lvlGetWorld(Lvl))), vec2(0, wdGetHeight(lvlGetWorld(Lvl))));
}

void lvlDisplayL2(Level* Lvl)
{
	(*Lvl->lvlDisplayTex)(Lvl->Layer2, vec2(0, 0), vec2(1,0), vec2(1,1), vec2(0,1),
						vec2(0, 0), vec2(wdGetWidth(lvlGetWorld(Lvl)), 0),
						vec2(wdGetWidth(lvlGetWorld(Lvl)), wdGetHeight(lvlGetWorld(Lvl))), vec2(0, wdGetHeight(lvlGetWorld(Lvl))));
}

