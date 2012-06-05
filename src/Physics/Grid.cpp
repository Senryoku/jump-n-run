#include "Grid.h"

#include <stdio.h>
#include <assert.h>
#include <SFML/OpenGL.hpp>

void gridInit(Grid* g, unsigned int HCells, unsigned int VCells)
{
	unsigned int i, j;
	g->HCells=HCells;
	g->VCells=VCells;
	g->Table=(List**)malloc(sizeof(List*)*HCells);
	for (i=0; i<HCells; i++)
		g->Table[i]=(List*)malloc(sizeof(List)*VCells);

	for (i=0; i<HCells; i++)
		for (j=0; j<VCells; j++)
			lstInit(&g->Table[i][j]);
}

void gridFree(Grid* g)
{
	unsigned int i, j;

	for (i=0; i<g->HCells; i++)
		for (j=0; j<g->VCells; j++)
			lstFree(&g->Table[i][j]);

	for (i=0; i<g->HCells; i++)
			free(g->Table[i]);
	free(g->Table);
	g->Table=NULL;
}

void gridSetCellSize(Grid* g, float Size)
{
	g->CellWidth=Size;
	g->CellHeight=Size;
}

List* gridGetCellList(const Grid* g, unsigned int x, unsigned int y)
{
	//assert(x < g->HCells && y < g->VCells);
	x = MAX(0, MIN(g->HCells-1, x));
	y = MAX(0, MIN(g->VCells-1, y));
	//printf("Acces to %u, %u; max: %u, %u\n", x, y, g->HCells, g->VCells);
	return &(g->Table[x][y]);
}

List* gridGetPositionList(const Grid* g, float x,float y)
{
	unsigned int i, j;
	x = MAX(0.f, MIN(g->HCells*g->CellWidth, x));
	y = MAX(0.f, MIN(g->VCells*g->CellHeight, y));
	i = x/g->CellWidth;
	j = y/g->CellHeight;

	return gridGetCellList(g, i, j);
}

void gridAddPolygon(Grid* g, Polygon* p) //Non fonctionnelle
{
	unsigned int i, j, k;
	int x1, y1, x2, y2; //Les int permetten de savoir si ça passe en dessous de 0 et de corriger ça

	for (i=0; i<daGetSize(&p->Vertices); i++)
	{
		if (daGetSize(&p->Vertices)==2 && i>0) continue;

		x1=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i)).x;
		y1=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i)).x;

		if (i==daGetSize(&p->Vertices)-1)
		{
			x2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, 0)).x;
			y2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, 0)).y;
		}
		else
		{
			x2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i+1)).x;
			y2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i+1)).y;
		}

		//On trouve les cases ou peut se toruver la ligne (Un rectangle) et on teste les intersections pour voir si la ligne se trouve ou pas dans cette cellule
		unsigned int cellx1=(unsigned int)x1/g->CellWidth, cellx2=(unsigned int)x2/g->CellWidth,
		celly1=(unsigned int)y1/g->CellHeight, celly2=(unsigned int)y2/g->CellHeight;

		printf("Want to add line going from %i, %i to %i, %i to rectangles of cells l,t;r,b: %u, %u; %u, %u\n",x1,y1,x2,y2,cellx1, celly1, cellx2, celly2);

		Vec2 v=vec2(x2-x1, y2-y1);
		printf("Vecteur de la droite: %f, %f\n", v.x, v.y);
		float a, b; //Coefficients de la droite y = ax + b
		unsigned int y;
		if (ABS(v.y)>ABS(v.x)) //On va regarder les intersections de la droite conteant le vecteur v avec les droites d'équation x=[cellx1..cellx2]
		{
			for (j=MIN(cellx1, cellx2)+1; j<=MAX(cellx1, cellx2); j++)
			{
				//On calcule l'y de l'intersection avec la droite d'equa x=j
				a=v.y/v.x; b=(float)y1-a*(float)x1;
				y=(a*j+b)/g->CellHeight+1;
				printf("Equation de la droite: y=%fx+%f ; intersection à %u %u(%f) (cellule)\n", a, b, j, y, (a*j+b)/g->CellHeight);

				for (k=MIN(y, MIN(celly1, celly2)); k<=MAX(y, MAX(celly1, celly2)); k++) //On ajoute le polygone a toutes les cases à gauche de l'intersection
					gridAddPolygonToCell(g, p, j-1, k);

			}
		}

		/*for (j=MIN(cellx1, cellx2); j<MAX(cellx1, cellx2); j++)
			for (k=MIN(celly1, celly2); k<MAX(celly1, celly2); k++)
			{

			}
		 */

		//On ajoute le polygone sur les cellules par lesquelles passent les vertices du pily
		//for (x=x1; x<=x2;
	}
}

void gridAddPolygonByBB(Grid* g, Polygon* p)
{
	BBox B = polyGetBBox(p);
	unsigned int l, t, r, b, i, j, count=0;
	l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
	t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
	for (i=l; i<=r; i++)
		for (j=t; j<=b; j++)
			gridAddPolygonToCell(g, p, i, j), count++;
	//printf("polygon %i added %u times\n", p, count);
	polyUpdateGridPosition(p, l, t, r, b);
}

void gridUpdatePolygonPositionByBB(Grid* g, Polygon* p)
{
	BBox B = polyGetBBox(p);
	unsigned int l, t, r, b, i, j;
	l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
	t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
	if (!p->GridPos.Valid)
	{
		l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
		t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
		for (i=l; i<=r; i++)
			for (j=t; j<=b; j++)
				gridAddPolygonToCell(g, p, i, j);

		polyUpdateGridPosition(p, l, t, r, b);

	}
	else
	{
		int offsetTop, offsetLeft, offsetRight, offsetBottom;
		offsetTop=t-p->GridPos.Top; offsetLeft=l-p->GridPos.Left; offsetRight=r-p->GridPos.Right; offsetBottom=b-p->GridPos.Bottom;
		if (offsetTop==0 && offsetBottom==0 && offsetLeft==0 && offsetRight==0)
		{
			//printf("On a évite un insert!\n");
			return;
		}
		else
		{
			gridRemovePolygon(g, p);
			l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
			t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
			for (i=l; i<=r; i++)
				for (j=t; j<=b; j++)
					gridAddPolygonToCell(g, p, i, j);

			polyUpdateGridPosition(p, l, t, r, b);

		}

	}
}

void gridAddPolygonToCell(Grid* g, Polygon* p, unsigned int x, unsigned int y)
{
	//on ajoute le polygone à la liste que s'il n'est pas déjà dedans
	List* L=gridGetCellList(g, x, y);

	Node* it = lstFirst(L);
	while(!nodeEnd(it))
	{
		if (p==nodeGetData(it))
			return;
		it = nodeGetNext(it);
	}

	lstAdd(L, p);
}


void gridRemovePolygonFromCell(Grid* g, Polygon* p, unsigned int x, unsigned int y)
{
	List* L=gridGetCellList(g, x, y);

	Node* it = lstFirst(L);
	while(!nodeEnd(it))
	{
		if (p==nodeGetData(it))
		{
			lstRem(L, it);
			return;
		}
		it = nodeGetNext(it);
	}
}


List gridGetPolygonList(const Grid* g, Polygon* p)
{
	List L, *LExtract;
	lstInit(&L);
	BBox B = polyGetBBox(p);
	unsigned int l, t, r, b, i, j;
	l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
	t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
	for (i=l; i<=r; i++)
		for (j=t; j<=b; j++)
		{
			LExtract=gridGetCellList(g, i, j);
			Node* it = lstFirst(LExtract);
			while(!nodeEnd(it))
			{
				if ((Polygon*)nodeGetData(it)!=p && !lstHaveElem(&L, nodeGetData(it)))
					lstAdd(&L, nodeGetData(it));
				it = nodeGetNext(it);
			}

		}

	return L;
}

void gridRemovePolygon(Grid* g, Polygon* p)
{
	BBox B = polyGetBBox(p);
	unsigned int l, t, r, b, i, j;
	l = MAX(0, B.Left/g->CellWidth); r = MIN(g->HCells-1, B.Right/g->CellWidth);
	t = MAX(0, B.Top/g->CellHeight); b = MIN(g->VCells-1, B.Bottom/g->CellHeight);
	for (i=l; i<=r; i++)
		for (j=t; j<=b; j++)
		{
			lstDel(&g->Table[i][j], p);
		}
}

void gridRemovePolygonByForce(Grid* g, Polygon* p)
{
	unsigned int i, j;
	for (i=0; i<g->HCells; i++)
		for (j=0; j<g->VCells; j++)
		{
			List* L = gridGetCellList(g, i, j);
			Node* it = lstFirst(L);

			while(!nodeEnd(it))
			{
				if (nodeGetData(it)==p)
					lstRem(L, it);
				it=nodeGetNext(it);
			}
		}
}

void gridRemovePolygons(Grid* g)
{
	unsigned int i, j, countr=0, count=0; List* L;
	for (i=0; i<g->HCells; i++)
		for (j=0; j<g->VCells; j++)
		{
			L=gridGetCellList(g, i, j);
			Node* it = lstFirst(L);
			while(!nodeEnd(it))
			{
				if (!polyIsFixed((Polygon*)nodeGetData(it)))
					lstRem(L, it), countr++;
				else
					count++;
				it = nodeGetNext(it);
			}
		}
	//printf("%u removed, %u not removed\n", countr, count);
}

void gridDraw(const Grid* g)
{
	unsigned int i, j;
	glColor4f(1.f, 1.f, .1f, 0.2f);
	glBegin(GL_LINES);
	//for (i=0; i<g->VCells; i++)
	for (j=0; j<g->HCells; j++)
	{
		glVertex2f(j*g->CellWidth, 0.f);
		glVertex2f(j*g->CellWidth, g->CellHeight*g->VCells);
	}

	for (i=0; i<g->VCells; i++)
	{
		glVertex2f(0.f, i*g->CellHeight);
		glVertex2f(g->CellWidth*g->HCells, i*g->CellHeight);
	}
	glEnd();
}

void gridRegressionTest(void)
{
	printf("\n === Debut du test de regression de Grid === \n\n");
	Grid g; List* L; Polygon* P; Vertex* V1, *V2;
	unsigned int i, j;

	V1=newVertex(); V2=newVertex();
	vxSetPosition(V1, vec2(0.f, 0.f));
	vxSetPosition(V2, vec2(32.f, 96.f));
	P=newPolygon(2, V1, V2);
	gridInit(&g, 36, 20);
	gridSetCellSize(&g, 32.f);

	//gridAddPolygonToCell(&g, P, 0, 1); gridAddPolygonToCell(&g, P, 0, 1); //On essaye de le rajouter encore une fois, mais ça marchera pas car il y est déjà

	gridAddPolygonByBB(&g, P);

	for (i=0; i<g.HCells; i++)
		for (j=0; j<g.VCells; j++)
			L=gridGetCellList(&g, i, j),
			printf("List size at position %u, %u is %u\n", i, j, lstCount(L));

	gridFree(&g);
	delPolygon(P);
	delVertex(V1); delVertex(V2);
	printf("\n === Fin du test de regression de Polygon === \n\n");
}
