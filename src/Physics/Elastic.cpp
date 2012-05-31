#include "Elastic.h"

Elastic* newElastic(Vertex* V1, Vertex* V2, float L, float S)
{
	Elastic* E = (Elastic*) malloc(sizeof(Elastic));
	elInit(E, V1, V2, L, S);
	return E;
}

void elInit(Elastic* E, Vertex* V1, Vertex* V2, float L, float S)
{
	E->V1 = V1;
	E->V2 = V2;
	if(L <= 0.f)
		L = vec2Length(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
	E->Length = L;
	E->Spring = S;
}

void delElastic(Elastic* E)
{
	free(E);
}

void elResolve(Elastic* E)
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
	vxApplyForce(E->V2, vec2Prod(Vect, -factor*(vxIsFixed(E->V1)?MassFactor*1:1)), 0),
	vxApplyForce(E->V1, vec2Prod(Vect, factor*(vxIsFixed(E->V2)?(1-MassFactor):1)), 0);
}

Vec2 elVector(const Elastic* E)
{
	return vec2Sub(vxGetPosition(E->V2), vxGetPosition(E->V1));
}

Vertex* elGetV1(const Elastic* E)
{
	return E->V1;
}

Vertex* elGetV2(const Elastic* E)
{
	return E->V2;
}

void elSetV1(Elastic* E, Vertex* V)
{
	E->V1 = V;
}

void elSetV2(Elastic* E, Vertex* V)
{
	E->V2 = V;
}

float elGetLength(const Elastic* E)
{
	return E->Length;
}

void elSetLength(Elastic* E, float newLength)
{
	if (newLength>0.f)
		E->Length = newLength;
	else
		E->Length = vec2Length(vec2Sub(vxGetPosition(E->V1), vxGetPosition(E->V2)));
}

void elSetSpring(Elastic* E, float newSpring)
{
	E->Spring = newSpring;
}

float elGetSpring(const Elastic* E)
{
	return E->Spring;
}
