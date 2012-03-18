#include "Lenght.h"



void lnInit(Lenght* R, Vertex* V1, Vertex* V2, float min, float max)
{
	R->V1 = V1;
	R->V2 = V2;
	R->MinLenght=min;
	R->MaxLenght=max;
}

void lnResolve(Lenght* R)
{
	/* Vecteur V1V2 */
	Vec2 Vect = vec2Sub(vxGetPosition(R->V2), vxGetPosition(R->V1));
	
	/* Précalcul de la distance V1V2 */
	float acLength = vec2Length(Vect), factor;
	
	if (acLength < R->MinLenght)
		factor = (acLength - R->MinLenght);
	else if (acLength > R->MaxLenght)
		factor = (acLength - R->MaxLenght);
	else return;
	
	//printf("Lenght: %f \n", factor);
	
	/* Normalisation du vecteur (pas besoin de vec2Normalized(),
	 on a déjà acLength) */
	if(acLength != 0.f)
		Vect = vec2Div(Vect, acLength);
	else
		Vect = vec2(1.f, 0.f); /* Vecteur quelconque en cas de deux points
								superposés, il ne peut raisonnablement être déduit des poistions
								précédentes, en effet, que faire si les points étaient déjà
								superposés à la frame précédente ?... */
	
	if(vxIsFixe(R->V2))
		vxCorrectPosition(R->V1, vec2Prod(Vect, factor));
	else if(vxIsFixe(R->V1))
		vxCorrectPosition(R->V2, vec2Prod(Vect, -factor));
	else
		vxCorrectPosition(R->V2, vec2Prod(Vect, -factor*0.5f)),
		vxCorrectPosition(R->V1, vec2Prod(Vect, factor*0.5f));
}
