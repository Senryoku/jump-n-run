#include "Polygon.h"

Polygon* newPolygon(unsigned int nbVx, ...)
{
	int i;
	va_list ap;
	Polygon* newPoly = (Polygon*) malloc(sizeof(Polygon));

	/* Pas d'appel à polyInit possible à cause de la liste d'arguments...

	Initialisation des Dynamic Arrays */
	newPoly->Rigids = da();
	newPoly->Vertices = da();
	newPoly->InternalRigids = da();
	daReserve(&newPoly->Rigids, nbVx);
	daReserve(&newPoly->Vertices, nbVx);

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPoly->Vertices, va_arg(ap, Vertex*));
	/* Construit les limites, i.e. Créé un nouveau Rigid à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&newPoly->Rigids, newRigid(daGet(&newPoly->Vertices, i),
			daGet(&newPoly->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition(daGet(&newPoly->Vertices, i)),
					vxGetPosition(daGet(&newPoly->Vertices, (i+1)%nbVx))))));

	return newPoly;
}

void polyInit(Polygon* P, unsigned int nbVx, ...)
{
	int i;
	va_list ap;

	/* Initialisation des Dynamic Arrays */
	P->Rigids = da();
	P->Vertices = da();
	P->InternalRigids = da();
	daReserve(&P->Rigids, nbVx);
	daReserve(&P->Vertices, nbVx);

	va_start(ap, nbVx);
	/* Ajoute les Vertices */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Vertices, va_arg(ap, Vertex*));
	/* Construit les limites, i.e. Créé un nouveau Rigid à partir de
	deux Vertices de la liste et la distance les séparant, puis l'ajoute
	 à la liste */
	for(i = 0; i < nbVx; i++)
		daAdd(&P->Rigids, newRigid(daGet(&P->Vertices, i),
			daGet(&P->Vertices, (i+1)%nbVx),
			vec2Length(vec2Sub(vxGetPosition(daGet(&P->Vertices, i)),
					vxGetPosition(daGet(&P->Vertices, (i+1)%nbVx))))));
}

void delPolygon(Polygon* P)
{
	int i;

	/* Destruction des liaisons */
	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		delRigid(daGet(&P->Rigids, i));
	}
	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
		delRigid(daGet(&P->InternalRigids, i));
	}

	/* Libération des Dynamic Arrays */
	daFree(&P->Rigids);
	daFree(&P->Vertices);
	daFree(&P->InternalRigids);

	free(P);
}

void polyAddInternal(Polygon* P, unsigned int V1, unsigned int V2, float Length)
{
	/* Si la longueur n'est pas valide, on la calcule */
	if(Length <= 0)
		Length = vec2Length(vec2Sub(vxGetPosition(daGet(&P->Vertices, V1)),
									vxGetPosition(daGet(&P->Vertices, V2))));
	daAdd(&P->InternalRigids, newRigid(daGet(&P->Vertices, V1),
										daGet(&P->Vertices, V2),
										Length));
}

Polygon* polyRectangle(Vertex* V1, Vertex* V2, Vertex* V3, Vertex* V4)
{
	Polygon* newRectangle = newPolygon(4, V1, V2, V3, V4);
	polyAddInternal(newRectangle, 0, 2, -1);
	polyAddInternal(newRectangle, 1, 3, -1);
	return newRectangle;
}

void polyProject(Polygon* P, float* Min, float* Max, Vec2 Axis)
{
	int i;
	float tmp;
	*Min = *Max = vec2Dot(Axis, vxGetPosition(daGet(&P->Vertices, 0)));
	for(i = 1; i < daGetSize(&P->Vertices); i++)
	{
		tmp = vec2Dot(Axis, vxGetPosition(daGet(&P->Vertices, i)));
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
	Vec2 Axis, Center;
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
			Edge = daGet(&P1->Rigids, i);
		else
			Edge = daGet(&P2->Rigids, i - daGetSize(&P1->Rigids)),
			Info.P1 = P2, /* On s'assure que Info.P1 est toujours bien celui
			dont on test une face */
			Info.P2 = P1;

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
		if(Gap > 0) return nullCollisionInfo();

		/* Il y a "collision" sur cet axe, on cherche le point d'entrée,
		c'est probablement le plus proche du bord... */
		if(abs(Gap) < Info.Depth)
			Info.Depth = abs(Gap),
			Info.Normal = Axis,
			Info.Edge = Edge;
	}

	/* Cas extrème ou aucune face n'a été testée... */
	if(Info.Edge == NULL) return nullCollisionInfo();

	/* Recherche du point de collision dans Info.P2
	On suppose que c'est le plus proche de P1 !
	On intialise Gap à une valeur élevée... */
	Gap = INFINITY;
	Center = polyGetCenter(Info.P1);
	for(i = 0; i < daGetSize(&Info.P2->Vertices); i++)
	{
		/* Calcul de la distance P1 - Vertex */
		tmpGap = vec2Dot(Info.Normal, vec2Sub(
				vxGetPosition(daGet(&Info.P2->Vertices, i)),
				Center));
		if(tmpGap < Gap)
			Gap = tmpGap,
			Info.V = daGet(&Info.P2->Vertices, i);
	}

	return Info;
}

Vec2 polyGetCenter(Polygon* P)
{
	unsigned int i;
	Vec2 Center = vec2(0.f, 0.f);
	for(i = 0; i < daGetSize(&P->Vertices); i++)
		Center = vec2Add(Center, vxGetPosition(daGet(&P->Vertices, i)));
	return vec2Div(Center, daGetSize(&P->Vertices));
}

void polyTestRegression()
{
	printf("\n === Debut du test de regression de Polygon === \n\n");
	Vertex* V1 = newVertex();
	Vertex* V2 = newVertex();
	Vertex* V3 = newVertex();

	Vertex* V10 = newVertex();
	Vertex* V11 = newVertex();
	Vertex* V12 = newVertex();
	Vertex* V13 = newVertex();

	Polygon* Poly = newPolygon(3, V1, V2, V3);
	Polygon* Rectangle = polyRectangle(V10, V11, V12, V13);

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
