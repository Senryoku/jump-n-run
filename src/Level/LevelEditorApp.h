#ifndef _LEVELEDITORAPP_H_
#define _LEVELEDITORAPP_H_

#include <Rendering/OpenGL.h>
#include <Level/LevelEditor.h>

/** @defgroup LevelEditorApp
 *
 * Interface de LevelEditor
 * Préfixe des méthodes : app
 * @todo Sortir la gestion de la configuration
 **/

typedef struct
{
	LevelEditor Led;
	sf::RenderWindow* Window;
	float WindowWidth;
	float WindowHeight;
	char WorkingPath[255];
} LevelEditorApp;

void appInit(LevelEditorApp* App);

void appWindowInit(LevelEditorApp* App);

void appWindowFree(LevelEditorApp* App);

void appFree(LevelEditorApp* App);

void appRun(LevelEditorApp* App);

void appSetWorkingPat(LevelEditorApp* App, const char* Path);

#endif // _LEVELEDITORAPP_H_
