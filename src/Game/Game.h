#ifndef _GAME_H_
#define _GAME_H_

#include <string.h>

#include <Rendering/OpenGL.h>
#include <Level/Level.h>
#include <Menu/Menu.h>
#include <Score/Score.h>

/** @defgroup Game
 *
 * Structure du jeu
 * Pr�fixe des m�thodes : gm
 **/
typedef struct
{
	Level* Lvl;
	Menu GameMenu;
	sf::RenderWindow* Window;
	float WindowWidth;
	float WindowHeight;
	float testy;
	Bool testyBool;
	float test2;
} Game;

/** @brief Initialise le jeu
 *
 * Cr�ation de la fen�tre, etc...
 * @param G Game
**/
void gmInit(Game* G);

/** @brief Lib�ration des ressources utilis�es par Game
 *
 * @param G Game
**/
void gmFree(Game* G);

/** @brief Boucle de menu
 *
 * @param G Game
**/
void gmMenu(Game* G);

/** @brief Lance le jeu sur le niveau port�e par Game
 *
 * @param G Game
**/
void gmPlay(Game* G);

/** @brief Charge un niveau depuis un fichier
 *
 * @param G Game
**/
void gmLoadLvl(Game* G, const char* Path);

/** @brief Utilise les input pour le menu
 *
 * @param G Game
 **/
void gmUpdateMenu(Game* G);

/** @brief Dessine le menu
 *
 * @param G Game
 **/
void gmDrawMenu(Game* G);

/** @}
**/

#endif

