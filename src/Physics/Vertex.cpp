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

Vertex* cpyVertex(const Vertex* V)
{
	Vertex* newV = newVertex();
	newV->Position = vxGetPosition(V);
	newV->OldPos = vxGetOldPos(V);
	newV->Acceleration = vxGetAcceleration(V);
	newV->Mass = vxGetMass(V);
	newV->Fixed = vxIsFixed(V);
	return newV;
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
	V->Mass = 1.f;
	V->Fixed = 0;
}

/* Accesseurs */
Vec2 vxGetPosition(const Vertex* V) { return V->Position; }
Vec2 vxGetOldPos(const Vertex* V) { return V->OldPos; }
Vec2 vxGetAcceleration(const Vertex* V) { return V->Acceleration; }
float vxGetMass(const Vertex* V) { return V->Mass; }
Bool vxIsFixed(const Vertex* V) { return V->Fixed; }


void vxSetPosition(Vertex* V, Vec2 newPos)
{
	V->Position = newPos;
	V->OldPos = newPos;
}

void vxTranslate(Vertex* V, Vec2 addPos)
{
	Vec2 spd = vec2Sub(V->Position, V->OldPos);
	V->Position = vec2Add(V->Position, addPos);
	V->OldPos = vec2Sub(V->Position, spd);
}

void vxChangePosition(Vertex* V, Vec2 newPos)
{
	Vec2 spd = vec2Sub(V->Position, V->OldPos);
	V->Position = newPos;
	V->OldPos = vec2Sub(newPos, spd);
}

void vxSetX(Vertex* V, float x)
{
	V->Position.x = x;
	V->OldPos.x = x;
}

void vxSetY(Vertex* V, float y)
{
	V->Position.y = y;
	V->OldPos.y = y;
}

void vxCorrectPosition(Vertex* V, Vec2 addPos)
{
	if(V->Fixed) return;
	V->Position = vec2Add(V->Position, addPos);
}

void vxMove(Vertex* V, Vec2 addPos)
{
	V->Position = vec2Add(V->Position, addPos);
}

void vxCorrectSpeed(Vertex* V, Vec2 addSpeed)
{
	V->OldPos = vec2Add(V->OldPos, addSpeed);
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

void vxSetFixed(Vertex* V, Bool B)
{
	V->Fixed = B;
}

void vxApplyForce(Vertex* V, Vec2 addForce, Bool Mass)
{
	if(vxIsFixed(V) || vec2SqLength(addForce) < 0.00001f) return;
	V->Acceleration = vec2Add(V->Acceleration, (Mass)? vec2Prod(addForce, V->Mass) : addForce);
}

void vxResolve(Vertex* V, float prevdt, float dt)
{
	if(vxIsFixed(V)) return;
	Vec2 tmp = V->Position;

	V->Position = vec2Add(V->Position, vec2Add(vec2Prod(vec2Sub(V->Position, V->OldPos), ((dt/prevdt)*0.999f)), vec2Prod(V->Acceleration, dt*dt)));

	V->OldPos = tmp;
	vxSetAcceleration(V, vec2(0.f, 0.f));
}

void vxRegressionTest(void)
{
	Vertex* V = newVertex();
	int i;
	vxSetPosition(V, vec2(25.f, 10.f));
	printf(" == Test de regression de Vertex == \n");
	printf("Coordonnees de V : %f %f \n", vxGetPosition(V).x, vxGetPosition(V).y);
	printf("Test de boucle sur Resolve avec une force de (2.f, 2.f) :\n");
	for(i = 0; i < 100; i++)
	{
		vxApplyForce(V, vec2(2.f, 2.f), 0);
		vxResolve(V, 1.f, 1.f);
		printf(" #%d - x : %f y : %f x-y : %f \n", i, vxGetPosition(V).x, vxGetPosition(V).y, vxGetPosition(V).x-vxGetPosition(V).y);
	}
	delVertex(V);
	printf(" == Fin du test de Vertex == \n\n");
}
