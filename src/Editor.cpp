#include <Level/LevelEditorApp.h>

#include <Score/Score.h>

int main(int argc, char** argv)
{
	sf::Context C;
	C.setActive(1);
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);
	shLoadAudio(&SR);
	shLoadTextures(&SR); //Il faut un contexte valid d'OpenGL
	shLoadFonts(&SR);

	LevelEditorApp App;

	appInit(&App, &SR);
	appRun(&App);
	appFree(&App);

	shFree(&SR);
	return EXIT_SUCCESS;
}


