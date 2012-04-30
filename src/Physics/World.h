#ifndef _WORLD_H_
#define _WORLD_H_

#include <Core/List.h>
#include "Vertex.h"
#include "Rigid.h"
#include "Polygon.h"
#include "Elastic.h"
#include "Grid.h"

/** @defgroup World
 *
 * Liste les objets du monde, appelle les méthodes de mise à jour
 * Préfixe des fonctions : wd
 * @{
**/

/** @brief Structure du monde physique
 *
 * Les Rigids des polygons étant considérés comme liés au Polygon
 * ils sont gérés à part, en revanche, leur Vertices doivent être
 * ajoutés manuellement au World pour être mis à jour par les
 * méthodes générales ! En effet un Vertex peut être partagé par
 * plusieurs polygons.
**/
typedef struct
{
	List Vertices; /**< Liste de Vertices **/
	List Elastics; /**< Liste de contraintes élastiques **/
	List Rigids; /**< Liste de contraintes rigides isolées **/
	List Polygons; /**< Liste de Polygones **/

	float Width; /**< Largeur du monde **/
	float Height; /**< Hauteur du monde **/

	float prevdt; /**< Intervalle de temps précédent **/
	float dt; /**< Intervalle de temps courant pour la simulation **/
	Grid CollisionGrid;
} World;

/** @brief Constructeur
 *
 * Appelle wbInit après l'allocation
 * @param W Largeur du monde
 * @param H Hauteur du monde
 * @return World* initialisé
**/
World* newWorld(float Width, float Height);

/** @brief Initialise le monde
 *
 * @param W Largeur du monde
 * @param H Hauteur du monde
**/
void wdInit(World* W, float Width, float Height);

/** @brief Ajoute un Vertex au monde
 *
 * @param W World
 * @param V Vertex
**/
void wdAddVertex(World* W, Vertex* V);

/** @brief Ajoute un Polygon au monde
 *
 * @param W World
 * @param P Polygon
**/
void wdAddPolygon(World* W, Polygon* P);

/** @brief Ajoute un Elastic au monde
 *
 * @param W World
 * @param E Elastic
 **/
void wdAddElastic(World* W, Elastic* E);

/** @brief Ajoute un Rigid au monde
 *
 * @param W World
 * @param R Rigid
 **/
void wdAddRigid(World* W, Rigid* R);

/** @brief Ajoute tout les Vertex d'un polygon à la simulation
 *
 * @param W World
 * @param P Polygon
**/
void wdAddVxFromPoly(World* W, Polygon* P);

/** @brief Ajoute tout les Vertex d'une Liste à la simulation
 *
 * @param W World
 * @param L List de Vertices
**/
void wdAddVxFromList(World* W, List L);

/** @brief Retire le Vertex de la simulation (du monde)
 *
 * Ne libère pas V !
 * @param W World
 * @param V Vertex à retirer
**/
void wdDelVertex(World* W, Vertex* V);

/** @brief Retire le Rigid de la simulation (du monde)
 *
 * Ne libère pas R !
 * @param W World
 * @param R Rigid à retirer
**/
void wdDelRigid(World* W, Rigid* R);

/** @brief Retire l'Elastic de la simulation (du monde)
 *
 * Ne libère pas E !
 * @param W World
 * @param E Elastic à retirer
**/
void wdDelElastic(World* W, Elastic* E);

/** @brief Retire le Polygon de la simulation (du monde)
 *
 * Ne libère pas P !
 * @param W World
 * @param P Polygon à retirer
**/
void wdDelPolygon(World* W, Polygon* P);

/** @brief Applique une force à tout les vextices du monde
 *
 * Ne parcours que la liste Vertices, les vertices faisant partie d'un
 * ensemble plus grand du monde (polygon...) doivent aussi y figurer !
 * @param W Monde à mettre à jour
**/
void wdApplyForce(World* W, Vec2 Force);

/** @brief Appelle Resolve pour tout les Vertices du monde
 *
 * Ne parcours que la liste Vertices, les vertices faisant partie d'un
 * ensemble plus grand du monde (polygon...) doivent aussi y figurer !
 * @param W Monde à mettre à jour
**/
void wdResolveVextex(World* W);

/** @brief Appelle Resolve() pour tout les Rigid du monde
 *
 * Resolve() les contraintes rigides isolées et contenues dans des polygones
 * non fixes.
 * @param W World
**/
void wdResolveRigid(World* W);

/** @brief Appelle Resolve() sur tout les Elastics de W
 *
 * @param W World
**/
void wdResolveElastic(World* W);

/** @brief Recherche et réagit aux collisions
 *
 * Gère les collision Polygon/Polygon
 * @param W World
**/
void wdHandleCollision(World* W);

/** @brief Accesseur de Width
**/
float wdGetWidth(World* W);

/** @brief Accesseur de Height
**/
float wdGetHeight(World* W);

/** @brief Retourne le nombre de Vertex dans le monde
**/
unsigned int wdGetVxCount(World* W);

/** @brief Retourne le nombre de Rigids orphelins dans le monde
**/
unsigned int wdGetElCount(World* W);

/** @brief Retourne le nombre d'Elastics dans le monde
**/
unsigned int wdGetRdCount(World* W);

/** @brief Retourne le nombre de Polygons dans le monde
**/
unsigned int wdGetPolyCount(World* W);

/** @brief Accès à l'iterateur de la liste de Vertex
 *
 * @param W World
 * @return Iterateur sur le premier node de la liste des Vertex
 */
Node* wdGetVertexIt(World* W);

/** @brief Accès à l'iterateur de la liste de Elastic
 *
 * @param W World
 * @return Iterateur sur le premier node de la liste des Elastic
 */
Node* wdGetElasticIt(World* W);

/** @brief Accès à l'iterateur de la liste de Rigid
 *
 * @param W World
 * @return Iterateur sur le premier node de la liste des Rigid
 */
Node* wdGetRigidIt(World* W);

/** @brief Retourne un itérateur sur les Polygons de World
**/
Node* wdGetPolyIt(World* W);

/** @brief Retourne le Vertex le plus proche des coordonnées
 * passées en paramètre.
 *
 * @param W World
 * @param X Coordonnée X
 * @param Y Coordonnée Y
**/
Vertex* wdGetNearest(World* W, float X, float Y);

/** @brief Retourne le Elastic le plus proche des coordonnées
 * passées en paramètre.
 *
 * @param W World
 * @param X Coordonnée X
 * @param Y Coordonnée Y
**/
Elastic* wdGetNearestElastic(World* W, float X, float Y);

/** @brief Retourne le Rigid le plus proche des coordonnées
 * passées en paramètre.
 *
 * @param W World
 * @param X Coordonnée X
 * @param Y Coordonnée Y
**/
Rigid* wdGetNearestRigid(World* W, float X, float Y);

/** @brief Retourne le Polygon le plus proche des coordonnées
 * passées en paramètre.
 *
 * @param W World
 * @param X Coordonnée X
 * @param Y Coordonnée Y
**/
Polygon* wdGetNearestPoly(World* W, float X, float Y);

/** @brief Affichage de Debug de World
 *
 * @param W World
 * @param vxDraw Fonction d'affichage d'un vertex
 * @param elDraw Fonction d'affichage d'un Elastic
 * @param rdDraw Fonction d'affichage d'un Rigid
 * @param wdDraw Fonction d'affichage d'un Polygon
**/
void wdDraw(World* W, void (*vxDraw)(Vertex* V, float R, float G, float B, float A),
	void (*elDraw)(Elastic* E),
	void (*rdDraw)(Rigid* R),
	void (*polyDraw) (Polygon* P));

/** @brief Libère la mémoire occupée
 *
 * Détruit tout les objets contenu en vidant les liste
**/
void wdFree(World *W);

/** @brief Dextructeur
 *
 * Appelle wbFree avant de compléter la libération
**/
void delWorld(World *W);


/** @brief Fait une mise à jour de la grille de collisions
 *
 * @param W World
 * @param Force Force la mise à jour de tous les polygones, inclus les fixes
 **/
void wdUpdateGrid(World *W, Bool Force);

/** @brief Cherche un polygone contenant un vertice donné
 *
 * @param W World
 * @param Vertex Vertex à rechercher
 * @return Pointeur vers le Polygone si trouvé, NULL sinon
 **/
Polygon* wdFindPolygon(World *W, Vertex* V);

/** @}
**/


#endif

