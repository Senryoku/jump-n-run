#include "Joint.h"
#include <SFML/OpenGL.hpp>



void jnInit(Joint* J, Vertex* C, Vertex* M, Vertex* S)
{
	J->C = C;
	J->M = M;
	J->S = S;
	J->MinAng = DEG2RAD(190);
	J->MaxAng = DEG2RAD(360);
	float L1, L2;
	L1 = vec2Length(vec2Sub(vxGetPosition(J->M), vxGetPosition(J->C)));
	L2 = vec2Length(vec2Sub(vxGetPosition(J->S), vxGetPosition(J->C)));
	J->Length =  L1 + L2;
	J->Factor = L2/L1;
	
	Vec2 CM = vec2Sub(vxGetPosition(J->M), vxGetPosition(J->C));
	Vec2 tmp = vec2Prod(vec2Rotate(CM, vec2(0.f, 0.f), DEG2RAD(-10.f)), J->Factor);
	//CM = vec2Add(CM, vxGetPosition(C));
	//tmp = vec2Sub(tmp, vxGetPosition(C));
	J->MaxDot = vec2Dot(tmp, CM);
	printf("tmp : %f, %f; cm : %f, %f\n", tmp.x, tmp.y, CM.x, CM.y);
	printf("factor: %f; factored : %f, %f\n", J->Factor, CM.x*J->Factor, CM.y*J->Factor);
	
}

void jnResolve(Joint* J)
{
	Vec2 CM = vec2Sub(vxGetPosition(J->M), vxGetPosition(J->C));
	Vec2 CS = vec2Sub(vxGetPosition(J->S), vxGetPosition(J->C));
	Vec2 Ortho = vec2Ortho(CM);
	
	//Vec2 tmp = vec2Prod(vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(170.f)), J->Factor);
	//J->MaxDot = vec2Dot(tmp, CM);

	
	float sc = vec2Dot(CS, Ortho), sc2 = vec2Dot(CS, CM); 
	
	//printf("scalaire: %f\n", vec2Dot(CS, CM));
	//printf("dot: %f", sc2);
	
	if (sc>0.f) //mauvais cote
	{
		//On applique une rigide
		/*Vec2 Vect = vec2Sub(vxGetPosition(J->S), vxGetPosition(J->M));
		 
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
		 
		
		
		if (sc2>0.f) // il va falloir corriger avec une rotation
		{
			//Vec2 pos = vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(0.f));
			Vec2 pos = vxGetPosition(J->M);
			pos = vec2Sub(pos, vxGetPosition(J->C));
			pos = vec2Prod(pos, J->Factor);
			pos = vec2Add(pos, vxGetPosition(J->C));
			vxSetPosition(J->S, pos);
			//Vec2 pos = vec2Prod(vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-10.f)), J->Factor);
			//printf("M pos :%f, %f, M rotated by 10º : %f, %f\n", vxGetPosition(J->M).x, vxGetPosition(J->M).y, pos.x, pos.y);
			
			//printf("pos: %f, %f\n", pos.x, pos.y);
		}
		else
		{
			Vec2 pos = vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(180.f));
			//printf("pos: %f, %f; Factor: %f", pos.x, pos.y, J->Factor);
			pos = vec2Sub(pos, vxGetPosition(J->C));
			pos = vec2Prod(pos, J->Factor);
			pos = vec2Add(pos, vxGetPosition(J->C));
			
			vxSetPosition(J->S, pos);
			
			
			 
		}
		 
	}
	//pour simplifier et pour avoir un meilleur fonctionnement on ne tiens pas compte des 10º juste 0º-180º
	/*else if (0)//sc2 > J->MaxDot) // il va falloir corriger avec une rotation
	{
		//printf("dot: %f\n", sc2);
		
		Vec2 pos = vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-10.f));
		//printf("pos: %f, %f; Factor: %f", pos.x, pos.y, J->Factor);
		pos = vec2Sub(pos, vxGetPosition(J->C));
		pos = vec2Prod(pos, J->Factor);
		pos = vec2Add(pos, vxGetPosition(J->C));
		//pos.x*=J->Factor; pos.y*=J->Factor;
		vxSetPosition(J->S, pos);
		//vxSetPosition(J->S, vec2Prod(vec2Rotate(vxGetPosition(J->M), vxGetPosition(J->C), DEG2RAD(-10.f)), 1.f));
	}
	 */
		
	
	
}
