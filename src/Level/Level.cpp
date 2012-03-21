#include "Level.h"

void lvlInit(Level* Lvl, float Width, float Height)
{
	Lvl->W = newWorld(Width, Height);
}

World* lvlGetWorld(Level* Lvl)
{
	return Lvl->W;
}
