#include "Node.h"

Node* newNode(Elem Data, Node* Prev, Node* Next)
{
	Node* tmp = (Node*) malloc(sizeof(Node));
	nodeInit(tmp, Data, Prev, Next);
	return tmp;
}

void nodeInit(Node* ptrNode, Elem Data, Node* Prev, Node* Next)
{
	ptrNode->Data = Data;
	ptrNode->Prev = Prev;
	ptrNode->Next = Next;
}

void delNode(Node* ptrNode)
{
	free(ptrNode);
}

void nodeSetPrev(Node* ptrNode, Node* prevNode)
{
	ptrNode->Prev = prevNode;
}

void nodeSetNext(Node* ptrNode, Node* nextNode)
{
	ptrNode->Next = nextNode;
}

Elem nodeGetData(Node* ptrNode)
{
	return ptrNode->Data;
}

Node* nodeGetPrev(Node* ptrNode)
{
	return ptrNode->Prev;
}

Node* nodeGetNext(Node* ptrNode)
{
	return ptrNode->Next;
}

Bool nodeEnd(Node* ptrNode)
{
	return ptrNode == NULL;
}
