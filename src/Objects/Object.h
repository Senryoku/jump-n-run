#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <Physics/Polygon.h>
#include <Rendering/OpenGl.h>


/** @defgroup Object
 *
 * Préfixe des méthodes : obj
 **/

/** @brief Objet dynamique afichable (texturé)
 *
 * Préfixe des fonctions : obj
**/
typedef struct {
	Polygon* Shape; /**< Polygon de collision **/
	Texture Tex; /**< Numero de texture, relatif au niveau **/
	List CoordTex; /**< List de Vec2*, coordonnées relatives de la texture **/
} Object;

/** @brief Constructeur
 *
 * @param P Polygon sur lequel sera appliqué la texture
 * @param T Identifiant de texture (relatif à un Lvl)
 * @param CT List de Vec2* faisant la liaison entre les Vertex et des points de la texture
**/
Object* newObject(Polygon* P, Texture T, List CT);

/** @brief Constructeur par Copie
 *
 * @param O Object à copier
 * @return Pointeur vers la copie
**/
Object* cpyObject(Object* O);

/** @brief Initialisation
 *
 * @param Obj Objrtect à Initialiser
 * @param P Polygon sur lequel sera appliqué la texture
 * @param T Identifiant de texture (relatif à un Lvl)
 * @param CT List de Vec2* faisant la liaison entre les Vertex et des points de la texture
**/
void objInit(Object* Obj, Polygon* P, Texture T, List CT);

/** @brief Libération des ressources
 *
 * Libère également tout les Vec2* de CoordTex
 * @param Obj Object à Libérer
**/
void objFree(Object* Obj);

/** @brief Destructeur
 *
 * Appelle également objFree
**/
void delObject(Object* Obj);

/** @brief Accesseur de Shape
 *
 * @param Obj Object à accéder
 * @return Shape
**/
Polygon* objGetShape(Object* Obj);

/** @}
**/

#endif
