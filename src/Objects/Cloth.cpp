#include "Cloth.h"

Cloth* newCloth(World *W, ClothType Type, unsigned int HCells, unsigned int VCells, float Width, float Height)
{
	Cloth* C = (Cloth*)malloc(sizeof(Cloth));
	clInit(C, W, Type, HCells, VCells, Width, Height);
	return C;
}

void delCloth(Cloth* C)
{
	clFree(C);
	free(C);
}

void clInit(Cloth* C, World *W, ClothType Type, unsigned int HCells, unsigned int VCells, float Width, float Height)
{
	C->HCells = HCells;
	C->VCells = VCells;
	C->Width = Width;
	C->Height = Height;
	C->Type = Type;
	printf("type of cloth %u\n", Type);
	
	C->Points = (Vertex***) malloc(sizeof(Vertex**)*HCells);
	unsigned int i, j;
	for (i=0; i<HCells; i++)
	{
		C->Points[i] = (Vertex**) malloc(sizeof(Vertex*)*VCells);
		
		for (j=0; j<VCells; j++)
		{
			C->Points[i][j] = newVertex();
			vxSetPosition(C->Points[i][j], vec2(i*Width, j*Height));
			wdAddVertex(W, C->Points[i][j]);

			if (i>0)
			{
				if (C->Type == CLOTH_RIGID)
				{
					Rigid* R;
					R = newRigid(C->Points[i-1][j], C->Points[i][j], -1.f);
					wdAddRigid(W, R);
				}
				else
				{
					Elastic* E;
					E = newElastic(C->Points[i-1][j], C->Points[i][j], -1.f, 0.02f);
					wdAddElastic(W, E);
				}
				
			}
			if (j>0)
			{
				if (C->Type == CLOTH_RIGID)
				{
					Rigid* R;
					R = newRigid(C->Points[i][j-1], C->Points[i][j], -1.f);
					wdAddRigid(W, R);
				}
				else
				{
					Elastic* E;
					E = newElastic(C->Points[i][j-1], C->Points[i][j], -1.f, 0.02f);
					wdAddElastic(W, E);
				}
			}
				
			
		}
	}

		
}


void clFree(Cloth* C)
{
	/*
	unsigned int i, j;
	for (i=0; i<C->HCells; i++)
		for (j=0; j<C->VCells; j++)
			delVertex(C->Points[i][j]);
	 */
}

Vertex* clGetVertex(Cloth* C, unsigned int x, unsigned int y)
{
	return C->Points[x][y];
}

void clSetPointsMass(Cloth* C, float Mass)
{
	unsigned int i, j;
	for (i=0; i<C->HCells; i++)
		for (j=0; j<C->VCells; j++)
			vxSetMass(clGetVertex(C, i, j), Mass);
}