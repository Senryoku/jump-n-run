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
 * Pr�fixe des m�thodes : gm
 **/
typedef struct
{
	Level* Lvl; /**< Niveau en cours **/
	sf::RenderWindow* Window; /**< Fen�tre de rendu **/
	float WindowWidth; /**< Largeur de la fen�tre **/
	float WindowHeight; /**< Hauteur de la fen�tre **/
	sf::Clock Clk; /**< Horloge **/
	unsigned int Time; /**< Temps (score) courrant **/

	SharedResources* SR; ///< Resources partag�es
} Game;

/** @brief Initialise le jeu
 *
 * Cr�ation de la fen�tre, etc...
 * @param SR SharedRessources
 * @param G Game
**/
void gmInit(Game* G, SharedResources* SR);

/** @brief Lib�ration des ressources utilis�es par Game
 *
 * @param[in,out] G Game
**/
void gmFree(Game* G);

/** @brief Boucle de menu
 *
 * @param G Game
**/
void gmMenu(Game* G);

/** @brief Lance le jeu sur le niveau port�e par Game
 *
 * @param[in,out] G Game
**/
void gmPlay(Game* G);

/** @brief Charge un niveau depuis un fichier
 *
 * @param[in,out] G Game
 * @param[in] Path Chemin du fichier de niveau � charger
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

/** @brief R�initialise le temps
 * @param[in,out] G Game
**/
void gmResetClk(Game* G);

/** @brief Met en pause le d�compte du temps
 * @param[in,out] G Game
**/
void gmPauseClk(Game* G);

/** @brief Relance le d�compte du temps
 * @param[in,out] G Game
**/
void gmRestartClk(Game* G);


/** @}
**/

#endif

