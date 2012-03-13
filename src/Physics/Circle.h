#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "Vertex.h"

/** @defgroup Circle
 *
 * Défini un cercle et fournis des tests de collision
 * Préfixe des méthodes : cir
 * @todo Test de régression complet
 * @todo Test de collision avec un polygone utilisant les régions de Voronoï
 * @{
**/

typedef struct
{
	Vertex* Center;
	float Radius;
} Circle;

/** @}
**/

#endif
