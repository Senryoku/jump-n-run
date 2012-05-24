#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdio.h>
#include <stdarg.h>

#include <Core/Tools.h>
#include <Core/DynArr.h>
#include <Core/List.h>
#include "Elastic.h"

/** @defgroup PolygonEl
 *
 * Polygone convexe composé de Vertex reliés par des Elastics
 * Préfixe des méthodes : polyel
 * @todo Test de régression complet
 * @{
**/

/** @brief Structure décrivant un polygon
 *
 * Les liaisons internes ne sont pas construites par défaut
 * par soucis de performance à part dans le cas à un appel à
 * polyelRectangle. Elles doivent être ajoutées manuellement
 * grâce à polyelAddInternal()
**/
typedef struct
{
	DynArr Vertices; /**< Vertices composant le polygon **/
	DynArr Elastics; /**< Limites du polygon, générées automatiquement **/
	DynArr InternalElastics; /**< Contraintes internes **/
	Bool Fixe; /**< Indique que tout les Vertices sont fixes **/
	Vertex* Center; /**< Centre, utile pour les polygones de plus de 4 côtés **/
} PolygonEl;

/** @brief Constructeur
 *
 * Ne créé pas de contraintes internes
 * @param nbVx Nombre de Vertex
 * @param ... Liste de Vertex*
**/
PolygonEl* newPolygonEl(unsigned int nbVx, ...);

/** @brief Constructeur à partir d'une liste
 *
 * Ne créé pas de contraintes internes
 * @param L Liste (List) de Vertex*
**/
PolygonEl* newPolygonElL(List L);

/** @brief Initialise P
 *
 * @param P Le Polygon à initialiser
 * @param nbVx le nombre de Vertex
 * @param ... Liste de Vertex*
**/
void polyelInit(PolygonEl* P, unsigned int nbVx, ...);

/** @brief Destructeur
 *
 * Ne libère pas les Vertices, uniquement les contraintes créées
 * par le constructeur !
**/
void delPolygon(PolygonEl* P);

/** @brief Construit un rectangle, contraintes internes comprises
 *
 * @param V1 Premier Vertex
 * @param V2 Deuxième Vertex
 * @param V3 Troisième Vertex
 * @param V4 Quatrième Vertex
**/
PolygonEl* polyelRectangle(Vertex* V1, Vertex* V2, Vertex* V3, Vertex* V4);

/** @brief Construit un rectangle, contraintes internes comprises à partir d'une liste
 *
 * @param L Liste de Vertices
**/
PolygonEl* polyelRectangleL(List L);

/** @brief Ajoute une contrainte interne au polygone
 *
 * @param P Polygon à modifier
 * @param V1 Position dans la liste de Vertices du polygon du premier Vertex
 * @param V2 Position dans la liste de Vertices du polygon du deuxième Vertex
 * @param Length Longueur de la contrainte, <= 0 pour une longueur automatique
 * @param Spring Contrainte de ressort des contraintes composant le polygonEl
**/
void polyelAddInternal(PolygonEl* P, unsigned int V1, unsigned int V2, float Length, float Spring);

/** @brief Créé un nouveau N-Gone avec des contraintes internes
 *
 * Seul type de polygon à posséder un "centre"
 * @param L List de Vertices
**/
PolygonEl* polyelNGone(List L);

/** @brief Structure décrivant une collision
 *
**/
typedef struct
{
	PolygonEl* P1; /**< Polygon dont on considère une face **/
	PolygonEl* P2; /**< Polygon dont on considère un Vertex **/
	Elastic* Edge; /**< Face impliquée **/
	Vertex* V; /**< Vertex impliqué **/
	Vec2 Normal; /**< Vecteur de collision **/
	float Depth; /**< Profondeur de la collision **/
} CollisionInfoEl;

/** @brief Retourne une struct CollisionInfoEl vide
 *
**/
CollisionInfoEl nullCollisionInfoEl();

/** @brief Test de collision entre deux polygons
 *
 * Utilise l'algorithme SAT (Separating Axis Theorem)
 * @param P1 Premier polygon
 * @param P2 Deuxième polygon
 * @return Structure CollisionInfo contenant les Polygons concernés,
 * le Vertex, la face, un vecteur normal à la face, la profondeur.
**/
CollisionInfoEl polyelCollide(PolygonEl* P1, PolygonEl* P2);

/** @brief Test si un Vextex est à l'intérieur d'un Polygon
 *
 * @return Vrai si le Vertex est dans le Polygon
**/
Bool polyelIsInside(PolygonEl* P, Vertex* V);

/** @brief Projete un Polygon sur un axe
 *
 * Calcule une projection sur un axe (Intervalle 1D) d'un polygon par
 * produit scalaire. L'axe est considéré normalisé.
 * @param P Polygon à projeter
 * @param Min Première borne de l'intervalle résultat
 * @param Max Deuxième borne de l'intervalle résultat
 * @param Axis Axe normalisé sur lequel projeter
**/
void polyelProject(PolygonEl* P, float* Min, float* Max, Vec2 Axis);

/** @brief Calcul le centre (non pondéré) des points de P
 *
 * Isobarycentre (Rien à voir avec P->Center qui lui sert aux contraintes internes)
 * @param P Polygon dont on cherche le centre
**/
Vec2 polyelComputeCenter(PolygonEl* P);

/** @brief Appelle rdResolve pour chaque contrainte du Polygon
 *
 * @param P Polygon
**/
void polyelResolve(PolygonEl* P);

/** @brief Accesseur de Fixe
 *
 * @param P Polygon
**/
Bool polyelIsFixe(PolygonEl* P);

/** @brief Mutateur de Fixe
 *
 * Modifie également l'état de TOUT les vertices de P
 * @param P Polygon à modifier
 * @param B Nouvelle valeur pour Fixe
**/
void polyelSetFixe(PolygonEl* P, Bool B);

void polyelSetSpring(PolygonEl* P, float Spring);
void polyelSetExternalSpring(PolygonEl* P, float Spring);
void polyelSetInternalSpring(PolygonEl* P, float Spring);

/** @brief Accesseur de Center (Contraintes internes)
**/
Vertex* polyelGetCenter(PolygonEl* P);

/** @brief Applique une force à tous les vertices du Polygon
 *
 * @param P Polygon
 * @param Force Force à appliquer
**/
void polyelApplyForce(PolygonEl* P, Vec2 Force);

/** @brief Change la vitesse de tous les points
 *
 * @param P Polygon
 * @param Speed nouvelle vitesse
 **/
void polyelSetSpeed(PolygonEl* P, Vec2 Speed);

/** @brief donne la vitesse du polygone
 *
 * @param P Polygon
 * @return vitesse
 **/
Vec2 polyelGetSpeed(PolygonEl* P);

/** @brief Test de régression
 *
 * Fait des tests...
**/
void polyelRegressionTest();

/** @}
**/

#endif

