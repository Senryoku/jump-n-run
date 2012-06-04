#include <Level/Level.h>
#include <SFML/OpenGL.hpp>

int main()
{
	sf::Context C;
	C.setActive(1);
	Level *L = newLevel(0, 0);
	printf("1\n");
	lvlLoad(L, "levels/SlidingBoxes.lvl");
	lvlLoadedInit(L);
	
	delLevel(L);
	
	return 0;
}