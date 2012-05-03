#include <Level/LevelEditorApp.h>

#include <Score/Score.h>

int main(int argc, char** argv)
{
	/*Score Sc;
	scInit(&Sc, "Daas", "tmpEditor.lvl", "3557a855ba37d9b60bc18583d99eb254", 8000);
	if(scSend(&Sc))
		printf("Erreur d'envoi\n");
	else
		printf("Score envoyé\n");
	scFree(&Sc);
	*/

	scRegressionTest();

	LevelEditorApp App;

	appInit(&App);
	appRun(&App);
	appFree(&App);

	return EXIT_SUCCESS;
}


