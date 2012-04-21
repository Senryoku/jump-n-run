#ifndef _CLOTH_H
#define _CLOTH_H

#include <Physics/Physics.h>

/**
 * @brief Tissu
 *
 *préfixe cl
 */

typedef unsigned char ClothType;

#define CLOTH_ELASTIC 0x00
#define CLOTH_RIGID 0x01

typedef struct {
	Vertex*** Points;
	unsigned int HCells, VCells;
	float Width, Height;
	ClothType Type;
} Cloth;

Cloth* newCloth(World *W, ClothType Type, unsigned int HCells, unsigned int VCells, float Width, float Height);

void delCloth(Cloth* C);

void clInit(Cloth* C, World *W, ClothType Type, unsigned int HCells, unsigned int VCells, float Width, float Height);

void clFree(Cloth* C);

void clSetPointsMass(Cloth* C, float Mass);

Vertex* clGetVertex(Cloth* C, unsigned int x, unsigned int y);

#endif