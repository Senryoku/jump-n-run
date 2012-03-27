#include "Polygon.h"

Polygon* newPolygon(unsigned int nbVx, ...)
{
	unsigned int i;
	va_list ap;
	Polygon* newPoly = (Polygon*) malloc(sizeof(Polygon));

	/* Pas d'appel à polyInit possible à cause de la liste d'arguments...

	Initialisation des Dynamic Arrays */
	newPoly->Rigids = da();
	newPoly->Vertices = da();
	newPoly->InternalRigids = da();
	daReserve(&newPoly->Rigids, nbVx);
	daReserve(&newPoly->Vertices, nbVx);
	newPoly->Center = NULL;
	newPoly->Fixe = FALSE;
	newPoly->GridPos.Valid = FALSE;
	newPoly->Collided = FALSE;

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPoly->Vertices, va_arg(ap, Vertex*));
	va_end(ap);
	/* Construit les limites, i.e. Créé un nouveau Rigid à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPoly->Rigids, newRigid((Vertex*)daGet(&newPoly->Vertices, i),
			(Vertex*)daGet(&newPoly->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&newPoly->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&newPoly->Vertices, (i+1)%nbVx))))));
	return newPoly;
}

Polygon* newPolygonL(List L)
{
	unsigned int i = 0, nbVx = lstCount(&L);
	Polygon* newPoly = (Polygon*) malloc(sizeof(Polygon));
	Node* it = lstFirst(&L);
	/*Initialisation des Dynamic Arrays */
	newPoly->Rigids = da();
	newPoly->Vertices = da();
	newPoly->InternalRigids = da();
	daReserve(&newPoly->Rigids, nbVx);
	daReserve(&newPoly->Vertices, nbVx);
	newPoly->Center = NULL;
	newPoly->Fixe = FALSE;
	newPoly->GridPos.Valid = FALSE;
	newPoly->Collided = FALSE;

	/* Ajoute les Vertices */
	while(!nodeEnd(it))
	{
		daAdd(&newPoly->Vertices, (Vertex*) nodeGetData(it));
		it = nodeGetNext(it);
	}

	/* Construit les limites, i.e. Créé un nouveau Rigid à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPoly->Rigids, newRigid((Vertex*)daGet(&newPoly->Vertices, i),
			(Vertex*)daGet(&newPoly->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&newPoly->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&newPoly->Vertices, (i+1)%nbVx))))));
	return newPoly;
}

void polyInit(Polygon* P, unsigned int nbVx, ...)
{
	unsigned int i;
	va_list ap;

	/* Initialisation des Dynamic Arrays */
	P->Rigids = da();
	P->Vertices = da();
	P->InternalRigids = da();
	daReserve(&P->Rigids, nbVx);
	daReserve(&P->Vertices, nbVx);
	P->Center = NULL;
	P->Fixe = FALSE;
	P->GridPos.Valid = FALSE;
	P->Collided = FALSE;

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Vertices, va_arg(ap, Vertex*));
	va_end(ap);
	/* Construit les limites, i.e. Créé un nouveau Rigid à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Rigids, newRigid((Vertex*)daGet(&P->Vertices, i),
			(Vertex*)daGet(&P->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&P->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&P->Vertices, (i+1)%nbVx))))));
	//printf("Poly %i init\n", P);
}

void delPolygon(Polygon* P)
{
	unsigned int i;

	/* Destruction des liaisons */
	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		delRigid((Rigid*)daGet(&P->Rigids, i));
	}
	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
		delRigid((Rigid*)daGet(&P->InternalRigids, i));
	}

	/* Libération des Dynamic Arrays */
	daFree(&P->Rigids);
	daFree(&P->Vertices);
	daFree(&P->InternalRigids);

	if(P->Center != NULL) delVertex(P->Center), P->Center = NULL, printf("Éliminéee!\n");

	//printf("Polygon %i deleted\n", P);

	free(P);
}

void polyAddInternal(Polygon* P, unsigned int V1, unsigned int V2, float Length)
{
	daAdd(&P->InternalRigids, newRigid((Vertex*)daGet(&P->Vertices, V1),
										(Vertex*)daGet(&P->Vertices, V2),
										Length));
}

Polygon* polyRectangle(Vertex* V1, Vertex* V2, Vertex* V3, Vertex* V4)
{
	Polygon* newRectangle = newPolygon(4, V1, V2, V3, V4);
	polyAddInternal(newRectangle, 0, 2, -1);
	polyAddInternal(newRectangle, 1, 3, -1);
	return newRectangle;
}

void polyUpdateGridPosition(Polygon* P, unsigned int Left, unsigned int Top, unsigned int Right, unsigned int Bottom)
{
	P->GridPos.Left=Left;
	P->GridPos.Right=Right;
	P->GridPos.Top=Top;
	P->GridPos.Bottom=Bottom;
	P->GridPos.Valid=TRUE;
}

Polygon* polyRectangleL(List L)
{
	Polygon* newRectangle = newPolygonL(L);
	polyAddInternal(newRectangle, 0, 2, -1);
	polyAddInternal(newRectangle, 1, 3, -1);
	return newRectangle;
}

Polygon* polyNGone(List L)
{
	unsigned int i;
	Polygon* newNGone = newPolygonL(L);
	newNGone->Center = newVertex();
	vxSetPosition(newNGone->Center, polyComputeCenter(newNGone));
	for(i = 0; i < daGetSize(&newNGone->Vertices); i++)
		daAdd(&newNGone->InternalRigids, newRigid((Vertex*)daGet(&newNGone->Vertices, i),
										newNGone->Center, -1.f));
	return newNGone;
}

void polyProject(Polygon* P, float* Min, float* Max, Vec2 Axis)
{
	unsigned int i;
	float tmp;
	*Min = *Max = vec2Dot(Axis, vxGetPosition((Vertex*)daGet(&P->Vertices, 0)));
	for(i = 1; i < daGetSize(&P->Vertices); i++)
	{
		tmp = vec2Dot(Axis, vxGetPosition((Vertex*)daGet(&P->Vertices, i)));
		*Min = (*Min > tmp) ? tmp : *Min;
		*Max = (*Max < tmp) ? tmp : *Max;
	}
}

CollisionInfo nullCollisionInfo()
{
	CollisionInfo tmp;
	tmp.Depth = 0;
	tmp.Edge = NULL;
	tmp.Normal = vec2(0, 0);
	tmp.P1 = NULL;
	tmp.P2 = NULL;
	tmp.V = NULL;
	return tmp;
}

CollisionInfo polyCollide(Polygon* P1, Polygon* P2)
{
	unsigned int i;
	Vec2 Axis, CenterP1;
	Rigid* Edge;
	CollisionInfo Info = nullCollisionInfo();
	Info.P1 = P1;
	Info.P2 = P2;
	Info.Depth = INFINITY; /* On recherche un minimum */
	float MinP1, MaxP1, MinP2, MaxP2, Gap, tmpGap;

	/* On itère sur les faces des deux polygones */
	for(i = 0; i < daGetSize(&P1->Rigids) + daGetSize(&P2->Rigids); i++)
	{
		/* On récupère la face à tester */
		if(i < daGetSize(&P1->Rigids))
			Edge = (Rigid*) daGet(&P1->Rigids, i);
		else Edge = (Rigid*) daGet(&P2->Rigids, i - daGetSize(&P1->Rigids));

		/* On ne teste surtout pas une face nulle... */
		if(vec2Equal(rdVector(Edge), vec2(0.f, 0.f))) continue;

		/* On calcule l'axe sur lequel projeter (Normal à la face) */
		Axis = vec2Normalized(vec2Ortho(rdVector(Edge)));

		/* Projection */
		polyProject(P1, &MinP1, &MaxP1, Axis);
		polyProject(P2, &MinP2, &MaxP2, Axis);

		if(MinP1 < MinP2)
			Gap = MinP2 - MaxP1;
		else
			Gap = MinP1 - MaxP2;

		/* Pas de collision */
		if(Gap >= 0) return nullCollisionInfo();

		/* Il y a "collision" sur cet axe, on cherche le point d'entrée,
		c'est probablement le plus proche du bord... */
		if(-Gap < Info.Depth)
		{
			Info.Depth = -Gap;
			Info.Normal = Axis;
			Info.Edge = Edge;
			/* On s'assure que Info.P1 est toujours bien celui
			dont on test une face */
			if(i < daGetSize(&P1->Rigids))
			{
				Info.P1 = P1;
				Info.P2 = P2;
			} else {
				Info.P1 = P2;
				Info.P2 = P1;
			}
		}
	}

	/* Cas extrème ou aucune face n'a été testée... */
	if(Info.Edge == NULL) return nullCollisionInfo();

	CenterP1 = polyComputeCenter(Info.P1);

	/* On s'assure que la normale est dans le bon sens (pointe vers P2) */
	if(vec2Dot(Info.Normal, vec2Sub(polyComputeCenter(Info.P2), CenterP1)) < 0)
		Info.Normal = vec2Prod(Info.Normal, -1.f);

	/* Recherche du point de collision dans Info.P2
	On suppose que c'est le plus proche de P1 !
	On intialise Gap à une valeur élevée... */
	Gap = INFINITY;
	for(i = 0; i < daGetSize(&Info.P2->Vertices); i++)
	{
		/* Calcul de la "distance P1" - Vertex */
		tmpGap = vec2Dot(Info.Normal, vec2Sub(
					vxGetPosition((Vertex*) daGet(&Info.P2->Vertices, i)),
					CenterP1));
		if(tmpGap < Gap)
			Gap = tmpGap,
			Info.V = (Vertex*) daGet(&Info.P2->Vertices, i);
	}

	return Info;
}

Bool polyIsInside(Polygon* P, Vertex* V)
{
	unsigned int i;
	Rigid* Edge;
	Vec2 Axis, VPos = vxGetPosition(V);
	float MinP, MaxP, ProjectV;
	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		Edge = (Rigid*)daGet(&P->Rigids, i);
		/* On évite les faces "nulles" */
		if(vec2Equal(rdVector(Edge), vec2(0.f, 0.f))) continue;

		/* On calcule l'axe sur lequel projeter (Normal à la face) */
		Axis = vec2Normalized(vec2Ortho(rdVector(Edge)));

		/* Projection */
		polyProject(P, &MinP, &MaxP, Axis);
		ProjectV = vec2Dot(VPos, Axis);

		/* Si la projection du point n'est pas dans l'intervalle
		défini par le polygone, pas de collision */
		if(ProjectV < MinP || ProjectV > MaxP)
			return false;
	}
	/* Toutes les faces ont été testées */
	return true;
}

Vec2 polyComputeCenter(Polygon* P)
{
	unsigned int i;
	Vec2 Center = vxGetPosition((Vertex*)daGet(&P->Vertices, 0));
	for(i = 1; i < daGetSize(&P->Vertices); i++)
		Center = vec2Add(Center, vxGetPosition((Vertex*)daGet(&P->Vertices, i)));
	return vec2Div(Center, daGetSize(&P->Vertices));
}

void polyResolve(Polygon* P)
{
	unsigned int i;
	for(i = 0; i < daGetSize(&P->Rigids); i++)
		rdResolve((Rigid*)daGet(&P->Rigids, i));
	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
		rdResolve((Rigid*)daGet(&P->InternalRigids, i));
}

Bool polyHasCollided(const Polygon* P)
{
	return P->Collided;
}

void polySetCollided(Polygon* P, Bool Collided)
{
	P->Collided=Collided;
}

Bool polyIsFixe(Polygon* P)
{
	return P->Fixe;
}

Vertex* polyGetCenter(Polygon* P)
{
	return P->Center;
}

Vertex* polyGetVertex(Polygon* P, unsigned int i)
{
	return (Vertex*) daGet(&P->Vertices, i);
}

void polySetFixe(Polygon* P, Bool B)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		vxSetFixe((Vertex*)daGet(&P->Vertices, i), B);
	}
	if(polyGetCenter(P) != NULL) vxSetFixe(polyGetCenter(P), B);
	P->Fixe = B;
}

void polyApplyForce(Polygon* P, Vec2 Force)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		vxApplyForce((Vertex*)daGet(&P->Vertices, i), Force);
	}
	if(polyGetCenter(P) != NULL) vxApplyForce(polyGetCenter(P), Force);
}

void polySetSpeed(Polygon* P, Vec2 Speed)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		((Vertex*)daGet(&P->Vertices, i))->OldPos=vec2Sub(((Vertex*)daGet(&P->Vertices, i))->Position, Speed);
	}
}

Vec2 polyGetSpeed(Polygon* P)
{
	return vec2Sub(((Vertex*)daGet(&P->Vertices, 0))->Position, ((Vertex*)daGet(&P->Vertices, 0))->OldPos);
}

BBox polyGetBBox(Polygon* P)
{
	BBox B; unsigned int i;
	B.Left = B.Right = vxGetPosition((Vertex*)daGet(&P->Vertices, 0)).x;
	B.Top = B.Bottom = vxGetPosition((Vertex*)daGet(&P->Vertices, 0)).y;

	for(i = 1; i < daGetSize(&P->Vertices); i++)
	{
		Vertex* V = (Vertex*) daGet(&P->Vertices, i);
		if (vxGetPosition(V).x < B.Left) B.Left=vxGetPosition(V).x;
		if (vxGetPosition(V).y < B.Top) B.Top=vxGetPosition(V).y;
		if (vxGetPosition(V).x > B.Right) B.Right=vxGetPosition(V).x;
		if (vxGetPosition(V).y > B.Bottom) B.Bottom=vxGetPosition(V).y;
	}

	return B;
}

void polyRegressionTest()
{
	printf("\n === Debut du test de regression de Polygon === \n\n");
	unsigned int i;
	float prevdt = 1.f, dt = 1.f;
	Vertex* V1 = newVertex();
	Vertex* V2 = newVertex();
	Vertex* V3 = newVertex();

	Vertex* V10 = newVertex();
	Vertex* V11 = newVertex();
	Vertex* V12 = newVertex();
	Vertex* V13 = newVertex();

	vxSetPosition(V1, vec2(0.f, 0.f));
	vxSetPosition(V2, vec2(10.f, 0.f));
	vxSetPosition(V3, vec2(0.f, 10.f));

	vxSetPosition(V10, vec2(20.f, 0.f));
	vxSetPosition(V11, vec2(30.f, 0.f));
	vxSetPosition(V12, vec2(30.f, 10.f));
	vxSetPosition(V13, vec2(20.f, 10.f));

	Polygon* Poly = newPolygon(3, V1, V2, V3);
	Polygon* Rectangle = polyRectangle(V10, V11, V12, V13);

	for(i = 0; i < 10; i++)
	{
		printf("Frame #%d\n", i);
		printf("Triangle : (%f, %f) (%f, %f) (%f, %f)\n\n",
				vxGetPosition(V1).x, vxGetPosition(V1).y,
				vxGetPosition(V2).x, vxGetPosition(V2).y,
				vxGetPosition(V3).x, vxGetPosition(V3).y);
		printf("Rectangle : (%f, %f) (%f, %f) \n (%f, %f), (%f, %f)\n\n",
				vxGetPosition(V10).x, vxGetPosition(V10).y,
				vxGetPosition(V11).x, vxGetPosition(V11).y,
				vxGetPosition(V12).x, vxGetPosition(V12).y,
				vxGetPosition(V13).x, vxGetPosition(V13).y);
		vxApplyForce(V1, vec2(1.f, 0.f));
		vxApplyForce(V10, vec2(1.f, 0.f));
		vxResolve(V1, prevdt, dt);
		vxResolve(V2, prevdt, dt);
		vxResolve(V3, prevdt, dt);
		vxResolve(V10, prevdt, dt);
		vxResolve(V11, prevdt, dt);
		vxResolve(V12, prevdt, dt);
		vxResolve(V13, prevdt, dt);
		polyResolve(Poly);
		polyResolve(Rectangle);
	}

	delPolygon(Rectangle);
	delPolygon(Poly);

	delVertex(V13);
	delVertex(V12);
	delVertex(V11);
	delVertex(V10);

	delVertex(V3);
	delVertex(V2);
	delVertex(V1);
	printf("\n === Fin du test de regression de Polygon === \n\n");
}
