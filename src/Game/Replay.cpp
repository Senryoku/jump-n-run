
#include "Replay.h"
#include <Core/ResourcePath.h>


Replay* newReplay()
{
	return (Replay*)malloc(sizeof(Replay));
}

void delReplay(Replay* R)
{
	if (R->f != NULL)
		fclose(R->f);
	else
		printf("Cannot save replay!\n");
	free(R);
}

void rpInitRecording(Replay* R, const char* File, const char LevelFile[255])
{
	R->f = fopen(File, "wb");
	char file[255] ;
	sprintf(file,(ResourcePath()+"levels/%s").c_str(),LevelFile);
	if (R->f == NULL)
		printf("Error creating replay file %s\n", File);
	else
		fwrite(file, sizeof(char), 255, R->f);
}

void rpInitReading(Replay* R, const char* File, char* LevelFile)
{
	R->f = fopen(File, "rb");
	if (R->f != NULL)
	{
		fread(LevelFile, sizeof(char), 255, R->f);
	}
}

void rpAddAction(Replay* R, rpAction Action)
{
	if (fwrite(&Action, sizeof(rpAction), 1, R->f) != 1)
		printf("Error writing to replay file\n");
}

rpAction rpReadAction(Replay* R)
{
	rpAction Action;
	if (fread(&Action, sizeof(rpAction), 1, R->f) == 0)
		return END_OF_FILE;
	else
		return Action;
}