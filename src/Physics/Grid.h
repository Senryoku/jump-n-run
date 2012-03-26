#ifndef _GRID_H
#define _GRID_H

#include <Core/List.h>
#include <Core/Tools.h>
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

List gridGetPolygonList(Grid* g, Polygon* p);

void gridAddPolygon(Grid* g, Polygon* p);

void gridAddPolygonByBB(Grid* g, Polygon* p);

void gridUpdatePolygonPositionByBB(Grid* g, Polygon* p);

void gridAddPolygonToCell(Grid* g, Polygon* p, unsigned int x, unsigned int y);

void gridRemovePolygonFromCell(Grid* g, Polygon* p, unsigned int x, unsigned int y);

void gridRemovePolygons(Grid* g);

/** @brief Supprime un polygone de la grille selon sa position
 *
 * Cette fonction ne cherche pas le polygone dans la grille! Il determine où le polygone est censé ètre dans la grille selon sa position et après il l'élimine des ces cases. Pour l'éliminer de la grille sans savoir s'il va être bien placer dans la grille selon sa position actuelle (ex: la position du polygone a changé et la grille n'a pas été mise à jour) utiliser la fonction @see gridRemovePolygonByForce 
 * @param g[in, out] grille à laquelle appliquer la fonction
 * @param p polygone à supprimer
 */
void gridRemovePolygon(Grid* g, Polygon* p);

void gridRemovePolygonByForce(Grid* g, Polygon* p);

void gridRegressionTest(void);

void gridDraw(Grid* g);


#endif