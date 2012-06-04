#include "Level.h"
#include <Rendering/OpenGL.h> // Pour le type Texture (adapté à l'API)


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
	lstInit(&Lvl->Objects);
	Lvl->lvlTexLoad = &glTexLoad;
	Lvl->lvlTexFree = &glTexFree;
	Lvl->lvlDisplayTex = &glDisplayTex; ///@todo Mettre une fonction our charger ça
	Lvl->lvlDispTexPoly = &glDispTexPoly;
	Lvl->lvlDispFlag = &glDispFlag;
	Lvl->lvlDispPlayer = &glDispPlayer;
	Lvl->lvlDispGrass = &glDispGrass;
	Lvl->lvlDispRope = &glDispRope;
	Lvl->lvlDispChain = &glDispChain;
	Lvl->DistBG = Lvl->DistFG = 1.f;
	flInit(&Lvl->GoalFlag, 4.f, 4.f, 25, 40, Lvl->lvlTexLoad("data/trollface.jpg"), 0);
	Texture* ptrTex = (Texture*) malloc(sizeof(Texture));
	*ptrTex = Lvl->lvlTexLoad("data/s_ground.png");
	daAdd(&Lvl->Textures, ptrTex);
	Lvl->Finished = 0;
	Lvl->VoidTex = (*Lvl->lvlTexLoad)("");
	Lvl->Background = Lvl->VoidTex;
	Lvl->Foreground = Lvl->VoidTex;
	Lvl->Layer1 = Lvl->VoidTex;
	Lvl->Layer2 = Lvl->VoidTex;
}

void lvlFree(Level* Lvl)
{
	unsigned int i;
	if(Lvl->P1 != NULL) delPlayer(Lvl->P1);
	if (Lvl->Background != Lvl->VoidTex) (*Lvl->lvlTexFree)(Lvl->Background);
	if (Lvl->Layer1 != Lvl->VoidTex) (*Lvl->lvlTexFree)(Lvl->Layer1);
	if (Lvl->Layer2 != Lvl->VoidTex) (*Lvl->lvlTexFree)(Lvl->Layer2);
	if (Lvl->Foreground != Lvl->VoidTex) (*Lvl->lvlTexFree)(Lvl->Foreground);
	(*Lvl->lvlTexFree)(Lvl->VoidTex);

	delWorld(Lvl->W);
	/*if (Lvl->C != NULL)
		delCloth(Lvl->C);
	Lvl->C = NULL;
	 */

	Node* it = lstFirst(&Lvl->Objects);
	while(!nodeEnd(it))
	{
		delObject((Object*) nodeGetData(it));
		it = nodeGetNext(it);
	}
	lstFree(&Lvl->Objects);

	for(i = 0; i < daGetSize(&Lvl->Textures); i++)
		(*Lvl->lvlTexFree)(*((Texture*) daGet(&Lvl->Textures, i))),
		free((Texture*) daGet(&Lvl->Textures, i));
	daFree(&Lvl->Textures);

	flFree(&Lvl->GoalFlag);
}

void delLevel(Level* lvl)
{
	lvlFree(lvl);
	free(lvl);
}

const char* lvlGetFilename(const Level* lvl)
{
	return lvl->Filename;
}

const char* lvlGetName(const Level* lvl)
{
	return lvl->Name;
}

const char* lvlGetDesc(const Level* lvl)
{
	return lvl->Desc;
}

const char* lvlGetMD5(const Level* lvl)
{
	return lvl->MD5;
}

Vec2 lvlGetSpawn(const Level* lvl)
{
	return lvl->Spawn;
}

Vec2 lvlGetGoal(const Level* lvl)
{
	return lvl->Goal;
}

Player* lvlGetP1(const Level* lvl)
{
	return lvl->P1;
}

Bool lvlIsFinished(const Level* lvl)
{
	return lvl->Finished;
}

void lvlSetFinished(Level* Lvl, Bool B)
{
	Lvl->Finished = B;
}

Bool lvlLoad(Level* Lvl, const char* File)
{
	printf("= Chargement... ========\n");

	/* Recherche du nom du fichier */
	char Path[255], Name[255];
	char* tmp;
	strcpy(Path, File);
	tmp = strtok(Path , "/");
	while(tmp != NULL)
	{
		strcpy(Name, tmp);
		tmp = strtok(NULL, "/");
	}

	strcpy(Lvl->MD5, md5FromFile(File).c_str());
	printf("MD5 : %s\n", Lvl->MD5);


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
		printf("Le fichier %s ne peut pas être lu (Erreur de lecture du nom)\n", File);
		return FALSE;
	}

	if (fgets(description, 255, f)==NULL)
	{
		printf("Le fichier %s ne peut pas être lu (Erreur de lecture de la description)\n", File);
		return FALSE;
	}

	float width, height, DistBG = 1.f, DistFG = 1.f;

	if (fgets(read, 300, f)==NULL)
	{
		printf("Le fichier %s ne peut pas être lu (Erreur de lecture #3)\n", File);
		return FALSE;
	}
	else
		sscanf(read, "%f, %f ; %f, %f", &width, &height, &DistBG, &DistFG);

	lvlFree(Lvl);

	lvlInit(Lvl, width, height);

	strcpy(Lvl->Filename, Name);
	printf("Filename : %s\n", Lvl->Filename);

	strcpy(Lvl->Name, lvl);
	Lvl->Name[strlen(Lvl->Name) - 1] = '\0'; // On enlève le CR
	strcpy(Lvl->Desc, description);
	Lvl->Desc[strlen(Lvl->Desc) - 1] = '\0'; // On enlève le CR

	lvlSetDistBG(Lvl, DistBG);
	lvlSetDistFG(Lvl, DistFG);

	if (fgets(read, 300, f)==NULL)
	{
		printf("Le fichier %s ne peut pas être lu (Erreur de lecture #4)\n", File);
		return FALSE;
	}
	else sscanf(read, "%f, %f ; %f, %f", &Lvl->Spawn.x, &Lvl->Spawn.y, &Lvl->Goal.x, &Lvl->Goal.y);

	unsigned int item, nVertex, i;
	Bool polyFixed; int booly;

	// Chargement des textures de fonds
	// Si le fichier n'existe pas (ou que la ligne est vide)
	// Une texture transparente est crée

	//back
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	Lvl->Background = (*Lvl->lvlTexLoad)(read);

	//layer 1
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	Lvl->Layer1 = (*Lvl->lvlTexLoad)(read);

	//layer 2
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	Lvl->Layer2 = (*Lvl->lvlTexLoad)(read);

	//foreground
	fgets(read, 255, f);
	*strstr(read, "\n") = '\0';
	Lvl->Foreground = (*Lvl->lvlTexLoad)(read);

	//liste des vertex
	DynArr* Vx = newDynArr();
	DynArr* Poly = newDynArr();

	while (fgets(read, 300, f)!=NULL)
	{
		item=o_end;
		polyFixed=FALSE;
		sscanf(read, "%u %u %i #\n", &item, &nVertex, &booly);
		polyFixed = (Bool)booly;

		switch (item)
		{
			case o_poly:
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
						polySetFixed(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p); ///@todo accesseur
					}

					daAdd(Poly, p);
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
						p = newPolygon(4, V1, V2, V3, V4), polySetFixed(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p);
					}

					daAdd(Poly, p);
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
						p = newPolygonL(*LPoly), polySetFixed(p, TRUE);
						gridAddPolygonByBB(&lvlGetWorld(Lvl)->CollisionGrid, p);
					}

					daAdd(Poly, p);
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
				vxSetFixed(V, fixe);
				daAdd(Vx, V);


				break;
			}
			case o_texture:
			{
				char path[255];
				//printf("Texture lue \n");
				fscanf(f, "%s\n", path);
				//printf("Chargement de %s", path);
				Texture* ptrTex = (Texture*) malloc(sizeof(Texture));
				*ptrTex = Lvl->lvlTexLoad(path);
				daAdd(&Lvl->Textures, ptrTex);
				break;
			}
			case o_object:
			{
				unsigned int UserDefTexCoord;
				Polygon* Shape;
				List lstTex;

				fscanf(f, "%u\n", &UserDefTexCoord);
				Shape = (Polygon*) daGet(Poly, nVertex); //dans ce cas la variable contient l'index du polygone commencent à 0
				if (UserDefTexCoord==1)
				{
					lstInit(&lstTex); // Les nodes ne seront ppas libérés ici, car utilisés par Object, qui se chargera de les libérer
					for(unsigned int i = 0; i < polyGetVxCount(Shape); i++)
					{
						Vec2* CoordTex = newVec2();
						fscanf(f, "%f %f\n", &CoordTex->x, &CoordTex->y);
						lstAdd(&lstTex, CoordTex);
					}
				}
				else
					lvlCreateTexListForPolygon(Shape, &lstTex);

				Object* Obj = newObject(Shape, booly, lstTex); //booly est l'indice de la texture
				lstAdd(&Lvl->Objects, Obj);
				break;
			}
			default:
				break;
		}
	}

	printf("Niveau correctement charge : \n%s\n%s\nw:%f h:%f\n========================\n", Lvl->Name, Lvl->Desc, width, height);
	fclose(f);

	delDynArr(Poly);
	delDynArr(Vx);

	return TRUE;
}

void lvlCreateTexListForPolygon(Polygon* P, List* l)
{
	if (polyGetVxCount(P) == 4) //cas du rectangle
	{
		lstInit(l);
		Vec2* V;
		for (int i=0; i<4; i++)
		{
			V = newVec2();

			V->x = (i == 1 || i == 2);
			V->y = (i>1);
			printf("v added: %f,%f\n", V->x, V->y);

			lstAdd(l, V);
		}

		return;
	}
	//On cacule la bounding box
	BBox B = polyGetBBox(P);
	float w = B.Right - B.Left;
	float h = B.Bottom - B.Top;
	lstInit(l);
	Vec2 *V;
	for (unsigned int i=0; i<polyGetVxCount(P); i++)
	{
		V = newVec2();
		V->x = (vxGetPosition(polyGetVertex(P, i)).x-B.Left)/w;
		V->y = (vxGetPosition(polyGetVertex(P, i)).y-B.Top)/h;
		lstAdd(l, V);
	}
}

void lvlSetName(Level* lvl, const char* Name)
{
	strcpy(lvl->Name, Name);
}

void lvlSetDesc(Level* lvl, const char* Desc)
{
	strcpy(lvl->Desc, Desc);
}

void lvlLoadedInit(Level* Lvl)
{
	Lvl->P1 = newPlayer(lvlGetWorld(Lvl));

	plCorrectPosition(Lvl->P1, Lvl->Spawn);
}

void lvlUpdate(Level* Lvl, Bool Paused)
{
	unsigned int i;

	if (!Paused)
	{
		/* Mise à jour du World */
		//if(Lvl->P1 != NULL) vxSetFixed(Lvl->P1->Stable, 0);

		wdApplyForce(lvlGetWorld(Lvl), vec2(0.f, 0.6f));
		wdResolveVextex(lvlGetWorld(Lvl));

		if(!Lvl->Finished)
			flApplyForce(&Lvl->GoalFlag, vec2(0.f, 0.6f), 1);
		else
			flApplyForce(&Lvl->GoalFlag, vec2(5.f + (rand()%100)/20, 0.6f), 1);
		flResolve(&Lvl->GoalFlag, 0.5f, 0.5f);

		wdUpdateGrid(lvlGetWorld(Lvl), FALSE);
		for(i = 0; i < 4; i++) /* Augmenter Imax pour augmenter la précision */
		{
			wdResolveRigid(lvlGetWorld(Lvl));
			//if(Lvl->P1 != NULL) vxSetFixed(Lvl->P1->Stable, 1);
			wdResolveElastic(lvlGetWorld(Lvl));
			//if(Lvl->P1 != NULL) vxSetFixed(Lvl->P1->Stable, 0);
			wdHandleCollision(lvlGetWorld(Lvl));

			if (Lvl->P1 != NULL)
				plPhysics(Lvl->P1, lvlGetWorld(Lvl));

		}

		if (Lvl->P1 != NULL)
			plUpdate(Lvl->P1);
	}
	else
	{
		wdLimitVextexPosition(lvlGetWorld(Lvl));
		wdUpdateGrid(lvlGetWorld(Lvl), FALSE);
		for(int i = 0; i < 4; i++) /* Augmenter Imax pour augmenter la précision */
		{
			wdResolveRigid(lvlGetWorld(Lvl));
			wdHandleCollision(lvlGetWorld(Lvl));
		}
	}


}

Bool lvlIsGoalReached(const Level* L)
{
	BBox B = polyGetBBox(L->P1->Shape);
	if(B.Left < L->Goal.x && B.Right > L->Goal.x &&
		B.Top < L->Goal.y && B.Bottom > L->Goal.y)
		return 1;
	else
		return 0;
}

void lvlAddObject(Level* Lvl, Object * Obj)
{
	lstAdd(&Lvl->Objects, Obj);
}

void lvlDelObject(Level* Lvl, Object * Obj)
{
	lstDel(&Lvl->Objects, Obj);
	delObject(Obj);
}

World* lvlGetWorld(const Level* Lvl)
{
	return Lvl->W;
}

void lvlSetDistBG(Level* Lvl, float F)
{
	Lvl->DistBG = F;
}

void lvlSetDistFG(Level* Lvl, float F)
{
	Lvl->DistFG = F;
}

void lvlDisplayBG(const Level* Lvl, float X, float Y, float W, float H)
{
	float wdW = wdGetWidth(lvlGetWorld(Lvl));
	float wdH = wdGetHeight(lvlGetWorld(Lvl));
	float X1 = MIN(X, wdW - W);
	float Y1 = MIN(Y, wdH - H);
	X1 /= wdW;
	Y1 /= wdH;

	float X2 = X + W;
	X2 = MAX(0, MIN(X2, wdW));
	X2 /= wdW;
	float Y2 = Y + H;
	Y2 = MAX(0, MIN(Y2, wdH));
	Y2 /= wdH;

	float factor = Lvl->DistBG;
	float AddX = (X1 - X2)*0.5f*factor;
	float AddY = (Y1 - Y2)*0.5f*factor;

	Vec2 Center = vec2((X1 + X2)/2, (Y1 + Y2)/2);
	Vec2 vX1 = vec2(Center.x + AddX, Center.y + AddY);
	Vec2 vX2 = vec2(Center.x - AddX, Center.y - AddY);

	if(vX1.x < 0.f)
	{
		vX1.x = 0.f;
		vX2.x = factor*W/wdW;
	}

	if(vX1.y < 0.f)
	{
		vX1.y = 0.f;
		vX2.y = factor*H/wdH;
	}

	if(vX2.x > 1.f)
	{
		vX1.x = 1.f - factor*W/wdW;
		vX2.x = 1.f;
	}

	if(vX2.y > 1.f)
	{
		vX1.y = 1.f - factor*H/wdH;
		vX2.y = 1.f;
	}

	(*Lvl->lvlDisplayTex)(Lvl->Background, vX1, vec2(vX2.x, vX1.y), vX2, vec2(vX1.x, vX2.y),
						vec2(X, Y), vec2(X + W, Y),
						vec2(X + W, Y + H), vec2(X, Y + H));
}

void lvlDisplayPlayer(const Level* Lvl, s_SharedResources* SR)
{
	(*Lvl->lvlDispPlayer)(Lvl->P1, SR);
}

void lvlDisplayL1(const Level* Lvl)
{

	(*Lvl->lvlDisplayTex)(Lvl->Layer1, vec2(0, 0), vec2(1,0), vec2(1,1), vec2(0,1),
						vec2(0, 0), vec2(wdGetWidth(lvlGetWorld(Lvl)), 0),
						vec2(wdGetWidth(lvlGetWorld(Lvl)), wdGetHeight(lvlGetWorld(Lvl))), vec2(0, wdGetHeight(lvlGetWorld(Lvl))));
}

void lvlDisplayL2(const Level* Lvl)
{
	(*Lvl->lvlDisplayTex)(Lvl->Layer2, vec2(0, 0), vec2(1,0), vec2(1,1), vec2(0,1),
						vec2(0, 0), vec2(wdGetWidth(lvlGetWorld(Lvl)), 0),
						vec2(wdGetWidth(lvlGetWorld(Lvl)), wdGetHeight(lvlGetWorld(Lvl))), vec2(0, wdGetHeight(lvlGetWorld(Lvl))));
}

void lvlDisplayFG(const Level* Lvl, float X, float Y, float W, float H)
{
	float wdW = wdGetWidth(lvlGetWorld(Lvl));
	float wdH = wdGetHeight(lvlGetWorld(Lvl));
	float X1 = MIN(X, wdW - W);
	float Y1 = MIN(Y, wdH - H);
	X1 /= wdW;
	Y1 /= wdH;

	float X2 = X + W;
	X2 = MAX(0, MIN(X2, wdW));
	X2 /= wdW;
	float Y2 = Y + H;
	Y2 = MAX(0, MIN(Y2, wdH));
	Y2 /= wdH;

	float factor = 1.f/Lvl->DistBG;
	float AddX = (X1 - X2)*0.5f*factor;
	float AddY = (Y1 - Y2)*0.5f*factor;

	Vec2 Center = vec2((X1 + X2)/2, (Y1 + Y2)/2);
	Vec2 vX1 = vec2(Center.x + AddX, Center.y + AddY);
	Vec2 vX2 = vec2(Center.x - AddX, Center.y - AddY);

	if(vX1.x < 0.f)
	{
		vX1.x = 0.f;
		vX2.x = factor*W/wdW;
	}

	if(vX1.y < 0.f)
	{
		vX1.y = 0.f;
		vX2.y = factor*H/wdH;
	}

	if(vX2.x > 1.f)
	{
		vX1.x = 1.f - factor*W/wdW;
		vX2.x = 1.f;
	}

	if(vX2.y > 1.f)
	{
		vX1.y = 1.f - factor*H/wdH;
		vX2.y = 1.f;
	}

	(*Lvl->lvlDisplayTex)(Lvl->Foreground, vX1, vec2(vX2.x, vX1.y), vX2, vec2(vX1.x, vX2.y),
						vec2(X, Y), vec2(X + W, Y),
						vec2(X + W, Y + H), vec2(X, Y + H));
}

void lvlDisplayObj(const Level* Lvl, Object* Obj)
{
	(*Lvl->lvlDispTexPoly)(*(Texture*) daGet(&Lvl->Textures, objGetTexture(Obj)), objGetShape(Obj), &Obj->CoordTex);
}

void lvlDispAllObj(Level* Lvl)
{
	Node* it = lstFirst(&Lvl->Objects);
	while(!nodeEnd(it))
	{
		lvlDisplayObj(Lvl, (Object*) nodeGetData(it));
		it = nodeGetNext(it);
	}
}

void lvlDisplayElastics(const Level* Lvl, s_SharedResources* SR)
{
	Node* it;
	Elastic* E;
	it = lstFirst(&Lvl->W->Elastics);
	
	while (!nodeEnd(it))
	{
		E = (Elastic*)nodeGetData(it);
		(*Lvl->lvlDispRope)(E, SR);
		it = nodeGetNext(it);
	}
}

void lvlDisplayRigids(const Level* Lvl, s_SharedResources* SR)
{
	Node* it;
	Rigid* R;
	it = lstFirst(&Lvl->W->Rigids);
	
	while (!nodeEnd(it))
	{
		R = (Rigid*)nodeGetData(it);
		(*Lvl->lvlDispChain)(R, SR);
		it = nodeGetNext(it);
	}
}


void lvlDisplayGrass(Level* Lvl, s_SharedResources* SR)
{
	Node* it = lstFirst(&Lvl->Objects);
	while(!nodeEnd(it))
	{
		Object* Obj = (Object*)nodeGetData(it);
		if (Obj->Tex == 0)
			(*Lvl->lvlDispGrass)(objGetShape(Obj), SR);
		it = nodeGetNext(it);
	}
}

void lvlDispGoalFlag(Level* Lvl)
{
	Lvl->lvlDispFlag(&Lvl->GoalFlag, Lvl->Goal.x, Lvl->Goal.y);
}

Object* lvlGetObjFromShape(Level* Lvl, Polygon* P)
{
	Node* it = lstFirst(&Lvl->Objects);
	while(!nodeEnd(it))
	{
		if(objGetShape((Object*) nodeGetData(it)) == P)
			return (Object*) nodeGetData(it);
		it = nodeGetNext(it);
	}
	return NULL;
}

void lvlAddTexture(Level* Lvl, Texture T)
{
	Texture* ptrTex = (Texture*) malloc(sizeof(Texture));
	*ptrTex = T;
	daAdd(&Lvl->Textures, ptrTex);
}
