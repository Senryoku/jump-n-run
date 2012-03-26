#include "Angular.h"
#include <SFML/OpenGL.hpp>

void glDrawCircle(float x, float y, float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i=0; i<=16; i++)
	{
		glVertex2f(radius*cos((2.0*M_PI)*(i/static_cast<double>(16))) + x,
				   radius*sin((2.0*M_PI)*(i/static_cast<double>(16))) + y);
	}
	glEnd();
}

void angInit(Angular* A, Vertex* C, Vertex* M, Vertex* S, float MinAng, float MaxAng)
{
	A->C=C;
	A->M=M;
	A->S=S;
	A->MinAng=MinAng;
	A->MaxAng=MaxAng;
	A->MC=vec2Sub(vxGetPosition(A->C), vxGetPosition(A->M));
	A->MCOrtho=vec2Ortho(A->MC);
	A->CS=vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));

	//On calcule les posistion limites du point
	float Lenghts=vec2Length(A->MC)*vec2Length(A->CS);
	float angle = acosf(vec2Dot(A->MC, A->CS)/Lenghts), correctionAgnle;
	
	Vec2 MaxPos, MinPos;
	
	correctionAgnle=A->MaxAng-angle;
	MaxPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	A->MaxLenght=vec2Length(vec2Sub(MaxPos, vxGetPosition(A->M)));
	
	correctionAgnle=A->MinAng-angle;
	MinPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	A->MinLenght=vec2Length(vec2Sub(MinPos, vxGetPosition(A->M)));
	
	A->MinOrthoDot=vec2Dot(A->MCOrtho, vec2Sub(MinPos, vxGetPosition(A->C)));
	A->MaxOrthoDot=vec2Dot(A->MCOrtho,vec2Sub(MaxPos, vxGetPosition(A->C)));
	
	A->MinDot=vec2Dot(A->MC, vec2Sub(MinPos, vxGetPosition(A->C)));
	A->MaxDot=vec2Dot(A->MC,vec2Sub(MaxPos, vxGetPosition(A->C)));
	
	
	//DEBUG
	A->MinPos=MinPos;
	A->MaxPos=MaxPos;
	
	printf("maxorthodot: %f, minorthodot: %f, mindot: %f, maxdot: %f\n", A->MaxOrthoDot, A->MinOrthoDot, A->MinDot, A->MaxDot);

}

void angResolve(Angular* A)
{
	//On calcule l'angle
	A->MC=vec2Sub(vxGetPosition(A->C), vxGetPosition(A->M));
	A->MCOrtho=vec2Ortho(A->MC);
	A->CS=vec2Sub(vxGetPosition(A->S), vxGetPosition(A->C));
	//float angle = acosf(vec2Dot(A->CM, A->CS)/A->Lenghts), correctionAgnle;
	//printf("angle : %f\n", angle*(180.f/M_PI) );

	//On calcule les posistion limites du point
	float Lenghts=vec2Length(A->MC)*vec2Length(A->CS);
	float angle = acosf(vec2Dot(A->MC, A->CS)/Lenghts), correctionAgnle;
	
	Vec2 MaxPos, MinPos;
	
	correctionAgnle=A->MaxAng-angle;
	MaxPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	//A->MaxLenght=vec2Length(vec2Sub(MaxPos, vxGetPosition(A->M)));
	
	correctionAgnle=A->MinAng-angle;
	MinPos=vec2Rotate(vxGetPosition(A->S), vxGetPosition(A->C), correctionAgnle);
	//A->MinLenght=vec2Length(vec2Sub(MinPos, vxGetPosition(A->M)));

	
	glColor3f(0.f, 1.f, 0.f); //min in green max in blue
	glDrawCircle(MinPos.x, MinPos.y, 5.f);

	glColor3f(0.f, 0.f, 1.f);
	glDrawCircle(MaxPos.x, MaxPos.y, 5.f);

	
	if (vec2Dot(A->MC, A->CS) < A->MinDot && SGN(A->MinOrthoDot)==SGN(vec2Dot(A->MCOrtho, A->CS)))
	{
		printf("vecdot of mc and cs < mindot\n");
		
		/* Vecteur V1V2 */
		Vec2 Vect = vec2Sub(vxGetPosition(A->S), vxGetPosition(A->M));
		
		/* Précalcul de la distance V1V2 */
		float acLength = vec2Length(Vect);
		
		float factor = (acLength - A->MinLenght);
		printf("minlenght:%f, aclenght:%f\n", A->MinLenght, acLength);
		
		/* Normalisation du vecteur (pas besoin de vec2Normalized(),
		 on a déjà acLength) */
		if(acLength != 0.f)
			Vect = vec2Div(Vect, acLength);
		else
			Vect = vec2(1.f, 0.f); /* Vecteur quelconque en cas de deux points
									superposés, il ne peut raisonnablement être déduit des poistions
									précédentes, en effet, que faire si les points étaient déjà
									superposés à la frame précédente ?... */
		
		//vxCorrectPosition(A->S, vec2Prod(Vect, -factor));
		
		if(vxIsFixe(A->S))
			vxCorrectPosition(A->M, vec2Prod(Vect, factor));
		else if(vxIsFixe(A->M))
			vxCorrectPosition(A->S, vec2Prod(Vect, -factor));
		else
			vxCorrectPosition(A->S, vec2Prod(Vect, -factor*0.5f)),
			vxCorrectPosition(A->M, vec2Prod(Vect, factor*0.5f));
		
		return;
	}
	
	if (0)//(vec2Dot(A->MC, A->CS) > A->MaxDot  && SGN(A->MaxOrthoDot)==SGN(vec2Dot(A->MCOrtho, A->CS)))
	{
		printf("vecdot of mc and cs > mindot\n");
		

		Vec2 Vect = vec2Sub(vxGetPosition(A->S), vxGetPosition(A->M));
		

		float acLength = vec2Length(Vect);
		
		float factor = (acLength - A->MaxLenght);
		//printf("minlenght:%f, aclenght:%f\n", A->MinLenght, acLength);
		

		if(acLength != 0.f)
			Vect = vec2Div(Vect, acLength);
		else
			Vect = vec2(1.f, 0.f); 
		
		//vxCorrectPosition(A->S, vec2Prod(Vect, -factor));
		
		if(vxIsFixe(A->S))
			vxCorrectPosition(A->M, vec2Prod(Vect, factor));
		else if(vxIsFixe(A->M))
			vxCorrectPosition(A->S, vec2Prod(Vect, -factor));
		else
			vxCorrectPosition(A->S, vec2Prod(Vect, -factor*0.5f)),
			vxCorrectPosition(A->M, vec2Prod(Vect, factor*0.5f));
		
	}

		
	
	

}
