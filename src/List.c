#include "list.h"

List* newList()
{
	List* L = (List*) malloc(sizeof(List));
	lstInit(L);
	return L;
}

void lstInit(List* L)
{
	L->First = NULL;
	L->Last = NULL;
	L->Count = 0;
}

void delList(List* L)
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

void lstAdd(List* L, Elem Data)
{
	Node* Node = newNode(Data, NULL);
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
	}
}

bool lstEmpty(List* L)
{
	return (L->Count == 0);
}

