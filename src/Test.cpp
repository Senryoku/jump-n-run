#include <Level/LevelEditorApp.h>

int main(int argc, char** argv)
{
	LevelEditorApp App;

	appInit(&App);
	appRun(&App);
	appFree(&App);

	return EXIT_SUCCESS;
}


