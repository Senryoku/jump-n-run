#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include "Level.h"
#include <Game/Game.h>
#include <stdio.h>

/** @defgroup LevelEditor LevelEditor
 *
 * Permet la création/édition de Niveau
 * Préfixe des méthodes : lvled
 * @todo Ajouter des fonctions de manipulation internes des objets (changement de
 * spring pour les elastics, ajout de contraintes internes pour les polygones... etc)
 * @todo Documentation :]
 * @{
**/

#define LVLED_RULE		1
#define LVLED_LIMITS	2

typedef struct
{
	Level* Lvl; ///< Niveau à éditer
	List tmpLstFixedFromV;
	List tmpLstDynFromV;
	List tmpLstFixed;
	List tmpLstDyn;

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse,
		*tmpElastic1, *tmpElastic2, *tmpRigid1, *tmpRigid2,
		*tmpBox1, *tmpBox2, *tmpBox3, *tmpBox4;
	char backPath[255], layer1Path[255], layer2Path[255], forePath[255];
	DynArr TexturesPath;

	Object* objClipboard;

	/* Fonctions de Callback */
	void (*lineDraw) (float X1, float Y1, float X2, float Y2, float R, float G, float B, float A); ///< Fontcion d'affichage de line
	void (*vxDraw)(Vertex* V, float R, float G, float B, float A); ///< Fontcion d'affichage de Vertex
	void (*elDraw)(Elastic* E); ///< Fontcion d'affichage d'Elastic
	void (*rdDraw)(Rigid* R); ///< Fontcion d'affichage de Rigid
	void (*polyDraw) (Polygon* P); ///< Fontcion d'affichage de Polygon
	SharedResources* SR; ///< Accès aux resources partagées

	Polygon* NearestPolygon;
} LevelEditor;

void lvledInit(LevelEditor *Led, float Width, float Height, SharedResources* SR);
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

/** @brief Définit le point de Spawn
**/
void lvledSetSpawn(LevelEditor* Led);

/** @brief Définit le Goal
**/
void lvledSetGoal(LevelEditor* Led);

// Fonctions de Manipulation/Vision
/** @brief Sélectionne le Vertex le plus proche de Mouse comme Grab
**/
void lvledGrab(LevelEditor *Led);
/** @brief Replace Grab sur Mouse
**/
void lvledGrabUpdate(LevelEditor *Led);
/**
 * @brief Relâche Grab
 * @param Led
 * @param Paused Si le monde est en pause et on a atrappé un polygone on aurai besoin de modifier la position de tous ces vertices
**/
void lvledRelease(LevelEditor *Led, Bool Paused);

/** @brief Attrape le Vertex le plus proche de Mouse avec un Elastic
**/
void lvledGrabEl(LevelEditor *Led);
/** @brief Relâche le Vertex lié par un Elastic à Mouse
**/
void lvledReleaseEl(LevelEditor *Led);

/** @brief Fixed/Libère le Vertex le plus proche de Mouse
**/
void lvledToogleNearestFixed(LevelEditor *Led);
/** @brief Fixed/Libère le Polygon le plus proche de Mouse
**/
void lvledToogleNearestPolyFixed(LevelEditor *Led);

/** @brief Replace le Vertex représentant la souris sur les coordonnées passées en paramètre
**/
void lvledSetMousePosition(LevelEditor *Led, float, float);

// Fonctions d'éditions

/** @brief Modifie la taille du world
**/
void lvledSetSize(LevelEditor* Led, float Width, float Height);

/** @brief Créé un nouveau Vertex isolé aux coordonnées de Mouse
**/
void lvledNewVertex(LevelEditor* Led);

/** @brief Prépare la création d'un polygon Dynamique à partir de Vertex préexistants
**/
void lvledPolyFromVertexInit(LevelEditor* Led);
/** @brief Ajoute un Vertex à la liste permettant la création d'un polygon Dynamique à partir de Vertex préexistants
**/
void lvledPolyFromVertexAdd(LevelEditor* Led);
/** @brief Créé un polygon Dynamique partir de Vertex préexistants
**/
void lvledPolyFromVertexCreate(LevelEditor* Led);

/** @brief Prépare la création d'un polygon Fixed à partir de Vertex préexistants
**/
void lvledPolyFixedFromVertexInit(LevelEditor* Led);
/** @brief Ajoute un Vertex à la liste permettant la création d'un polygon Fixed à partir de Vertex préexistants
**/
void lvledPolyFixedFromVertexAdd(LevelEditor* Led);
/** @brief Créé un polygon fixe à partir de Vertex préexistants
**/
void lvledPolyFixedFromVertexCreate(LevelEditor* Led);

/** @brief Prépare un nouveau Polygon quelconque dynamique
**/
void lvledNewPolyInit(LevelEditor *Led);
/** @brief Ajoute un Vertex au polygon quelconque dynamique en cours de création
**/
void lvledNewPolyAddV(LevelEditor *Led);
/** @brief Finalise le Polygon quelconque dynamique en cours de création et l'ajoute au monde
**/
void lvledNewPolyCreate(LevelEditor *Led);

/** @brief Initialise un nouveau Polygon quelconque fixe
**/
void lvledNewPolyFixedInit(LevelEditor *Led);
/** @brief Ajoute un nouveau Vertex au polygon quelconque fixe en cours de création
**/
void lvledNewPolyFixedAddV(LevelEditor *Led);
/** @brief Finalise le polygon quelconque fixe en cours de création et l'ajoute au monde
**/
void lvledNewPolyFixedCreate(LevelEditor *Led);

/** @brief Sélectionne un Vertex afin de le lier à un autre via un Elastic
**/
void lvledNewElasticAddV(LevelEditor *Led);
/** @brief Lie les Vertex sélectionnés à l'aide de lvledNewElasticAddV par un Elastic
**/
void lvledNewElasticCreate(LevelEditor *Led);

/** @brief Sélectionne un Vertex afin de le lier à un autre via un Rigid
**/
void lvledNewRigidAddV(LevelEditor *Led);
/** @brief Lie les Vertex sélectionnés à l'aide de lvledNewRigidAddV par un Elastic
**/
void lvledNewRigidCreate(LevelEditor *Led);

/** @brief Initialise une nouvelle boite
**/
void lvledNewBoxInit(LevelEditor *Led);
/** @brief Ajuste la taille de la boite en cours de création en fonction de la position de Mouse
**/
void lvledNewBoxUpdate(LevelEditor *Led);
/** @brief Finalise la création de la boite
**/
void lvledNewBoxCreate(LevelEditor *Led);

/** @brief Créé une boîte carrée
**/
void lvledCreateBox(LevelEditor* Led, float boxSize);

/** @brief Crée ou édite un objet
**/
void lvledObject(LevelEditor *Led, Polygon* P, unsigned int T, List CT);

/** @brief Supprime le polygone le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
void lvledDelPoly(LevelEditor *Led);

/** @brief Supprime le polygone le plus proche de Mouse ainsi que les Vertices rendus orphelins
 *
 * @param Led LevelEditor
**/
void lvledDelPolyAndVertex(LevelEditor *Led);

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

/** @brief Supprime le vertex passé en paramètre
 *
 * Fait toute les vérifications avant de supprimer
 * @param Led LevelEditor
 * @param tmpVertex Vertex à supprimer
**/
void lvledDelVertexPointer(LevelEditor *Led, Vertex* tmpVertex);

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

/** @brief Créé une copie en mémoire de l'objet le plus proche de la souris
 *
 * Coller à l'aide de ledPasteObject
 * @param Led LevelEditor
**/
void lvledCopyObject(LevelEditor* Led);

/** @brief Ajoute au niveau une copie de l'object sauvegardé grâce à ledCopyObject()
 *
 * @param Led LevelEditor
**/
void lvledPasteObject(LevelEditor* Led);

/** @brief Met à jour le champ Nearest de LevelEditor
 *
 * @param Led LevelEditor
**/
void lvledUpdateNearestPoly(LevelEditor* Led);

/** @brief Met à NULL le champ Nearest de LevelEditor
 *
 * @param Led LevelEditor
 **/
void lvledResetNearestPoly(LevelEditor* Led);

/** @brief Retourne le polygon le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
Polygon* lvledGetNearestPoly(LevelEditor* Led);

/** @brief Retourne le Vertex le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
Vertex* lvledGetNearest(LevelEditor* Led);

/** @brief Retourne le rigid le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
Rigid* lvledGetNearestRigid(LevelEditor* Led);

/** @brief Retourne l'elastic le plus proche de Mouse
 *
 * @param Led LevelEditor
**/
Elastic* lvledGetNearestElastic(LevelEditor* Led);

/** @brief Accesseur de Lvl
**/
Level* lvledGetLvl(const LevelEditor* Led);

/** @brief Ajoute une texture au niveau
 *
 * La texture es ajouté au niveau aussi (elle est donc chargée en mémoire)
 */
void lvledAddTexture(LevelEditor* Led, const char* Path);

/** @}
**/

#endif
