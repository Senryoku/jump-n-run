#include "Angular.h"
#include <SFML/OpenGL.hpp>



void angInit(Angular* A, Vertex* C, Vertex* M, Vertex* S, float MinAng, float MaxAng)
{
	A->C = C;
	A->M = M;
	A->S = S;
	A->MinAng = MinAng;
	A->MaxAng = MaxAng;
}

void angResolve(Angular* A)
{
	Vec2 CM = vec2Sub(vxGetPosition(A->M), vxGetPosition(A->C));
	Vec2 CS = vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));
	Vec2 Ortho = vec2Ortho(CM);

	float Angle = acosf(vec2Dot(CM, CS)/(vec2Length(CM)*vec2Length(CS)));

	if(vec2Dot(CS, Ortho) > 0)
	{
		Angle *= -1.f;
	}

	printf("Angle : %f \n Cosinus : %f\n", Angle, vec2Dot(CM, CS)/(vec2Length(CM)*vec2Length(CS)));

	if(Angle > A->MaxAng)
	{
		CS = vec2Rotate(CS, vxGetPosition(A->C), Angle - A->MaxAng);
		vxSetPosition(A->S, vec2Add(CS, vxGetPosition(A->C)));
		printf(" Correction (Sup) : %f\n", Angle - A->MaxAng);
	} else if(Angle < A->MinAng) {
		CS = vec2Rotate(CS, vxGetPosition(A->C), A->MinAng - Angle);
		vxSetPosition(A->S, vec2Add(CS, vxGetPosition(A->C)));
		printf(" Correction (Inf) : %f\n", A->MinAng - Angle);
	}
}
