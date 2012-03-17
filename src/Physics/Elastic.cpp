#include "Elastic.h"

Elastic* newElastic(Vertex* V1, Vertex* V2, float L)
{
	Elastic* E = (Elastic*) malloc(sizeof(Elastic));
	elasticInit(E, V1, V2, L);
	return E;
}

void elasticInit(Elastic* E, Vertex* V1, Vertex* V2, float L)
{
	E->V1 = V1;
	E->V2 = V2;
	E->Length = L;
}

void delElastic(Elastic* E)
{
	free(E);
}

void elasticResolve(Elastic* E)
{
	/* Vecteur V1V2 */
	Vec2 Vect = vec2Sub(vxGetPosition(E->V2), vxGetPosition(E->V1));

	/* Précalcul de la distance V1V2 */
	float acLength = vec2Length(Vect);

	float factor = E->Spring*(acLength - E->Length);

	/* Rapport entre les masses : Si !=0.5, l'un des points sera moins enclin à bouger */
	float MassFactor = vxGetMass(E->V1)/(vxGetMass(E->V1)+vxGetMass(E->V2));

	/* Normalisation du vecteur (pas besoin de vec2Normalized(),
	 on a déjà acLength) */
	if(acLength != 0.f)
		Vect = vec2Div(Vect, acLength);
	else
		Vect = vec2(1.f, 0.f); /* Voir Rigid.cpp */

	/* Si un point est fixe, le factor passe à 1 pour l'autre */
	vxApplyForce(E->V2, vec2Prod(Vect, -factor*(vxIsFixe(E->V1)?MassFactor*1:1))),
	vxApplyForce(E->V1, vec2Prod(Vect, factor*(vxIsFixe(E->V2)?(1-MassFactor):1)));
}

Vec2 elasticVector(Elastic* E)
{
	return vec2Sub(vxGetPosition(E->V2), vxGetPosition(E->V1));
}

Vertex* elasticGetV1(Elastic* E)
{
	return E->V1;
}

Vertex* elasticGetV2(Elastic* E)
{
	return E->V2;
}
