#ifndef _RIGID_H_
#define _RIGID_H_

#include "Vertex.h"

/** @defgroup Rigid
 *
 * Liaison Rigid entre deux Vertex
 * Préfixe des méthodes : rd
 * @{
**/

typedef struct
{
	Vertex* V1;
	Vertex* V2;
	float Length;
} Rigid;

Rigid* newRigid();

void rdResolve(Vertex* V);

/** @}
**/

#endif
