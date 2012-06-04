#include <Game/Game.h>

int main(int argc, char** argv)
{
	sf::Context C;
	C.setActive(1);
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);
	shLoadAudio(&SR);
	shLoadTextures(&SR);
	shLoadFonts(&SR);

	Game G;

	gmInit(&G, &SR);
	gmPlay(&G);
	gmFree(&G);

	shFree(&SR);

	return EXIT_SUCCESS;
}
