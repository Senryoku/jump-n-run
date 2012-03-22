#ifndef _GRID_H
#define _GRID_H

#include <Core/List.h>
#include "Polygon.h"

/**
 * @defgroup Grid
 * 
 * Grille qui permet de filtrer la détection de collisions
 */


typedef struct {
	List** Table;
	unsigned int HCells, VCells;
	float Width, Height, CellWidth, CellHeight;
	
} Grid;

void gridInit(Grid* g, unsigned int HCells, unsigned int VCells);

void gridFree(Grid* g);

void gridSetCellSize(Grid* g, float Size);

List* gridGetCellList(Grid* g, unsigned int x, unsigned int y);

void gridAddPolygon(Grid* g, Polygon* p);

void gridAddPolygonToCell(Grid* g, Polygon* p, unsigned int x, unsigned int y);

void gridRegressionTest(void);


#endif