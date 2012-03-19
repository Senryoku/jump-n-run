#include "Angular.h"
#include <SFML/OpenGL.hpp>

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
	
	//On calcule les posistion limites du point
	float angle = acosf(vec2Dot(A->CM, A->CS)/A->Lenghts), correctionAgnle;
	
	correctionAgnle=angle-A->MaxAng;
	A->MaxPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	correctionAgnle=angle-A->MinAng;
	A->MinPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	
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
	//printf("angle : %f\n", angle*(180.f/M_PI) );
	
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(vxGetPosition(A->S).x, vxGetPosition(A->S).y);
	for (int i=0; i<=16; i++)
	{
		glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(A->S).x,
				   1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(A->S).y);
	}
	glEnd();
	
	if (angle > A->MaxAng)
	{
		
		correctionAgnle=angle-A->MaxAng;
		A->MaxPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
		printf("angle > max\n ; currentpos: %f, %f ; maxpos: %f, %f\n", vxGetPosition(A->S).x, vxGetPosition(A->S).y, A->MaxPos.x, A->MaxPos.y);
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(A->MaxPos.x, A->MaxPos.y);
		for (int i=0; i<=16; i++)
		{
			glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + A->MaxPos.x,
					   1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + A->MaxPos.y);
		}
		glEnd();
		
		
		//On fait une rotations sur le vertex esclave
		//vxCorrectPosition(A->S, vec2Rotate(vxGetPosition(A->S), correctionAgnle));
	}
	
	if (angle < A->MinAng)
	{
		//printf("angle < min\n");
		correctionAgnle=angle-A->MinAng;
		A->MinPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
		
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(A->MinPos.x, A->MinPos.y);
		for (int i=0; i<=16; i++)
		{
			glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + A->MinPos.x,
					   1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + A->MinPos.y);
		}
		
		glEnd();
		//On fait une rotations sur le vertex esclave
		//vxSetPosition(A->S, vec2Rotate(vxGetPosition(A->S), correctionAgnle));
	}
	 
}