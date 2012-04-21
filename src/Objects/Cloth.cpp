#include "Cloth.h"

Cloth* newCloth(World *W, unsigned int HCells, unsigned int VCells, float Width, float Height)
{
	Cloth* C = (Cloth*)malloc(sizeof(Cloth));
	clInit(C, W, HCells, VCells, Width, Height);
	return C;
}

void delCloth(Cloth* C)
{
	clFree(C);
	free(C);
}

void clInit(Cloth* C, World *W, unsigned int HCells, unsigned int VCells, float Width, float Height)
{
	C->HCells = HCells;
	C->VCells = VCells;
	C->Width = Width;
	C->Height = Height;
	
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

			/*if (i>0)
			{
				Elastic* E;
				E = newElastic(C->Points[i-1][j], C->Points[i][j], -1.f, 0.15f);
				wdAddElastic(W, E);
			}
			if (j>0)
			{
				Elastic* E;
				E = newElastic(C->Points[i][j-1], C->Points[i][j], -1.f, 0.15f);
				wdAddElastic(W, E);
			}
			 */
			if (i>0)
			{
				Rigid* R;
				R = newRigid(C->Points[i-1][j], C->Points[i][j], -1.f);
				wdAddRigid(W, R);
			}
			if (j>0)
			{
				Rigid* R;
				R = newRigid(C->Points[i][j-1], C->Points[i][j], -1.f);
				wdAddRigid(W, R);
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