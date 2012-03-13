#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdio.h>
#include <stdarg.h>

#include "Tools.h"
#include "DynArr.h"
#include "Rigid.h"

/** @defgroup Polygon
 *
 * Polygone convexe composé de Vertex reliés par des Rigid
 * Préfixe des méthodes : poly
 * @todo Test de régression complet
 * @{
**/

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
} Polygon;

/** @brief Constructeur
 *
 * Ne créé pas de contraintes internes
 * @param nbVx Nombre de Vertex
 * @param ... Liste de Vertex*
**/
Polygon* newPolygon(unsigned int nbVx, ...);

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
Polygon* polyRectangle(Vertex* V1, Vertex* V2, Vertex* V3, Vertex* V4);

/** @brief Ajoute une contrainte interne au polygone
 *
 * @param P Polygon à modifier
 * @param V1 Position dans la liste de Vertices du polygon du premier Vertex
 * @param V2 Position dans la liste de Vertices du polygon du deuxième Vertex
 * @param Length Longueur de la contrainte, <= 0 pour une longueur automatique
**/
void polyAddInternal(Polygon* P, unsigned int V1, unsigned int V2, float Length);

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

/** @brief Projete un Polygon sur un axe
 *
 * Calcule une projection sur un axe (Intervalle 1D) d'un polygon par
 * produit scalaire. L'axe est considéré normalisé.
 * @param P Polygon à projeter
 * @param Min Première borne de l'intervalle résultat
 * @param Max Deuxième borne de l'intervalle résultat
 * @param Axis Axe normalisé sur lequel projeter
**/
void polyProject(Polygon* P, float* Min, float* Max, Vec2 Axis);

/** @brief Retourne le centre (non pondéré) des points de P
 *
 * Isobarycentre
 * @param P Polygon dont on cherche le centre
**/
Vec2 polyGetCenter(Polygon* P);

/** @brief Appelle rdResolve pour chaque contrainte du Polygon
 *
 * @param P Polygon
**/
void polyResolve(Polygon* P);

/** @brief Accesseur de Fixe
 *
 * @param P Polygon
**/
Bool polyIsFixe(Polygon* P);

/** @brief Mutateur de Fixe
 *
 * Modifie également l'état de TOUT les vertices de P
 * @param P Polygon à modifier
 * @param B Nouvelle valeur pour Fixe
**/
void polySetFixe(Polygon* P, Bool B);

/** @brief Test de régression
 *
 * Fait des tests...
**/
void polyTestRegression();

/** @}
**/

#endif
