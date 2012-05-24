#ifndef _LEVELEDITORAPP_H_
#define _LEVELEDITORAPP_H_

#include <Rendering/OpenGL.h>
#include <Level/LevelEditor.h>
#include <Game/FPSCounter.h>
#include <Audio/SoundManager.h>
#include <Menu/Menu.h>

/** @defgroup LevelEditorApp LevelEditorApplication
 *
 * Interface de LevelEditor
 * Préfixe des méthodes : app
 * @{
 **/

typedef struct
{
	LevelEditor Led;
	sf::RenderWindow Window;
	Bool WindowIsActive;
	float WindowWidth;
	float WindowHeight;
	float ViewX;
	float ViewY;
	float ViewWidth;
	float ViewHeight;
	char WorkingPath[255];
	Menu M;
	bool MenuUsed;
	SharedResources* SR; ///< Resources Partagées
	Polygon* NearestPolygon;
} LevelEditorApp;

/** @brief Initialisation de LelvelEditorApp
 *
 * @param App LevelEditorApp à initialiser
**/
void appInit(LevelEditorApp* App, SharedResources* SR);

/** @brief Initialistion la fenêtre
 *
 * Est appellé par appInit
 * @param App LevelEditorApp à initialiser
**/
void appWindowInit(LevelEditorApp* App);

/** @brief Libère les ressources  utilisées par LevelEditorApp
 *
 * @param App LevelEditorApp à libérer
**/
void appFree(LevelEditorApp* App);

/** @brief Lance l'éditeur de niveau
 *
 * Boucle principale de l'éditeur de niveau
 * @param App LevelEditorApp à lancer
**/
void appRun(LevelEditorApp* App);

/** @brief Sélectionne le fichier en cours d'édition
 *
 * levels/tmpEditor.lvl par défaut
 * Détermine où est sauvegardé le niveau en cours et quel fichier sera chargé
 * @param App LevelEditorApp à modifier
 * @param Path Nouveau WorkingPath
**/
void appSetWorkingPath(LevelEditorApp* App, const char* Path);

/** @}
**/

#endif // _LEVELEDITORAPP_H_
