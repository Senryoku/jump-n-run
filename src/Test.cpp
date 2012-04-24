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

	DynArr DA;
	unsigned int ErrorCode = scCollect(&DA, "tmpEditor.lvl", "3557a855ba37d9b60bc18583d99eb254");
	if(ErrorCode == 0)
	{
		printf("Number of Score(s) : %u\n", daGetSize(&DA));
		for(unsigned int i = 0; i < daGetSize(&DA); i++)
		{
			Score* S = (Score*) daGet(&DA, i);
			printf("Player : %s, Hour : %s, Date : %s, Time : %u\n", S->Player, S->LvlName, S->LvlMD5, S->Time);
		}
		scCollectFree(&DA);
	}

	LevelEditorApp App;

	appInit(&App);
	appRun(&App);
	appFree(&App);

	return EXIT_SUCCESS;
}


