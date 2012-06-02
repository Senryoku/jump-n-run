#include <Game/Game.h>

int main(int argc, char** argv)
{
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);
	shLoadAudio(&SR);

	Game G;

	gmInit(&G, &SR);
	shLoadTextures(&SR);
	shLoadFonts(&SR);
	gmPlay(&G);
	gmFree(&G);

	shFree(&SR);

	return EXIT_SUCCESS;
}
