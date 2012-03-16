#include "Vertex.h"

Vertex* newVertex()
{
	Vertex* V = (Vertex*) malloc(sizeof(Vertex));
	if(V == NULL)
	{
		fprintf(stderr, "Erreur d'allocation memoire (newVertex).\n");
		exit(1);
	} else {
		vxInit(V);
		return V;
	}
}

void delVertex(Vertex* V)
{
	free(V);
}

void vxInit(Vertex* V)
{
	Vec2 vec2Null = vec2(0.f, 0.f);
	V->Position = vec2Null;
	V->OldPos = vec2Null;
	V->Acceleration = vec2Null;
	V->Mass = 1;
	V->Fixe = 0;
}

/* Accesseurs */
Vec2 vxGetPosition(const Vertex* V) { return V->Position; }
Vec2 vxGetOldPos(const Vertex* V) { return V->OldPos; }
Vec2 vxGetAcceleration(const Vertex* V) { return V->Acceleration; }
float vxGetMass(const Vertex* V) { return V->Mass; }
Bool vxIsFixe(const Vertex* V) { return V->Fixe; }


void vxSetPosition(Vertex* V, Vec2 newPos)
{
	V->Position = newPos;
	V->OldPos = newPos;
}

void vxCorrectPosition(Vertex* V, Vec2 addPos)
{
	V->Position = vec2Add(V->Position, addPos);
}

void vxSetMass(Vertex* V, float newMass)
{
	if(newMass > 0)
		V->Mass = newMass;
}

void vxSetAcceleration(Vertex* V, Vec2 newAcc)
{
	vec2Cp(&(V->Acceleration), newAcc);
}

void vxSetFixe(Vertex* V, Bool B)
{
	V->Fixe = B;
}

void vxApplyForce(Vertex* V, Vec2 addForce)
{
	if(vxIsFixe(V)) return;
	V->Acceleration = vec2Add(V->Acceleration, addForce);
}

void vxResolve(Vertex* V, float prevdt, float dt)
{
	if(vxIsFixe(V)) return;
	Vec2 tmp = V->Position;
	V->Position = vec2Add(vec2Add(
				V->Position,
				vec2Prod(vec2Sub(V->Position,
								V->OldPos),
							0.99f*(dt/prevdt))),
		vec2Prod(V->Acceleration, dt*dt));
	V->OldPos = tmp;
	vxSetAcceleration(V, vec2(0.f, 0.f));
}

void vxTestRegression(void)
{
	Vertex* V = newVertex();
	int i;
	vxSetPosition(V, vec2(25.f, 10.f));
	printf(" == Test de regression de Vertex == \n");
	printf("Coordonnees de V : %f %f \n", vxGetPosition(V).x, vxGetPosition(V).y);
	printf("Test de boucle sur Resolve avec une force de (2.f, 2.f) :\n");
	for(i = 0; i < 100; i++)
	{
		vxApplyForce(V, vec2(2.f, 2.f));
		vxResolve(V, 1.f, 1.f);
		printf(" #%d - x : %f y : %f x-y : %f \n", i, vxGetPosition(V).x, vxGetPosition(V).y, vxGetPosition(V).x-vxGetPosition(V).y);
	}
	delVertex(V);
	printf(" == Fin du test de Vertex == \n\n");
}
