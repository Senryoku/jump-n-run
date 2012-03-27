#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include "Level.h"
#include "Game/Game.h"
#include <stdio.h>

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
 * @todo Débuger NewBox
 * @{
**/

#define LVLED_GRID		1
#define LVLED_RULE		2
#define LVLED_LIMITS	4

typedef struct
{
	Level* Lvl;
	List tmpLstFixe;
	List tmpLstDyn;

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse,
		*tmpElastic1, *tmpElastic2, *tmpRigid1, *tmpRigid2,
		*tmpBox1, *tmpBox2, *tmpBox3, *tmpBox4;

	/* Fonctions de Callback */
	void (*lineDraw) (float X1, float Y1, float X2, float Y2, float R, float G, float B, float A);
	void (*vxDraw)(Vertex* V, float R, float G, float B, float A);
	void (*elDraw)(Elastic* E);
	void (*rdDraw)(Rigid* R);
	void (*polyDraw) (Polygon* P);
} LevelEditor;

void lvledInit(LevelEditor *Led, float Width, float Height);
void lvledFree(LevelEditor *Led);

/* Mutateurs */
/** @brief Défini la fonction d'affichage d'une ligne (Aides à l'édition)
**/
void lvledSetLineDraw(LevelEditor* Led, void (*lineDraw) (float X1, float Y1, float X2, float Y2, float R, float G, float B, float A));
/** @brief Défini la fonction d'affichage des Vertex
**/
void lvledSetVxDraw(LevelEditor* Led, void (*vxDraw)(Vertex* V, float R, float G, float B, float A));
/** @brief Défini la fonction d'affichage des Elastic
**/
void lvledSetElDraw(LevelEditor* Led, void (*elDraw)(Elastic* E));
/** @brief Défini la fonction d'affichage des Rigid
**/
void lvledSetRdDraw(LevelEditor* Led, void (*rdDraw)(Rigid* R));
/** @brief Défini la fonction d'affichage des Polygon
**/
void lvledSetPolyDraw(LevelEditor* Led, void (*polyDraw)(Polygon* P));

/** @brief Affichage du niveau en cours d'édition
 *
 * @param Led LevelEditor
 * @param flag Flags possibles : LVLED_RULE, LVLED_LIMITS
**/
void lvledDraw(const LevelEditor *Led, char flag);

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

void lvledNewBoxInit(LevelEditor *Led);
void lvledNewBoxUpdate(LevelEditor *Led);
void lvledNewBoxCreate(LevelEditor *Led);

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

/** @brief Lance le jeu sur le niveau en cours d'édition
 *
 * @param Led LevelEditor
**/
void lvledTestLevel(LevelEditor *Led);

/** @brief Charge un fichier dans l'éditeur
 *
 * @param Led LevelEditor
 * @param File nom du fichier
 * @return Vrai si chargement est bon
 **/
Bool lvledLoad(LevelEditor *Led, const char* File);

/** @brief Sauvegarde le niveau de l'éditeur dans un fichier
 *
 * @param Led LevelEditor
 * @param File nom du fichier
 * @return Vrai si la sauvegarde est bonne
 **/
Bool lvledSave(LevelEditor *Led, const char* File);

/** @}
**/

#endif
