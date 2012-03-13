#ifndef _WORLD_H_
#define _WORLD_H_

#include "List.h"
#include "Vertex.h"
#include "Rigid.h"
#include "Polygon.h"
#include "Circle.h"

/** @defgroup World
 *
 * Liste les objets du monde, appelle les méthodes de mise à jour
 * Préfixe des fonctions : wd
 * @{
**/

typedef struct
{
	List Vertices; /**< Liste de Vertices **/
	List Rigids; /**< Liste de contraintes isolées **/
	List Polygons; /**< Liste de Polygones **/
	List Circles; /**< Liste de Cercles **/

	float Width; /**< Largeur du monde **/
	float Heigth; /**< Hauteur du monde **/
} World;

/** @brief Appelle Resolve() pour tout les Rigid du monde
 *
 * Resolve() les contraintes rigides isolées et contenues dans des polygones
 * non fixes.
 * @param W World
**/
void wdResolveRigid(World* W);

/** @brief Recherche et réagit aux collisions
 *
 * Gère les collision Polygon/Polygon, Circle/Polygon, Circle/Cirle
 * @param W World
**/
void wdHandleCollision(World* W);

/** @}
**/


#endif

