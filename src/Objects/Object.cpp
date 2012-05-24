#include "Object.h"

Object* newObject(Polygon* P, unsigned int T, List CT)
{
	Object* Obj = (Object*) malloc(sizeof(Object));
	objInit(Obj, P, T, CT);
	return Obj;
}

Object* cpyObject(Object* O)
{
	List CT;
	lstInit(&CT);
	Node* it = lstFirst(&O->CoordTex);
	while(!nodeEnd(it))
	{
		Vec2* CoordTex = newVec2();
		vec2Cp(CoordTex, *((Vec2*) nodeGetData(it)));
		lstAdd(&CT, CoordTex);
		it = nodeGetNext(it);
	}
	return newObject(cpyPolygon(O->Shape), O->Tex, CT);
}

void objInit(Object* Obj, Polygon* P, unsigned int T, List CT)
{
	Obj->Shape = P;
	Obj->Tex = T;
	Obj->CoordTex = CT;
}

void objFree(Object* Obj)
{
	Node* N;
	while(lstCount(&Obj->CoordTex) > 0)
	{
		N = lstFirst(&Obj->CoordTex);
		free(nodeGetData(N));
		lstRem(&Obj->CoordTex, N);
	}
	lstFree(&Obj->CoordTex); // Peu utile
	Obj->Shape = NULL;
	Obj->Tex = 0;
}

void delObject(Object* Obj)
{
	objFree(Obj);
	free(Obj);
}

Polygon* objGetShape(Object* Obj)
{
	return Obj->Shape;
}
