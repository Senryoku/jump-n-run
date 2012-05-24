#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdio.h>
#include <stdarg.h>

#include <Core/Tools.h>
#include <Core/DynArr.h>
#include <Core/List.h>
#include "Rigid.h"

/** @defgroup Polygon Polygon
 *
 * Polygone convexe composé de Vertex reliés par des Rigid
 * Préfixe des méthodes : poly
 * @todo Test de régression complet
 * @{
**/

typedef struct
{
	unsigned int Left; /**< x **/
	unsigned int Top; /**< y **/
	unsigned int Right; /**< x+w **/
	unsigned int Bottom; /**< y+h **/
	Bool Valid; /**< détermine si cette position est valide **/
} GridBBox;


/** @brief Structure décrivant un polygon
 *
 * Les liaisons internes ne sont pas construites par défaut
 * par soucis de performance à part dans le cas à un appel à
 * polyRectangle. Elles doivent être ajoutées manuellement
 * grâce à polyAddInternal()
**/
typedef struct
{
	DynArr Vertices; /**< Vertices composant le polygon **/
	DynArr Rigids; /**< Limites du polygon, générées automatiquement **/
	DynArr InternalRigids; /**< Contraintes internes **/
	Bool Fixe; /**< Indique que tout les Vertices sont fixes **/
	Vertex* Center; /**< Centre, utile pour les polygones de plus de 4 côtés **/
	GridBBox GridPos; /**<dernier position du polygone dans la grille depuis la mise à jour **/
	Bool Collided; /**<Détermine si le polygone a déjà collisioné et si on doit dont le fqire collisioner**/
} Polygon;

/** @brief Constructeur
 *
 * Ne créé pas de contraintes internes
 * @param nbVx Nombre de Vertex
 * @param ... Liste de Vertex*
**/
Polygon* newPolygon(unsigned int nbVx, ...);

/** @brief Constructeur à partir d'une liste
 *
 * Ne créé pas de contraintes internes
 * @param L Liste (List) de Vertex*
**/
Polygon* newPolygonL(List L);

/** @brief Constructeur par copie
 *
 * Créé une copie du polygon et de ses vertices en mémoire
 * @param P Polygon ç copier
 * @return Pointeur vers la copie
**/
Polygon* cpyPolygon(Polygon* P);

/** @brief Initialise P
 *
 * @param P Le Polygon à initialiser
 * @param nbVx le nombre de Vertex
 * @param ... Liste de Vertex*
**/
void polyInit(Polygon* P, unsigned int nbVx, ...);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices, uniquement les contraintes créées
 * par le constructeur !
**/
void delPolygon(Polygon* P);

/** @brief Construit un rectangle, contraintes internes comprises
 *
 * @param V1 Premier Vertex
 * @param V2 Deuxième Vertex
 * @param V3 Troisième Vertex
 * @param V4 Quatrième Vertex
**/
Polygon* polyRectangle(const Vertex* V1, const Vertex* V2, const Vertex* V3, const Vertex* V4);

/** @brief Teste la convexité (validité) du polygon
 *
 * @param P Polygon à tester
 * @return Vrai (1) si P est convexe, 0 sinon
**/
Bool polyIsConvexe(const Polygon* P);

void polyUpdateGridPosition(Polygon* P, unsigned int Left, unsigned int Top, unsigned int Right, unsigned int Bottom);

/** @brief Construit un rectangle, contraintes internes comprises à partir d'une liste
 *
 * @param L Liste de Vertices
**/
Polygon* polyRectangleL(List L);

/** @brief Ajoute une contrainte interne au polygone
 *
 * @param P Polygon à modifier
 * @param V1 Position dans la liste de Vertices du polygon du premier Vertex
 * @param V2 Position dans la liste de Vertices du polygon du deuxième Vertex
 * @param Length Longueur de la contrainte, <= 0 pour une longueur automatique
**/
void polyAddInternal(Polygon* P, unsigned int V1, unsigned int V2, float Length);

/** @brief Donne si le polygone a collisionné le step en cours ou pas
 *
 * @param[in] P Polygon à tester
 * @return collided
 */
Bool polyHasCollided(const Polygon* P);

/** @brief Dit si le polygone a collisionné le step en cours ou pas
 *
 * @param[in,out] P Polygon à éditer
 * @param[in] Collided nouvelle valeur
 */
void polySetCollided(Polygon* P, Bool Collided);

/** @brief Créé un nouveau N-Gone avec des contraintes internes
 *
 * Seul type de polygon à posséder un "centre"
 * @param L List de Vertices
**/
Polygon* polyNGone(List L);

/** @brief Structure décrivant une collision
 *
**/
typedef struct
{
	Polygon* P1; /**< Polygon dont on considère une face **/
	Polygon* P2; /**< Polygon dont on considère un Vertex **/
	Rigid* Edge; /**< Face impliquée **/
	Vertex* V; /**< Vertex impliqué **/
	Vec2 Normal; /**< Vecteur de collision **/
	float Depth; /**< Profondeur de la collision **/
} CollisionInfo;

/** @brief Structure décrivant une Bounding Box de position x, y, largeur w et hauteur h
 *
 **/
typedef struct
{
	float Left; /**< x **/
	float Top; /**< y **/
	float Right; /**< x+w **/
	float Bottom; /**< y+h **/
} BBox;

/** @brief Retourne une la bounding box du polygone
 * @return BBox
 **/
BBox polyGetBBox(const Polygon* P);

/** @brief Retourne une struct CollisionInfo vide
 *
**/
CollisionInfo nullCollisionInfo();

/** @brief Test de collision entre deux polygons
 *
 * Utilise l'algorithme SAT (Separating Axis Theorem)
 * @param P1 Premier polygon
 * @param P2 Deuxième polygon
 * @return Structure CollisionInfo contenant les Polygons concernés,
 * le Vertex, la face, un vecteur normal à la face, la profondeur.
**/
CollisionInfo polyCollide(Polygon* P1, Polygon* P2);

/** @brief Réaction à une collision entre deux polygones
 *
 * @param Info CollisionInfo retournée par polyCollide
**/
void polyHandleCollision(CollisionInfo Info);

/** @brief Test si un Vextex est à l'intérieur d'un Polygon
 *
 * @return Vrai si le Vertex est dans le Polygon
**/
Bool polyIsInside(const Polygon* P, const Vertex* V);

/** @brief Projete un Polygon sur un axe
 *
 * Calcule une projection sur un axe (Intervalle 1D) d'un polygon par
 * produit scalaire. L'axe est considéré normalisé.
 * @param P Polygon à projeter
 * @param Min Première borne de l'intervalle résultat
 * @param Max Deuxième borne de l'intervalle résultat
 * @param Axis Axe normalisé sur lequel projeter
**/
void polyProject(const Polygon* P, float* Min, float* Max, Vec2 Axis);

/** @brief Calcul le centre (non pondéré) des points de P
 *
 * Isobarycentre (Rien à voir avec P->Center qui lui sert aux contraintes internes)
 * @param P Polygon dont on cherche le centre
**/
Vec2 polyComputeCenter(const Polygon* P);

/** @brief Appelle rdResolve pour chaque contrainte du Polygon
 *
 * @param P Polygon
**/
void polyResolve(Polygon* P);

/** @brief Accesseur de Fixe
 *
 * @param P Polygon
**/
Bool polyIsFixe(const Polygon* P);

/** @brief Mutateur de Fixe
 *
 * Modifie également l'état de TOUT les vertices de P
 * @param P Polygon à modifier
 * @param B Nouvelle valeur pour Fixe
**/
void polySetFixe(Polygon* P, Bool B);

/** @brief Accesseur de Center (Contraintes internes)
**/
Vertex* polyGetCenter(const Polygon* P);

/** @brief Retourne le nombre de Vertices composants le polygone
**/
unsigned int polyGetVxCount(const Polygon* P);

/** @brief Retourne le Vertex i
**/
Vertex* polyGetVertex(const Polygon* P, unsigned int i);

/** @brief Retourne un pointeur vers le tableau des Vertices du polygon
 *
 * @param P Polygon
**/
const DynArr* polyGetVertexDA(const Polygon* P);

/** @brief Retourne le nombre de Rigids composants le contour du polygone
**/
unsigned int polyGetRdCount(const Polygon* P);

/** @brief Retourne le Rigid i
**/
Rigid* polyGetRigid(const Polygon* P, unsigned int i);

/** @brief Retourne le nombre de Rigids internes composants le contour du polygone
**/
unsigned int polyGetInternalRdCount(const Polygon* P);

/** @brief Retourne le Rigid interne i
**/
Rigid* polyGetInternalRigid(const Polygon* P, unsigned int i);

/** @brief Retourne un pointeur vers le tableau des contraintes internes du polygon
 *
 * @param P Polygon
**/
const DynArr* polyGetInternalRigidDA(const Polygon* P);

/** @brief Applique une force à tous les vertices du Polygon
 *
 * @param P Polygon
 * @param Force Force à appliquer
 * @param B Prise en compte de la Masse
**/
void polyApplyForce(Polygon* P, Vec2 Force, Bool B);

/** @brief Change la vitesse de tous les points
 *
 * @param P Polygon
 * @param Speed nouvelle vitesse
 **/
void polySetSpeed(Polygon* P, Vec2 Speed);

/** @brief donne la vitesse du polygone
 *
 * @param P Polygon
 * @return vitesse
 **/
Vec2 polyGetSpeed(const Polygon* P);

/** @brief Translate un polygon
 *
 * Modifie également l'ancienne position => Ne modifie pas la vitesse
 * @param P Polygon
 * @param V Vecteur de translation
**/
void polyTranslate(Polygon* P, Vec2 V);

/** @brief Translate un polygon
 *
 * Fait gagner de la vitesse !
 * @param P Polygon
 * @param V Vecteur de translation
**/
void polyMove(Polygon* P, Vec2 V);

/** @brief Test de régression
 *
 * Fait des tests...
**/
void polyRegressionTest();

/** @}
**/

#endif
