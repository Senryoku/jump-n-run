#ifndef _LEVELEDITORAPP_H_
#define _LEVELEDITORAPP_H_

#include <Rendering/OpenGL.h>
#include <Level/LevelEditor.h>
#include <Game/FPSCounter.h>

/** @defgroup LevelEditorApp
 *
 * Interface de LevelEditor
 * Préfixe des méthodes : app
 **/

typedef struct
{
	LevelEditor Led;
	sf::RenderWindow Window;
	Bool WindowIsActive;
	float WindowWidth;
	float WindowHeight;
	char WorkingPath[255];
} LevelEditorApp;

/** @brief Initialisation de LelvelEditorApp
 *
**/
void appInit(LevelEditorApp* App);

/** @brief Initialistion la fenêtre
 *
 * Est appellé par appInit
**/
void appWindowInit(LevelEditorApp* App);


/** @brief Libère les ressources  utilisées par LevelEditorApp
 *
**/
void appFree(LevelEditorApp* App);

/** @brief Lance l'éditeur de niveau
 *
 * Boucle principale de l'éditeur de niveau
**/
void appRun(LevelEditorApp* App);

/** @brief Sélectionne le fichier en cours d'édition
 *
 * levels/tmpEditor.lvl par défaut
 * Détermine où est sauvegardé le niveau en cours et quel fichier sera chargé
**/
void appSetWorkingPath(LevelEditorApp* App, const char* Path);

#endif // _LEVELEDITORAPP_H_
