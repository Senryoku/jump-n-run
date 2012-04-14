#include <Level/LevelEditorApp.h>

#include <Score/Score.h>

int main(int argc, char** argv)
{
	/*
	Score Sc;
	scInit(&Sc, "Senryoku", "Test.lvl", "375", 25000);
	if(scSend(&Sc))
		printf("Erreur d'envoi\n");
	else
		printf("Score envoyé\n");
	scFree(&Sc);
	*/

	LevelEditorApp App;

	appInit(&App);
	appRun(&App);
	appFree(&App);

	return EXIT_SUCCESS;
}


