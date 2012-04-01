#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Rendering/OpenGL.h"
#include "Level/Level.h"

/** @defgroup Game
 *
 * Structure du jeu
 * Pr�fixe des m�thodes : gm
 **/
typedef struct
{
	Level* Lvl;
	sf::RenderWindow* Window;
	float WindowWidth;
	float WindowHeight;
} Game;

/** @brief Initialise le jeu
 *
 * Cr�ation de la fen�tre, etc...
 * @param G Game
**/
void gmInit(Game*);

/** @brief Lib�ration des ressources utilis�es par Game
 *
 * @param G Game
**/
void gmFree(Game*);

/** @brief Boucle de menu
 *
 * @param G Game
**/
void gmMenu(Game*);

/** @brief Lance le jeu sur le niveau port�e par Game
 *
 * @param G Game
**/
void gmPlay(Game*);

/** @brief Mutateur du niveau
 *
 * @param G Game
**/
void gmSetLvl(Game*, Level*);

/** @}
**/

#endif

