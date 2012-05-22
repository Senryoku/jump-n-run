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

	//scRegressionTest();
	/*for (int i=0; i<1000; i++)
	{
		FILE* f;
		f = fopen("data/animRun.txt", "r");
		printf("md5: %s\n", md5FromFile("data/animRun.txt").c_str());
		fclose(f);
	}*/
	
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);

	LevelEditorApp App;

	appInit(&App, &SR);
	shLoadTextures(&SR); //Il faut un contexte valid d'OpenGL
	shLoadFonts(&SR);
	shLoadAudio(&SR);
	appRun(&App);
	appFree(&App);

	shFree(&SR);

	return EXIT_SUCCESS;
}


