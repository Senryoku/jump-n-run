/* TEMP TRASH

OLD LIST.H AUTRES CONTENEURS


/*
typedef struct
{
	List* lst;
	Node* node;
} Iterator;
*/

/* En fait, autant utiliser des pointeurs sur Node :o

/** @brief Retourne un itérateur sur la liste
**/
/*Iterator* newIterator(List*);
/** @brief Destructeur
**/
/*void delIterator(Iterator* ite);
/** @brief Renvoi l'élément suivant et fait avancer l'iterateur
 *
 * Ne fait rien si on est à la fin de la liste
**/
/*Node* iteNext(Iterator* ite);
/** @brief Test si l'itérateur a atteind la fin de la liste
 *
 * @return true si la fin de la liste est atteinte
**/
/*bool iteEnd(const Iterator *ite);
/** @brief Replace l'itérateur en début de liste
**/
/*void iteReset(Iterator* ite);

*/

/*

Iterator iterator(List* L)
{
	Iterator ite;
	ite.lst = L;
	ite.node = L->First;
	return ite;
}

Node* iteNext(Iterator* ite)
{
	Node* tmp = ite->node->Next;
	if(ite->node->Next != NULL)
		ite->node = ite->node->Next;
	return tmp;
}

bool iteEnd(const Iterator *ite)
{
	return ite->node->Next == NULL;
}

void iteReset(Iterator* ite)
{
	ite->node = ite->lst->First;
}

*/

/* LIFO (Pile) */

typedef struct
{
	Node* First;
	unsigned int Count;
} Pile;

Pile* newPile();
void PileInit();
void delPile(Pile*);

/** @brief Ajoute un élément (empile)
**/
void PileAdd(Pile* P, Elem Data);
/** @brief Renvoi le premier élément (i.e. le dernier ajouté)
**/
Elem PileGet(Pile*);
/** @brief Renvoi le premier élément et l'ôte de la liste
**/
Elem PilePop(Pile*);
/** @brief Supprime le premier élément
**/
void PileDel(Pile*);
/** @brief Renvoi vrai si la pile est vide
**/
bool PileEmpty(Pile*);

/* FIFO */

typedef List FIFO;

FIFO* newFIFO();
void FIFOInit();
void delFIFO(FIFO*);

/** @brief Ajoute un élément en fin (enfile)
**/
void FIFOAdd(FIFO* F, Elem Data);
/** @brief Renvoi le premier élément (i.e. le premier ajouté)
**/
Elem FIFOGet(FIFO*);
/** @brief Renvoi le premier élément et l'ôte de la liste
**/
Elem FIFOPop(FIFO*);
/** @brief Supprime le premier élément
**/
void FIFODel(FIFO*);
/** @brief Renvoi vrai si la file est vide
**/
bool FIFOEmpty(FIFO*);

/* OLD LIST.C

/* Pile */

Pile* newPile()
{
	Pile* P = (Pile*) malloc(sizeof(Pile));
	PileInit(P);
	return P;
}

void PileInit(Pile* P)
{
	P->First = NULL;
	P->Count = 0;
}

void delPile(Pile* P)
{
	while(!PileEmpty(P))
	{
		PileDel(P);
	}
}

void PileAdd(Pile* P, Elem Data)
{
	Node* Node = newNode(Data, P->First);
	P->First = Node;
	P->Count += 1;
}

Elem PileGet(Pile* P)
{
	return P->First->Data;
}

Elem PilePop(Pile* P)
{
	Elem Data = PileGet(P);
	PileDel(P);
	return Data;
}

void PileDel(Pile* P)
{
	Node* tmp = P->First;
	P->First = tmp->Next;
	delNode(tmp);
	P->Count -= 1;
}

bool PileEmpty(Pile* P)
{
	return (P->First == NULL);
}

/* FIFO */

FIFO* newFIFO()
{
	FIFO* F = (FIFO*) malloc(sizeof(FIFO));
	FIFOInit(F);
	return F;
}

void FIFOInit(FIFO* F)
{
	F->First = NULL;
	F->Last = NULL;
	F->Count = 0;
}

void delFIFO(FIFO* F)
{
	while(!FIFOEmpty(F))
	{
		FIFODel(F);
	}
}

void FIFOAdd(FIFO* F, Elem Data)
{
	Node* Node = newNode(Data, NULL);
	if(F->Count == 0) F->First = Node; else F->Last->Next = Node;
	F->Last = Node;
	F->Count += 1;
}

Elem FIFOGet(FIFO* F)
{
	return F->First->Data;
}

Elem FIFOPop(FIFO* F)
{
	Elem Data = FIFOGet(F);
	FIFODel(F);
	return Data;
}

void FIFODel(FIFO* F)
{
	Node* tmp = F->First;
	F->First = tmp->Next;
	delNode(tmp);
	F->Count -= 1;
}

bool FIFOEmpty(FIFO* F)
{
	return (F->Count == 0);
}
