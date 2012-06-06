#ifndef _GAME_H_
#define _GAME_H_

#include <string.h>

#include <Rendering/OpenGL.h>
#include <Level/Level.h>
#include <Menu/Menu.h>
#include <Score/Score.h>
#include "FPSCounter.h"
#include <Audio/SoundManager.h>
#include <Game/SharedResources.h>
#include <Objects/Animation.h>
#include <Game/Message.h>

/** @defgroup Game Game
 *
 * Structure du jeu
 * Préfixe des méthodes : gm
 **/
typedef struct
{
	Level* Lvl;
	Menu GameMenu;
	sf::RenderWindow* Window;
	float WindowWidth;
	float WindowHeight;
	sf::Clock Clk;
	unsigned int Time;

	float testy;
	Bool testyBool;
	float test2;

	SharedResources* SR; ///< Resources partagŽes
} Game;

/** @brief Initialise le jeu
 *
 * Création de la fenêtre, etc...
 * @param SR SharedRessources
 * @param G Game
**/
void gmInit(Game* G, SharedResources* SR);

/** @brief Libération des ressources utilisées par Game
 *
 * @param[in,out] G Game
**/
void gmFree(Game* G);

/** @brief Boucle de menu
 *
 * @param G Game
**/
void gmMenu(Game* G);

/** @brief Lance le jeu sur le niveau portée par Game
 *
 * @param[in,out] G Game
**/
void gmPlay(Game* G);

/** @brief Charge un niveau depuis un fichier
 *
 * @param[in,out] G Game
 * @param[in] Path Chemin du fichier de niveau à charger
**/
void gmLoadLvl(Game* G, const char* Path);

/** @brief Utilise les input pour le menu
 *
 * @param G Game
 **/
void gmUpdateMenu(Game* G);

/** @brief Dessine le menu
 *
 * @param[in] G Game
 **/
void gmDrawMenu(Game* G);

/** @brief Réinitialise le temps
 * @param[in,out] G Game
**/
void gmResetClk(Game* G);

/** @brief Met en pause le décompte du temps
 * @param[in,out] G Game
**/
void gmPauseClk(Game* G);

/** @brief Relance le décompte du temps
 * @param[in,out] G Game
**/
void gmRestartClk(Game* G);


/** @}
**/

#endif

