#include "PolygonEl.h"

PolygonEl* newPolyElgonEl(unsigned int nbVx, ...)
{
	unsigned int i;
	va_list ap;
	PolygonEl* newPolyEl = (PolygonEl*) malloc(sizeof(PolygonEl));

	/* Pas d'appel à polyelInit possible à cause de la liste d'arguments...

	Initialisation des Dynamic Arrays */
	newPolyEl->Elastics = da();
	newPolyEl->Vertices = da();
	newPolyEl->InternalElastics = da();
	daReserve(&newPolyEl->Elastics, nbVx);
	daReserve(&newPolyEl->Vertices, nbVx);
	newPolyEl->Center = NULL;

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPolyEl->Vertices, va_arg(ap, Vertex*));
	va_end(ap);
	/* Construit les limites, i.e. Créé un nouveau Elastic à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPolyEl->Elastics, newElastic((Vertex*)daGet(&newPolyEl->Vertices, i),
			(Vertex*)daGet(&newPolyEl->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&newPolyEl->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&newPolyEl->Vertices, (i+1)%nbVx)))), 0.1f));
	return newPolyEl;
}

PolygonEl* newPolyElgonElL(List L)
{
	unsigned int i = 0, nbVx = lstCount(&L);
	PolygonEl* newPolyEl = (PolygonEl*) malloc(sizeof(PolygonEl));
	Node* it = lstFirst(&L);
	/*Initialisation des Dynamic Arrays */
	newPolyEl->Elastics = da();
	newPolyEl->Vertices = da();
	newPolyEl->InternalElastics = da();
	daReserve(&newPolyEl->Elastics, nbVx);
	daReserve(&newPolyEl->Vertices, nbVx);
	newPolyEl->Center = NULL;

	/* Ajoute les Vertices */
	while(!nodeEnd(it))
	{
		daAdd(&newPolyEl->Vertices, (Vertex*) nodeGetData(it));
		it = nodeGetNext(it);
	}

	/* Construit les limites, i.e. Créé un nouveau Elastic à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPolyEl->Elastics, newElastic((Vertex*)daGet(&newPolyEl->Vertices, i),
			(Vertex*)daGet(&newPolyEl->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&newPolyEl->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&newPolyEl->Vertices, (i+1)%nbVx)))), 0.1f));
	return newPolyEl;
}

void polyelInit(PolygonEl* P, unsigned int nbVx, ...)
{
	unsigned int i;
	va_list ap;

	/* Initialisation des Dynamic Arrays */
	P->Elastics = da();
	P->Vertices = da();
	P->InternalElastics = da();
	daReserve(&P->Elastics, nbVx);
	daReserve(&P->Vertices, nbVx);
	P->Center = NULL;

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Vertices, va_arg(ap, Vertex*));
	va_end(ap);
	/* Construit les limites, i.e. Créé un nouveau Elastic à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Elastics, newElastic((Vertex*)daGet(&P->Vertices, i),
			(Vertex*)daGet(&P->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition((Vertex*)daGet(&P->Vertices, i)),
					vxGetPosition((Vertex*)daGet(&P->Vertices, (i+1)%nbVx)))), 0.1f));
}

void delPolygonEl(PolygonEl* P)
{
	unsigned int i;

	/* Destruction des liaisons */
	for(i = 0; i < daGetSize(&P->Elastics); i++)
	{
		delElastic((Elastic*)daGet(&P->Elastics, i));
	}
	for(i = 0; i < daGetSize(&P->InternalElastics); i++)
	{
		delElastic((Elastic*)daGet(&P->InternalElastics, i));
	}

	/* Libération des Dynamic Arrays */
	daFree(&P->Elastics);
	daFree(&P->Vertices);
	daFree(&P->InternalElastics);

	if(P->Center != NULL) delVertex(P->Center), P->Center = NULL, printf("Éliminéee!\n");

	free(P);
}

void polyelAddInternal(PolygonEl* P, unsigned int V1, unsigned int V2, float Length, float Spring)
{
	daAdd(&P->InternalElastics, newElastic((Vertex*)daGet(&P->Vertices, V1),
										(Vertex*)daGet(&P->Vertices, V2),
										Length, Spring));
}

PolygonEl* polyelRectangle(Vertex* V1, Vertex* V2, Vertex* V3, Vertex* V4)
{
	PolygonEl* newRectangle = newPolyElgonEl(4, V1, V2, V3, V4);
	polyelAddInternal(newRectangle, 0, 2, -1, 0.1f);
	polyelAddInternal(newRectangle, 1, 3, -1, 0.1f);
	return newRectangle;
}

PolygonEl* polyelRectangleL(List L)
{
	PolygonEl* newRectangle = newPolyElgonElL(L);
	polyelAddInternal(newRectangle, 0, 2, -1, 0.1f);
	polyelAddInternal(newRectangle, 1, 3, -1, 0.1f);
	return newRectangle;
}

PolygonEl* polyelNGone(List L)
{
	unsigned int i;
	PolygonEl* newNGone = newPolyElgonElL(L);
	newNGone->Center = newVertex();
	vxSetPosition(newNGone->Center, polyelComputeCenter(newNGone));
	for(i = 0; i < daGetSize(&newNGone->Vertices); i++)
		daAdd(&newNGone->InternalElastics, newElastic((Vertex*)daGet(&newNGone->Vertices, i),
										newNGone->Center, -1.f, 0.1f));
	return newNGone;
}

void polyelProject(PolygonEl* P, float* Min, float* Max, Vec2 Axis)
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

CollisionInfoEl nullCollisionInfoEl()
{
	CollisionInfoEl tmp;
	tmp.Depth = 0;
	tmp.Edge = NULL;
	tmp.Normal = vec2(0, 0);
	tmp.P1 = NULL;
	tmp.P2 = NULL;
	tmp.V = NULL;
	return tmp;
}

CollisionInfoEl polyelCollide(PolygonEl* P1, PolygonEl* P2)
{
	unsigned int i;
	Vec2 Axis, CenterP1;
	Elastic* Edge;
	CollisionInfoEl Info = nullCollisionInfoEl();
	Info.P1 = P1;
	Info.P2 = P2;
	Info.Depth = INFINITY; /* On recherche un minimum */
	float MinP1, MaxP1, MinP2, MaxP2, Gap, tmpGap;

	/* On itère sur les faces des deux polyelgones */
	for(i = 0; i < daGetSize(&P1->Elastics) + daGetSize(&P2->Elastics); i++)
	{
		/* On récupère la face à tester */
		if(i < daGetSize(&P1->Elastics))
			Edge = (Elastic*) daGet(&P1->Elastics, i);
		else Edge = (Elastic*) daGet(&P2->Elastics, i - daGetSize(&P1->Elastics));

		/* On ne teste surtout pas une face nulle... */
		if(vec2Equal(elasticVector(Edge), vec2(0.f, 0.f))) continue;

		/* On calcule l'axe sur lequel projeter (Normal à la face) */
		Axis = vec2Normalized(vec2Ortho(elasticVector(Edge)));

		/* Projection */
		polyelProject(P1, &MinP1, &MaxP1, Axis);
		polyelProject(P2, &MinP2, &MaxP2, Axis);

		if(MinP1 < MinP2)
			Gap = MinP2 - MaxP1;
		else
			Gap = MinP1 - MaxP2;

		/* Pas de collision */
		if(Gap >= 0) return nullCollisionInfoEl();

		/* Il y a "collision" sur cet axe, on cherche le point d'entrée,
		c'est probablement le plus proche du bord... */
		if(-Gap < Info.Depth)
		{
			Info.Depth = -Gap;
			Info.Normal = Axis;
			Info.Edge = Edge;
			/* On s'assure que Info.P1 est toujours bien celui
			dont on test une face */
			if(i < daGetSize(&P1->Elastics))
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
	if(Info.Edge == NULL) return nullCollisionInfoEl();

	CenterP1 = polyelComputeCenter(Info.P1);

	/* On s'assure que la normale est dans le bon sens (pointe vers P2) */
	if(vec2Dot(Info.Normal, vec2Sub(polyelComputeCenter(Info.P2), CenterP1)) < 0)
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

Bool polyelIsInside(PolygonEl* P, Vertex* V)
{
	unsigned int i;
	Elastic* Edge;
	Vec2 Axis, VPos = vxGetPosition(V);
	float MinP, MaxP, ProjectV;
	for(i = 0; i < daGetSize(&P->Elastics); i++)
	{
		Edge = (Elastic*)daGet(&P->Elastics, i);
		/* On évite les faces "nulles" */
		if(vec2Equal(elasticVector(Edge), vec2(0.f, 0.f))) continue;

		/* On calcule l'axe sur lequel projeter (Normal à la face) */
		Axis = vec2Normalized(vec2Ortho(elasticVector(Edge)));

		/* Projection */
		polyelProject(P, &MinP, &MaxP, Axis);
		ProjectV = vec2Dot(VPos, Axis);

		/* Si la projection du point n'est pas dans l'intervalle
		défini par le polyelgone, pas de collision */
		if(ProjectV < MinP || ProjectV > MaxP)
			return false;
	}
	/* Toutes les faces ont été testées */
	return true;
}

Vec2 polyelComputeCenter(PolygonEl* P)
{
	unsigned int i;
	Vec2 Center = vxGetPosition((Vertex*)daGet(&P->Vertices, 0));
	for(i = 1; i < daGetSize(&P->Vertices); i++)
		Center = vec2Add(Center, vxGetPosition((Vertex*)daGet(&P->Vertices, i)));
	return vec2Div(Center, daGetSize(&P->Vertices));
}

void polyelResolve(PolygonEl* P)
{
	unsigned int i;
	for(i = 0; i < daGetSize(&P->Elastics); i++)
		elasticResolve((Elastic*)daGet(&P->Elastics, i));
	for(i = 0; i < daGetSize(&P->InternalElastics); i++)
		elasticResolve((Elastic*)daGet(&P->InternalElastics, i));
}

Bool polyelIsFixe(PolygonEl* P)
{
	return P->Fixe;
}

Vertex* polyelGetCenter(PolygonEl* P)
{
	return P->Center;
}

void polyelSetFixe(PolygonEl* P, Bool B)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		vxSetFixe((Vertex*)daGet(&P->Vertices, i), B);
	}
	if(polyelGetCenter(P) != NULL) vxSetFixe(polyelGetCenter(P), B);
	P->Fixe = B;
}

void polyelApplyForce(PolygonEl* P, Vec2 Force)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		vxApplyForce((Vertex*)daGet(&P->Vertices, i), Force);
	}
	if(polyelGetCenter(P) != NULL) vxApplyForce(polyelGetCenter(P), Force);
}

void polyelSetSpeed(PolygonEl* P, Vec2 Speed)
{
	unsigned int i;
	for (i = 0; i < daGetSize(&P->Vertices); i++)
	{
		((Vertex*)daGet(&P->Vertices, i))->OldPos=vec2Sub(((Vertex*)daGet(&P->Vertices, i))->Position, Speed);
	}
}

Vec2 polyelGetSpeed(PolygonEl* P)
{
	return vec2Sub(((Vertex*)daGet(&P->Vertices, 0))->Position, ((Vertex*)daGet(&P->Vertices, 0))->OldPos);
}

void polyelRegressionTest()
{
	printf("\n === Debut du test de regression de PolygonEl === \n\n");
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

	PolygonEl* Poly = newPolyElgonEl(3, V1, V2, V3);
	PolygonEl* Rectangle = polyelRectangle(V10, V11, V12, V13);

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
		polyelResolve(Poly);
		polyelResolve(Rectangle);
	}

	delPolygonEl(Rectangle);
	delPolygonEl(Poly);

	delVertex(V13);
	delVertex(V12);
	delVertex(V11);
	delVertex(V10);

	delVertex(V3);
	delVertex(V2);
	delVertex(V1);
	printf("\n === Fin du test de regression de PolygonEl === \n\n");
}
