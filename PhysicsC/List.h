#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

#include "Tools.h"
#include "Node.h"

/* List */

typedef struct
{
	Node* First;
	Node* Last;
	unsigned int Count;
} List;

List* newList();
void lstInit();
void delList(List*);

/** @brief Ajoute un élément en fin (enfile)
**/
void lstAdd(List* L, Elem Data);
/** @brief Supprime l'élément passé en paramètre s'il se trouve dans la liste
 *
 * Compléxité en O(n)
**/
void lstDel(List*, Elem);
/** @brief Renvoi vrai si la file est vide
**/
bool lstEmpty(List*);

typedef Node* Iterator;

#endif
