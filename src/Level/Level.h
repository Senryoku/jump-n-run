#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Physics/Physics.h"
#include "Objects/Player.h"

/** @defgroup Level
 *
 * Décrit un niveau de jeu
 * Préfixe des méthodes : lvl
 **/

typedef struct
{
	World* W;
	Vec2 Spawn;
	Vec2 Goal;
	// char* Background;
	// char* Layer 1 // Juste derrière les objets
	// char* Layer 2 // Juste devant les objets
 	// char* Foreground
 	Player* P1;
} Level;

Level* newLevel(float Width, float Height);
void lvlInit(Level* Lvl, float Width, float Height);
void lvlFree(Level* Lvl);
void delLevel(Level* lvl);

/** @brief Charge un niveau depuis un fichier
 *
**/
void lvlLoad(Level* Lvl, char* Path);

/** @brief Initialise un niveau APRES son chargement complet
 *
 * Le niveau est alors prêt à être joué
**/
void lvlLoadedInit(Level* Lvl);

void lvlUpdate(Level* Lvl);

World* lvlGetWorld(Level* Lvl);

/** @}
**/

#endif

