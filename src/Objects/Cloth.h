#ifndef _CLOTH_H
#define _CLOTH_H

#include <Physics/Physics.h>

/**
 * @brief Tissu
 *
 *préfixe cl
 */

typedef struct {
	Vertex*** Points;
	unsigned int HCells, VCells;
	float Width, Height;
} Cloth;

Cloth* newCloth(World *W, unsigned int HCells, unsigned int VCells, float Width, float Height);

void delCloth(Cloth* C);

void clInit(Cloth* C, World *W, unsigned int HCells, unsigned int VCells, float Width, float Height);

void clFree(Cloth* C);

void clSetPointsMass(Cloth* C, float Mass);

Vertex* clGetVertex(Cloth* C, unsigned int x, unsigned int y);

#endif