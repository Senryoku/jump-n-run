#include "Flag.h"

void flInit(Flag* F, float cellH, float cellW, unsigned int H, unsigned int W, Texture TexFlag, Texture TexPole)
{
	F->cellH = cellH;
	F->cellW = cellW;
	F->W = W;
	F->H = H;
	F->TexFlag = TexFlag;
	F->TexPole = TexPole;

	daInit(&F->Vertices);
	daInit(&F->Rigids);
	daReserve(&F->Vertices, W*H);
	daReserve(&F->Rigids, (W*H*2 - W - H + 1));
	for(unsigned int i = 0; i < H; i++)
	{
		for(unsigned int j = 0; j < W; j++)
		{
			daAdd(&F->Vertices, newVertex());
			vxSetPosition((Vertex*) daGet(&F->Vertices, i*W + j), vec2(j*cellW, i*cellH));
			vxSetMass((Vertex*) daGet(&F->Vertices, i*W + j), 0.01f);
		}
	}
	for(unsigned int i = 0; i < H; i++)
	{
		vxSetFixed((Vertex*) daGet(&F->Vertices, i*W), 1);
	}
	for(unsigned int i = 0; i < H; i++)
	{
		for(unsigned int j = 0; j < W - 1 ; j++)
		{
			daAdd(&F->Rigids, newRigid((Vertex*) daGet(&F->Vertices, i*W + j), (Vertex*) daGet(&F->Vertices, i*W + j + 1), cellW));
		}
	}
	for(unsigned int j = 0; j < W - 1 ; j++)
	{
		for(unsigned int i = 0; i < H - 1; i++)
		{
			daAdd(&F->Rigids, newRigid((Vertex*) daGet(&F->Vertices, i*W + j), (Vertex*) daGet(&F->Vertices, (i + 1)*W + j), cellH));
		}
	}
}

void flFree(Flag* F)
{
	for(unsigned int i = 0; i < daGetSize(&F->Vertices); i++)
	{
		delVertex((Vertex*) daGet(&F->Vertices, i));
	}
	for(unsigned int i = 0; i < daGetSize(&F->Rigids); i++)
	{
		delRigid((Rigid*) daGet(&F->Rigids, i));
	}
	daFree(&F->Vertices);
	daFree(&F->Rigids);
}

void flApplyForce(Flag* F, Vec2 Force, Bool B)
{
	for(unsigned int i = 0; i < daGetSize(&F->Vertices); i++)
	{
		vxApplyForce((Vertex*) daGet(&F->Vertices, i), Force, B);
	}
}

void flResolve(Flag* F, float prevdt, float dt)
{
	for(unsigned int i = 0; i < daGetSize(&F->Vertices); i++)
	{
		vxResolve((Vertex*) daGet(&F->Vertices, i), prevdt, dt);
	}
	for(unsigned int i = 0; i < daGetSize(&F->Rigids); i++)
	{
		for(unsigned int j = 0; j < 4; j++) rdResolve((Rigid*) daGet(&F->Rigids, i));
	}
}
