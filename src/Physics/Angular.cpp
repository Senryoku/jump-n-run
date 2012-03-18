//
//  Angular.cpp
//  Jump N Run
//
//  Created by Edu San Martin Morote on 17/03/12.
//  Copyright (c) 2012 Posva Games. All rights reserved.
//

#include "Angular.h"

void angInit(Angular* A, Vertex* C, Vertex* M, Vertex* S, float MinAng, float MaxAng)
{
	A->C=C;
	A->M=M;
	A->S=S;
	A->MinAng=MinAng;
	A->MaxAng=MaxAng;
	A->CM=vec2Sub(vxGetPosition(A->M), vxGetPosition(A->C));
	A->CS=vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));
	A->Lenghts = vec2Length(A->CM)*vec2Length(A->CS);
}

void angUpdateLenghts(Angular* A)
{
	A->CM=vec2Sub(vxGetPosition(A->M), vxGetPosition(A->C));
	A->CS=vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));
	A->Lenghts = vec2Length(A->CM)*vec2Length(A->CS);
}

void angResolve(Angular* A)
{
	//On calcule l'angle
	A->CM=vec2Sub(vxGetPosition(A->M), vxGetPosition(A->C));
	A->CS=vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));
	float angle = acosf(vec2Dot(A->CM, A->CS)/A->Lenghts), correctionAgnle;
	printf("angle : %f\n", angle*(180.f/M_PI) );
	
	if (angle > A->MaxAng)
	{
		correctionAgnle=angle-A->MaxAng;
		//On fait une rotations sur le vertex esclave
		vxCorrectPosition(A->S, vec2Rotate(vxGetPosition(A->S), correctionAgnle));
	}
	/*
	if (angle < A->MinAng)
	{
		correctionAgnle=angle+A->MinAng;
		//On fait une rotations sur le vertex esclave
		vxSetPosition(A->S, vec2Rotate(vxGetPosition(A->S), correctionAgnle));
	}
	 */
}