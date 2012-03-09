#include "Rigid.h"

Rigid* newRigid(const Vertex* V1, const Vertex* V2, float L)
{
	Rigid* R = (Rigid*) malloc(sizeof(Rigid));
	rdInit(R, V1, V2, L);
	return R;
}

void rdInit(Rigid* R, const Vertex* V1, const Vertex* V2, float L)
{
	R->V1 = V1;
	R->V2 = V2;
	R->Length = L;
}

void delRigid(Rigid* R)
{
	free(R);
}

void rdResolve(Rigid* R)
{
	/* Vecteur V1V2 */
	Vec2 Vect = vec2Sub(vxGetPosition(R->V2), vxGetPosition(R->V1));

	/* Précalcul de la distance V1V2 */
	float acLength = vec2Length(Vect);

	float factor = (acLength - R->Length);

	/* Normalisation du vecteur (pas besoin de vec2Normalized(),
								 on a déjà acLength) */
	Vect = vec2Div(Vect, acLength);

	if(vxIsFixe(R->V2))
		vxCorrectPosition(R->V1, vec2Prod(Vect, factor));
	else if(vxIsFixe(R->V1))
		vxCorrectPosition(R->V1, vec2Prod(Vect, -factor));
	else
		vxCorrectPosition(R->V2, vec2Prod(Vect, -factor*0.5f)),
		vxCorrectPosition(R->V1, vec2Prod(Vect, factor*0.5f));
}
