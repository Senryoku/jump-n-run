#include "Vec2.h"

Vec2 vec2(float x, float y)
{
	Vec2 tmp;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

Vec2 vec2Add(Vec2 V1, Vec2 V2)
{
	Vec2 tmp;
	tmp.x = V1.x + V2.x;
	tmp.y = V1.y + V2.y;
	return tmp;
}

Vec2 vec2Sub(Vec2 V1, Vec2 V2)
{
	Vec2 tmp;
	tmp.x = V1.x - V2.x;
	tmp.y = V1.y - V2.y;
	return tmp;
}

Vec2 vec2Prod(Vec2 V, float f)
{
	Vec2 tmp;
	tmp.x = V.x*f;
	tmp.y = V.y*f;
	return tmp;
}

Vec2 vec2Div(Vec2 V, float f)
{
	Vec2 tmp;
	tmp.x = V.x/f;
	tmp.y = V.y/f;
	return tmp;
}

float vec2Dot(Vec2 V1, Vec2 V2)
{
	return (V1.x*V2.x + V1.y*V2.y);
}

float vec2Length(Vec2 V)
{
	return sqrt(V.x*V.x + V.y*V.y);
}

float vec2SqLength(Vec2 V)
{
	return (V.x*V.x + V.y*V.y);
}

Vec2 vec2Ortho(Vec2 V)
{
	return vec2(-1.f*V.y, V.x);
}

Vec2 vec2Normalized(Vec2 V)
{
	return vec2Div(V, vec2Length(V));
}

Bool vec2Equal(Vec2 V1, Vec2 V2)
{
	return (V1.x == V2.x && V1.y == V2.y);
}

void vec2Cp(Vec2* V1, Vec2 V2)
{
	V1->x = V2.x;
	V1->y = V2.y;
}

Vec2 vec2Rotate(Vec2 V, Vec2 Origin, float R)
{
	float cosR=cosf(R), sinR=sinf(R);
	V.x-=Origin.x; V.y-=Origin.y;
	return vec2(cosR*V.x-sinR*V.y + Origin.x, sinR*V.x+cosR*V.y + Origin.y);
}

void vec2RegressionTest(void)
{
	Vec2 V1;
	Vec2 V2;
	V1.x = 10.f;
	V1.y = 10.f;
	V2.x = 5.f;
	V2.y = 7.f;
	printf(" == Test de regression de Vec2 == \n");
	printf("Coordonnees de V1 : %f %f\n", V1.x, V1.y);
	printf("Coordonnees de V2 : %f %f\n", V2.x, V2.y);
	printf("V1 + V2 : %f %f, V1 - V2 : %f %f\n", vec2Add(V1, V2).x, vec2Add(V1, V2).y, vec2Sub(V1, V2).x, vec2Sub(V1, V2).y);
	printf("V1*2 : %f %f, V2/5 : %f %f\n", vec2Prod(V1, 2).x, vec2Prod(V1, 2).y, vec2Div(V2, 5).x, vec2Div(V2, 5).y);
	printf("Longueur de V1 : %f, Longueur de V2 : %f\n", vec2Length(V1), vec2Length(V2));
	printf("Produit scalaire de V1 et V2 : %f\n", vec2Dot(V1, V2));
	printf(" == Fin du test de Vec2 == \n\n");
	
	//quelques test pour les angles
	Vec2 V1ortho=vec2Ortho(V1);
	V2=vec2Rotate(V1, vec2(0.f, 0.f), DEG2RAD(-30.f));
	printf("V1 rotated: %f, %f\n", V2.x,  V2.y);
	V2=vec2Normalized(V2);
	printf("V2 normalized: %f, %f\n", V2.x, V2.y);
	printf("produit scalaire entre V1 et V2 vaut:%f\n", vec2Dot(V1, V2));
	printf("produit scalaire entre V1ortho et V2 vaut:%f\n", vec2Dot(V1ortho, V2));
	
}
