#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>

#include "Tools.h"

typedef void* Elem;

typedef struct s_Node
{
	Elem Data;
	struct s_Node* Next;
} Node;

Node* newNode(Elem Data, Node* Next);
void nodeInit(Node* ptrNode, Elem Data, Node* Next);

void delNode(Node* ptrNode);

/* Accesseurs */
/** @brief Retourne l'information portée par le noeud
**/
Elem nodeGetData(Node* ptrNode);
/** @brief Retourne le noeud suivant
**/
Node* nodeGetNext(Node* ptrNode);
/** @brief Teste la fin d'une liste
 *
 * @return true si la fin de la liste est atteinte
**/
bool nodeEnd(Node* ptrNode);

/* Mutateurs */
void nodeSetNext(Node* ptrNode, Node* nextNode);


#endif
