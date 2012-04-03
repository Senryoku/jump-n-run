#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <Physics/Polygon.h>

/** @defgroup Object
 *
 * Préfixe des méthodes : obj
 **/

/** @brief Objet dynamique afichable (texturé)
 *
 * @todo Réfléchir à la façon de binder la texture (Liste de coordonnée ?
 * Une par Vexter de Shape ? Toujours 4 (Shape = Rectangle) ?...)
**/
typedef struct {
	Polygon* Shape; /**< Polygon de collision **/
	unsigned int Texture; /**< Numero de texture, relatif au niveau **/
} Object;

/** @}
**/

#endif
