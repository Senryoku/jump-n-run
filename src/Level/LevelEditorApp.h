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
} LevelEditorApp;

void appInit(LevelEditorApp* App);

void appFree(LevelEditorApp* App);

void appRun(LevelEditorApp* App);

#endif // _LEVELEDITORAPP_H_
