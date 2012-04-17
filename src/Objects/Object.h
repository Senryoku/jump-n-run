#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <Physics/Polygon.h>

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
	unsigned int Texture; /**< Numero de texture, relatif au niveau **/
	List CoordTex; /**< List de Vec2*, coordonnées relatives de la texture **/
} Object;

Object* newObject(Polygon* P, unsigned int T, List CT);

void objInit(Object* Obj, Polygon* P, unsigned int T, List CT);

void objFree(Object* Obj);

void delObject(Object* Obj);

/** @}
**/

#endif
