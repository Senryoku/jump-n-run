#include "Rigid.h"

Rigid* newRigid(Vertex* V1, Vertex* V2, float L)
{
	Rigid* R = (Rigid*) malloc(sizeof(Rigid));
	rdInit(R, V1, V2, L);
	return R;
}

void rdInit(Rigid* R, Vertex* V1, Vertex* V2, float L)
{
	R->V1 = V1;
	R->V2 = V2;
	/* Si la longueur n'est pas valide, on la calcule */
	if(L <= 0.f)
		L = vec2Length(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
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

Vec2 rdVector(Rigid* R)
{
	return vec2Sub(vxGetPosition(R->V2), vxGetPosition(R->V1));
}

Vertex* rdGetV1(Rigid* R)
{
	return R->V1;
}

Vertex* rdGetV2(Rigid* R)
{
	return R->V2;
}

void rdSetLength(Rigid* R, float newLength)
{
	R->Length = newLength;
}

float rdGetLength(const Rigid* R)
{
	return R->Length;
}

void rdRegressionTest()
{
	unsigned int i;
	float Diff = 0;
	Vertex* V1 = newVertex();
	Vertex* V2 = newVertex();
	Rigid* Rd = newRigid(V1, V2, 50.f);

	printf("========= rdRegressionTest Begin ==============\n");
	for(i = 0; i < 10000; i++)
	{
		vxApplyForce(V1, vec2(1.6487f*(i % 29), 2.6875f), 0);
		vxApplyForce(V2, vec2(5.4575f*(i % 89), -8.575678f), 0);
		vxResolve(V1, 0.1f, 0.1f);
		vxResolve(V2, 0.1f, 0.1f);
		rdResolve(Rd);
		Diff = MAX(Diff, fabs(vec2Length(vec2Sub(vxGetPosition(V1), vxGetPosition(V2))) - rdGetLength(Rd)));
		// printf("Difference a l'equilibre : %f\n", vec2Length(vec2Sub(vxGetPosition(V1), vxGetPosition(V2))) - rdGetLength(Rd));
	}
	printf("Difference maximale constatee : %f\n", Diff);
	printf("========= rdRegressionTest End ================\n");
}
