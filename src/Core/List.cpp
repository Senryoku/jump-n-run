#include "List.h"

List* newList()
{
	List* L = (List*) malloc(sizeof(List));
	lstInit(L);
	return L;
}

void lstInit(List* L)
{
	L->First = NULL;
	L->Count = 0;
}

void lstFree(List* L)
{
	Node* tmp;
	while(!lstEmpty(L))
	{
		tmp = L->First;
		L->First = nodeGetNext(tmp);
		delNode(tmp);
		L->Count -= 1;
	}
}

void delList(List* L)
{
	lstFree(L);
	free(L);
}

void lstAdd(List* L, Elem Data)
{
	Node* Node = newNode(Data, L->Last, NULL);
	if(L->Count == 0) L->First = Node; else L->Last->Next = Node;
	L->Last = Node;
	L->Count += 1;
}

void lstDel(List* L, Elem E)
{
	Node* prevNode = NULL;
	Node* tmpNode = L->First;
	while(nodeEnd(tmpNode) && nodeGetData(tmpNode) != E)
	{
		tmpNode = nodeGetNext(tmpNode);
		prevNode = nodeGetNext(prevNode);
	}
	if(!nodeEnd(tmpNode))
	{
		if(prevNode == NULL)
		{
			L->First = nodeGetNext(tmpNode);
		} else {
			nodeSetNext(prevNode, nodeGetNext(tmpNode));
		}
		delNode(tmpNode);
		L->Count--;
	}
}

void lstRem(List* L, Node* N)
{
	if(N == L->First)
	{
		if(N == L->Last)
		{
			L->First = NULL;
			L->Last = NULL;
		} else {
			nodeSetPrev(nodeGetNext(N), NULL);
			L->First = nodeGetNext(N);
		}
	} else if(N == L->Last) {
		nodeSetNext(nodeGetPrev(N), NULL);
		L->Last = nodeGetPrev(N);
	} else {
		nodeSetNext(nodeGetPrev(N), nodeGetNext(N));
		nodeSetPrev(nodeGetNext(N), nodeGetPrev(N));
	}
	delNode(N);
	L->Count--;
}

Bool lstEmpty(List* L)
{
	return (L->Count == 0);
}

Node* lstFirst(List* L)
{
	return L->First;
}

Node* lstEnd(List* L)
{
	return L->Last;
}

