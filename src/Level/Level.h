#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Physics/Physics.h"

typedef struct
{
	World* W;
	Vec2 Spawn;
	Vec2 Goal;
	// char* Background;
	// char* Layer 1 // Juste derrière les objets
	// char* Layer 2 // Juste devant les objets
 	// char* Foreground
} Level;

Level* newLevel(float Width, float Height);
void lvlInit(Level* Lvl, float Width, float Height);

World* lvlGetWorld(Level* Lvl);

#endif

