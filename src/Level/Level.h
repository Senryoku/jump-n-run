#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Physics/Physics.h"

typedef struct
{
	World* W;
	Vec2 Spawn;
	Vec2 Goal;
	// Background
	// Layer 1 // Juste derrière les objets
	// Layer 2 // Juste devant les objets
 	// Foreground

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse; // Mouse devra être mis à jour par Game ?

 	// Variables temporaires pour la création/destruction/modification des objets
	Vertex *tmpVertex;
 	Rigid *tmpRigid;
 	Elastic *tmpElastic;
 	Polygon *tmpPoly;
} Level;

void lvledInit();

// Fonctions de Manipulation/Vision
void lvledGrab();
void lvledRelease();

void lvledGradEl();
void lvledReleaseEl();

void lvledToogleNearestFixe();
void lvledToogleNearestPolyFixe();

void lvledSetMousePosition(float, float); // Déplace le Vertex Mouse ! Pas la souris...

// Fonctions d'éditions
void lvledNewPolyInit();
void lvledNewPolyAddV(Vertex*);
void lvledNewPolyCreate();

void lvledNewPolyFixeInit();
void lvledNewPolyFixeAddV(Vertex*);
void lvledNewPolyFixeCreate();

void lvledDelPoly(Polygon*);
void lvledDelVertex(Vertex*); // Fait toute les vérifications avant de supprimer

void lvledTestLevel(); // Lance le jeu sur le niveau en cours d'édition

void lvledLoad(char**);
void lvledSave(char**);


#endif

