#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include "Level.h"

/** @defgroup LevelEditor
 *
 * Permet la création/édition de Niveau
 * Préfixe des méthodes : lvled
 * @todo Ajouter des fonctions de Draw basées sur des callback
 * de primitives d'affichage de :
 * - Points basée sur un Vertex* et une couleur
 * - Lignes basée un Rigid* (couleur prédéterminée)
 * - Lignes basée un Elastic* (couleur déterminée par la contrainte)
 * - Polygones basé sur un Polygon*
 * Ces fonctions de Draw pourraient être ajoutées aux modules
 * correspondant, cependant les fonctions de callback seraient stockées
 * dans LevelEditor.
 * @{
**/

typedef struct
{
	Level* Lvl;
	List tmpLstFixe;
	List tmpLstDyn;

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse, // Mouse devra être mis à jour par Game ?
		*tmpElastic1, *tmpElastic2, *tmpRigid1, *tmpRigid2;
	Bool Testing;

	/* Fonctions de Callback */
	void (*vxDraw)(Vertex* V, float R, float G, float B, float A);
	void (*elasticDraw)(Elastic* E);
	void (*rdDraw)(Rigid* R);
	void (*polyDraw) (Polygon* P);
} LevelEditor;

void lvledInit(LevelEditor *Led, float Width, float Height);

// Fonctions de Manipulation/Vision
void lvledGrab(LevelEditor *Led);
void lvledGrabUpdate(LevelEditor *Led);
void lvledRelease(LevelEditor *Led);

void lvledGrabEl(LevelEditor *Led);
void lvledReleaseEl(LevelEditor *Led);

void lvledToogleNearestFixe(LevelEditor *Led);
void lvledToogleNearestPolyFixe(LevelEditor *Led);

void lvledSetMousePosition(LevelEditor *Led, float, float); // Déplace le Vertex Mouse ! Pas la souris...

// Fonctions d'éditions
void lvledNewPolyInit(LevelEditor *Led);
void lvledNewPolyAddV(LevelEditor *Led);
void lvledNewPolyCreate(LevelEditor *Led);

void lvledNewPolyFixeInit(LevelEditor *Led);
void lvledNewPolyFixeAddV(LevelEditor *Led);
void lvledNewPolyFixeCreate(LevelEditor *Led);

void lvledNewElasticAddV(LevelEditor *Led);
void lvledNewElasticCreate(LevelEditor *Led);

void lvledNewRigidAddV(LevelEditor *Led);
void lvledNewRigidCreate(LevelEditor *Led);

/** @brief Supprime le polygone le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
void lvledDelPoly(LevelEditor *Led);

/** @brief Supprime le Elastic le plus proche de Mouse
 *
 * Test s'il ne fait pas partie d'un polygone avant de le supprimer
 * @param Led LevelEditor
**/
void lvledDelElastic(LevelEditor *Led);

/** @brief Supprime le Rigid le plus proche de Mouse
 *
 * Test s'il ne fait pas partie d'un polygone avant de le supprimer
 * @param Led LevelEditor
**/
void lvledDelRigid(LevelEditor *Led);

/** @brief Supprime le vertex le plus proche de Mouse
 *
 * Fait toute les vérifications avant de supprimer
 * @param Led LevelEditor
**/
void lvledDelVertex(LevelEditor *Led);

void lvledTestLevel(LevelEditor *Led); // Lance le jeu sur le niveau en cours d'édition

void lvledLoad(LevelEditor *Led, const char* File);
void lvledSave(LevelEditor *Led, const char* File);

/** @}
**/

#endif
