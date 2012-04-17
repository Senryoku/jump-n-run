#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include "Level.h"
#include <Game/Game.h>
#include <stdio.h>

/** @defgroup LevelEditor
 *
 * Permet la création/édition de Niveau
 * Préfixe des méthodes : lvled
 * @todo Ajouter des fonctions pour ajouter des Textures (chemins ?)
 * @todo Ajouter des fonctions de manipulation internes des objets (changement de
 * spring pour les elastics, ajout de contraintes internes pour les polygones... etc)
 * @todo Documentation :]
 * @{
**/

#define LVLED_RULE		1
#define LVLED_LIMITS	2

typedef struct
{
	Level* Lvl;
	List tmpLstFixeFromV;
	List tmpLstDynFromV;
	List tmpLstFixe;
	List tmpLstDyn;

	// Servent à la manipulation des objets
 	Elastic* GrabElastic;
 	Vertex *Grab, *GrabEl, *Mouse,
		*tmpElastic1, *tmpElastic2, *tmpRigid1, *tmpRigid2,
		*tmpBox1, *tmpBox2, *tmpBox3, *tmpBox4;
	char backPath[255], layer1Path[255], layer2Path[255], forePath[255];
	DynArr TexturesPath;

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
/** @brief Sélectionne le Vertex le plus proche de Mouse comme Grab
**/
void lvledGrab(LevelEditor *Led);
/** @brief Replace Grab sur Mouse
**/
void lvledGrabUpdate(LevelEditor *Led);
/** @brief Relâche Grab
**/
void lvledRelease(LevelEditor *Led);

/** @brief Attrape le Vertex le plus proche de Mouse avec un Elastic
**/
void lvledGrabEl(LevelEditor *Led);
/** @brief Relâche le Vertex lié par un Elastic à Mouse
**/
void lvledReleaseEl(LevelEditor *Led);

/** @brief Fixe/Libère le Vertex le plus proche de Mouse
**/
void lvledToogleNearestFixe(LevelEditor *Led);
/** @brief Fixe/Libère le Polygon le plus proche de Mouse
**/
void lvledToogleNearestPolyFixe(LevelEditor *Led);

/** @brief Replace le Vertex représentant la souris sur les coordonnées passées en paramètre
**/
void lvledSetMousePosition(LevelEditor *Led, float, float);

// Fonctions d'éditions

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

/** @brief Prépare la création d'un polygon Fixe à partir de Vertex préexistants
**/
void lvledPolyFixeFromVertexInit(LevelEditor* Led);
/** @brief Ajoute un Vertex à la liste permettant la création d'un polygon Fixe à partir de Vertex préexistants
**/
void lvledPolyFixeFromVertexAdd(LevelEditor* Led);
/** @brief Créé un polygon fixe à partir de Vertex préexistants
**/
void lvledPolyFixeFromVertexCreate(LevelEditor* Led);

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
void lvledNewPolyFixeInit(LevelEditor *Led);
/** @brief Ajoute un nouveau Vertex au polygon quelconque fixe en cours de création
**/
void lvledNewPolyFixeAddV(LevelEditor *Led);
/** @brief Finalise le polygon quelconque fixe en cours de création et l'ajoute au monde
**/
void lvledNewPolyFixeCreate(LevelEditor *Led);

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

/** @brief Crée ou édite un objet
**/
void lvledObject(LevelEditor *Led, Polygon* P, unsigned int T, List CT);

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
