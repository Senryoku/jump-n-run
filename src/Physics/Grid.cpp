#include "Grid.h"

#include <stdio.h>
#include <assert.h>

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

List* gridGetCellList(Grid* g, unsigned int x, unsigned int y)
{
	assert(x < g->HCells && y < g->VCells);
	return &(g->Table[x][y]);
}

void gridAddPolygon(Grid* g, Polygon* p)
{
	unsigned int i;
	int x1, y1, x2, y2, x, y; //Les int permetten de savoir si ça passe en dessous de 0 et de corriger ça
	for (i=0; i<daGetSize(&p->Vertices); i++)
	{
		x1=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i)).x;
		y1=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i)).x;
		
		if (i==daGetSize(&p->Vertices)-1)
		{
			x2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, 0)).x;
			y2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, 0)).x;
		}
		else
		{
			x2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i+1)).x;
			y2=(int)vxGetPosition((Vertex*)daGet(&p->Vertices, i+1)).x;
		}
		
		//On ajoute le polygone sur les cellules par lesquelles passent les vertices du pily
		//for (x=x1; x<=x2; 
	}
}

void gridAddPolygonToCell(Grid* g, Polygon* p, unsigned int x, unsigned int y)
{
	assert(x < g->HCells && y < g->VCells);
	//on ajoute le polygone à la liste que s'il n'est pas déjà dedans
	List* L=&g->Table[x][y];
	
	Node* it = lstFirst(L);
	while(!nodeEnd(it))
	{
		if (p==nodeGetData(it))
			return;
		it = nodeGetNext(it);
	}
	
	lstAdd(L, p);
}

void gridRegressionTest(void)
{
	Grid g; List* L; Polygon* P;
	unsigned int i, j;
	printf("%u\n", (unsigned int)-2);
	
	printf("Initialisation de la grille\n");
	gridInit(&g, 4, 3);
	gridSetCellSize(&g, 32.f);
	
	gridAddPolygonToCell(&g, P, 0, 1);
	gridAddPolygonToCell(&g, P, 0, 1); //On essaille de le rajouter encore une fois, mais ça marchera pas car il y est déjà
	
	for (i=0; i<g.HCells; i++)
		for (j=0; j<g.VCells; j++)
			L=gridGetCellList(&g, i, j),
			printf("List size at position %u, %u is %u\n", i, j, lstCount(L));
	
	printf("Libération de la grille\n");
	gridFree(&g);
}