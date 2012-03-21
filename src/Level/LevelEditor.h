#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include "Level.h"

typedef struct
{
	Level* Lvl;
	List tmpLstFixe;
	List tmpLstDyn;

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse; // Mouse devra être mis à jour par Game ?

} LevelEditor;

void lvledInit();

// Fonctions de Manipulation/Vision
void lvledGrab(LevelEditor *Led);
void lvledRelease(LevelEditor *Led);

void lvledGradEl(LevelEditor *Led);
void lvledReleaseEl(LevelEditor *Led);

void lvledToogleNearestFixe(LevelEditor *Led);
void lvledToogleNearestPolyFixe(LevelEditor *Led);

void lvledSetMousePosition(LevelEditor *Led, float, float); // Déplace le Vertex Mouse ! Pas la souris...

// Fonctions d'éditions
void lvledNewPolyInit(LevelEditor *Led);
void lvledNewPolyAddV(LevelEditor *Led, Vertex*);
void lvledNewPolyCreate(LevelEditor *Led);

void lvledNewPolyFixeInit(LevelEditor *Led);
void lvledNewPolyFixeAddV(LevelEditor *Led, Vertex* V);
void lvledNewPolyFixeCreate(LevelEditor *Led);

void lvledDelPoly(LevelEditor *Led, Polygon*);
void lvledDelVertex(LevelEditor *Led, Vertex*); // Fait toute les vérifications avant de supprimer

void lvledTestLevel(LevelEditor *Led); // Lance le jeu sur le niveau en cours d'édition

void lvledLoad(LevelEditor *Led, char**);
void lvledSave(LevelEditor *Led, char**);

#endif
