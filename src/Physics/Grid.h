#ifndef _GRID_H
#define _GRID_H

#include <Core/List.h>
#include <Core/Tools.h>
#include "Polygon.h"

/**
 * @defgroup Grid Grid
 *
 * Grille qui permet de filtrer la détection de collisions. Les collisions étant faites avec le SAT il faut faire le plus petit nombre de collisions possibles. On fixe la taille de la case de la grille et les polygones sont ajoutés par world dans la grille. Celui-ci récupere la liste des polygones qui sont dans les cases d'un autre polygone pour pouvoir effectuer les collisions.
 * @{
 */

/** @brief Structure donnant une grille de collision
 */

typedef struct {
	List** Table; /**<Tableau 2D de liste (Ce sont les cellules) **/
	unsigned int HCells /**<Nombre de cellules en horizontal (colonnes) **/,
	VCells; /**<Nombre de cellules en vertical (lignes) **/
	float CellWidth /**<Largeur d'une cellule **/,
	CellHeight; /**<Longueur d'une cellule **/

} Grid;

/** @brief Initialise la grille
 *
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] HCells Colonnes
 * @param[in] VCells Lignes
 */
void gridInit(Grid* g, unsigned int HCells, unsigned int VCells);

/** @brief Libère la grille
 *
 * @param[in, out] g grille à laquelle appliquer la fonction
 */
void gridFree(Grid* g);

/** @brief Change la taille de la cellule
 *
 * @param[in, out] g grille à laquelle appliquer la fonction
 * @param[in] Size nouvelle taille de la cellule
 */
void gridSetCellSize(Grid* g, float Size);

/** @brief Donne accès à la liste d'une cellule de la grille
 *
 * @param[in] g grille à laquelle appliquer la fonction
 * @param[in] x position x dans le tableau [0..HCells]
 * @param[in] y position y dans le tableau [0..VCells]
 * @return Un pointeur vers la liste qui est dans la cellule
 */
List* gridGetCellList(const Grid* g, unsigned int x, unsigned int y);

/** @brief Donne accès à la liste contenant tous les polygones qui sont dans des cellule où se trouve un polygone
 *
 * Cette fonction crée une liste, elle l'initialise, elle doit donc être libérée après l'appel à la fonction.
 * @param[in] g grille à laquelle appliquer la fonction
 * @param[in] p polygone duquel on doit récupérer la liste
 * @return Une liste qui contient tous les polygones (sauf p) qui sont sensibles d'être en intersection avec p
 */
List gridGetPolygonList(const Grid* g, Polygon* p);

/** @brief Ajoute un Polygon à la grille
 *
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] p Polygon à ajouter
 */
void gridAddPolygon(Grid* g, Polygon* p);

/** @brief Ajoute un polygone à la grille selon sa BoundingBox
 *
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in,out] p polygone à ajouter
 */
void gridAddPolygonByBB(Grid* g, Polygon* p);

/** @brief Fait une mise à jour de la position du polygone dans la grille selon sa BoundingBox et selon la position antérieure de celui-ci dans la grille
 *
 * La différence avec @see gridAddPolygonByBB est que ici on vérifie si on a besoin de mettre à jour sa position dans la grille et si c'est le cas on l'élimine des cases ou il était avant et on l'ajoute dans les nouvelles cases
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] p polygone à ajouter
 */
void gridUpdatePolygonPositionByBB(Grid* g, Polygon* p);

/** @brief Ajoute un polygone à une cellule de la grille
 *
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] p polygone à ajouter
 * @param[in] x position x dans le tableau [0..HCells]
 * @param[in] y position y dans le tableau [0..VCells]
 */
void gridAddPolygonToCell(Grid* g, Polygon* p, unsigned int x, unsigned int y);

/** @brief Supprime un polygone  d'une cellule de la grille
 *
 * On va rechercher dans la cellule indiquée le polygone et on le supprime.
 * @param[in, out] g grille à laquelle appliquer la fonction
 * @param[in] p polygone à supprimer
 * @param[in] x position x dans le tableau [0..HCells]
 * @param[in] y position y dans le tableau [0..VCells]
 */
void gridRemovePolygonFromCell(Grid* g, Polygon* p, unsigned int x, unsigned int y);

/** @brief Supprime tous les polygones nos fixes de la grille
 *
 * @param[in,out] g grille à laquelle appliquer la fonction
 */
void gridRemovePolygons(Grid* g);

/** @brief Supprime un polygone de la grille selon sa position
 *
 * Cette fonction ne cherche pas le polygone dans la grille! Il determine où le polygone est censé ètre dans la grille selon sa position et après il l'élimine des ces cases. Pour l'éliminer de la grille sans savoir s'il va être bien placer dans la grille selon sa position actuelle (ex: la position du polygone a changé et la grille n'a pas été mise à jour) utiliser la fonction @see gridRemovePolygonByForce
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] p polygone à supprimer
 */
void gridRemovePolygon(Grid* g, Polygon* p);

/** @brief Supprime un polygone de la grille
 *
 * Cette fonction recherche dans toutes les listes de la grille le polygone p et il l'élimine de la grille
 * @param[in,out] g grille à laquelle appliquer la fonction
 * @param[in] p polygone à supprimer
 */
void gridRemovePolygonByForce(Grid* g, Polygon* p);

/** @brief Test e regression de la grille
 */
void gridRegressionTest(void);

/** @brief Dessine la grille en Debug avec OpenGL
 *
 * @param[in] g grille à laquelle appliquer la fonction
 */
void gridDraw(const Grid* g);

/**@}*/

#endif
