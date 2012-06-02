#include <Level/LevelEditorApp.h>

#include <Score/Score.h>

int main(int argc, char** argv)
{
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);
	shLoadAudio(&SR);

	LevelEditorApp App;

	appInit(&App, &SR);
	shLoadTextures(&SR); //Il faut un contexte valid d'OpenGL
	shLoadFonts(&SR);
	appRun(&App);
	appFree(&App);

	shFree(&SR);

	return EXIT_SUCCESS;
}


