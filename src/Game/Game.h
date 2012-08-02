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
#include "Replay.h"
#include <Core/ResourcePath.h>

/** @defgroup Game Game
 *
 * Structure du jeu
 * Préfixe des méthodes : gm
 **/
typedef struct
{
	char Path[255]; /**< Chemin du niveau en cours **/
	Level* Lvl; /**< Niveau en cours **/
	sf::RenderWindow* Window; /**< Fenêtre de rendu **/
	float WindowWidth; /**< Largeur de la fenêtre **/
	float WindowHeight; /**< Hauteur de la fenêtre **/
	sf::Clock Clk; /**< Horloge **/
	unsigned int Time; /**< Temps (score) courrant **/
	Bool WindowIsActive;
	Replay* Rep;
	char ReplayFile[1024];
	Bool IsAReplay;
	SharedResources* SR; ///< Resources partagées
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
 * @return Vrai si le niveau a bien été chargé
**/
Bool gmLoadLvl(Game* G, const char* Path);

/** @brief Charge une replay
 *
 * @param[in,out] G Game
 * @param[in] Path Chemin du replay de niveau à charger
 * @return Vrai si le replay a bien été chargé
 **/
Bool gmLoadReplay(Game* G, const char* Path);

/** @brief Recharge le niveau précédement chargé
 *
 * @param[in,out] G Game
 * @return Vrai si le niveau a bien été chargé
 **/
Bool gmReloadLevel(Game* G);

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

/** @brief Affiche les scores du niveau chargŽ
 * @param[in,out] G Game
 **/
void gmShowScores(Game* G);



/** @}
**/

#endif

