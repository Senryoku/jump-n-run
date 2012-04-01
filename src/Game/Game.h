#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Rendering/OpenGL.h"
#include "Level/Level.h"

/** @defgroup Game
 *
 * Structure du jeu
 * Préfixe des méthodes : gm
 **/
typedef struct
{
	Level* Lvl;
	sf::RenderWindow* Window;
	float WindowWidth;
	float WindowHeight;
} Game;

void gmInit(Game*);

void gmFree(Game*);

void gmMenu(Game*);

void gmPlay(Game*);

void gmSetLvl(Game*, Level*);

/** @}
**/

#endif

