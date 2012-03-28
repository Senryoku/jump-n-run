#include "Joint.h"
#include <SFML/OpenGL.hpp>



void jnInit(Joint* J, Vertex* C, Vertex* M, Vertex* S)
{
	J->C = C;
	J->M = M;
	J->S = S;
	J->MinAng = DEG2RAD(190);
	J->MaxAng = DEG2RAD(360);
	J->Length = vec2Length(vec2Sub(vxGetPosition(J->M), vxGetPosition(J->C))) + vec2Length(vec2Sub(vxGetPosition(J->S), vxGetPosition(J->C)));
}

void jnResolve(Joint* J)
{
	Vec2 CM = vec2Sub(vxGetPosition(J->M), vxGetPosition(J->C));
	Vec2 CS = vec2Sub(vxGetPosition(J->S), vxGetPosition(J->C));
	Vec2 Ortho = vec2Ortho(CM);
	
	float sc = vec2Dot(CS, Ortho), sc2 = vec2Dot(CS, CM); 
	
	printf("scalaire: %f\n", vec2Dot(CS, CM));
	
	if (sc>0.f) //mauvais cote
	{
		if (sc2>0.f) // il va falloir corriger avec une rotation
		{
			vxSetPosition(J->S, vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-10.f)));
		}
		else
		{
			vxSetPosition(J->S, vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-180.f)));
			/*
			Vec2 Vect = vec2Sub(vxGetPosition(J->S), vxGetPosition(J->M));
			
			float acLength = vec2Length(Vect);
			
			float factor = (acLength - J->Length);
			
			if(acLength != 0.f)
				Vect = vec2Div(Vect, acLength);
			else
				Vect = vec2(1.f, 0.f); 
			
			if(vxIsFixe(J->M))
				vxCorrectPosition(J->S, vec2Prod(Vect, factor));
			else if(vxIsFixe(J->S))
				vxCorrectPosition(J->M, vec2Prod(Vect, -factor));
			else
				vxCorrectPosition(J->M, vec2Prod(Vect, -factor*0.5f)),
				vxCorrectPosition(J->S, vec2Prod(Vect, factor*0.5f));
			 */
		}
	}
	else if (sc2>0.f) // il va falloir corriger avec une rotation
	{
		//vxSetPosition(J->S, vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-10.f)));
	}
		
	
	
}
