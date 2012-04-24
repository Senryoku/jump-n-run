#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <math.h>

/**
 * @brief 
 * @param
 * @return
 */

typedef unsigned char AnimType;

#define ANIM_ANGLES 0x00
#define ANIM_POSITIONS 0x01


#define ANIM_FREE 0xFF000000 /**< Définie quand on ne tient pas en compte cette valeur. C'est un NaN **/

typedef struct {
	AnimType Type;
} Animation;


void aniInit(Animation* A);

void aniFree(Animation* A);
 
#endif