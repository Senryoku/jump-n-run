#ifndef _GRID_H
#define _GRID_H

#include <Core/List.h>

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


#endif