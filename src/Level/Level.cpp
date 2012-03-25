#include "Level.h"

Level* newLevel(float Width, float Height)
{
	Level* lvl = (Level*) malloc(sizeof(Level));
	lvlInit(lvl, Width, Height);
	return lvl;
}

void lvlInit(Level* Lvl, float Width, float Height)
{
	Lvl->W = newWorld(Width, Height);
}

void lvlFree(Level* Lvl)
{
	delWorld(Lvl->W);
}

void delLevel(Level* lvl)
{
	lvlFree(lvl);
	free(lvl);
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}
