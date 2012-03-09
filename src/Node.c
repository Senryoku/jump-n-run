#include "Node.h"

Node* newNode(Elem Data, Node* Next)
{
	Node* tmp = (Node*) malloc(sizeof(Node));
	nodeInit(tmp, Data, Next);
	return tmp;
}

void nodeInit(Node* ptrNode, Elem Data, Node* Next)
{
	ptrNode->Data = Data;
	ptrNode->Next = Next;
}

void delNode(Node* ptrNode)
{
	free(ptrNode);
}

void nodeSetNext(Node* ptrNode, Node* nextNode)
{
	ptrNode->Next = nextNode;
}

Elem nodeGetData(Node* ptrNode)
{
	return ptrNode->Data;
}

Node* nodeGetNext(Node* ptrNode)
{
	return ptrNode->Next;
}

bool nodeEnd(Node* ptrNode)
{
	return nodeGetNext(ptrNode) == NULL;
}
