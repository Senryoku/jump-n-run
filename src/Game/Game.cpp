#include "Game.h"

void gmInit(Game* G)
{
	G->Lvl = NULL;
}

void gmSetLvl(Game* G, Level* L)
{
	G->Lvl = L;
}

void gmPlay(Game* G)
{
	if(G->Lvl == NULL) return;
	Vec2 Center;

	lvlLoadedInit(G->Lvl);

	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, WindowWidth = 1200.f,
	WindowHeight = 600.f, MapWidth = WindowWidth/10.f, MapHeight = WindowHeight/10.f,
	OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
	ViewXSpeed = 0.f, ViewYSpeed = 0.f, ViewWidth = WindowWidth, ViewHeight = WindowHeight,
	WindowRatio = WindowWidth/WindowHeight;

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Jump'n'Run");
	window.setFramerateLimit(60.f);
	window.setKeyRepeatEnabled(0);
	window.setMouseCursorVisible(0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

	while (window.isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(window).x/WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(window).y/WindowHeight + ViewY;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

			if (event.type == sf::Event::Resized)
				printf("Resized ! %u, %u \n", event.size.width, event.size.height);
		}

		lvlUpdate(G->Lvl);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            plJump(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            plJump(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            plMoveL(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            plMoveR(G->Lvl->P1);

		Center = polyComputeCenter(G->Lvl->P1->Shape);

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glOrtho(Center.x - ViewWidth/2, Center.x + ViewWidth/2, Center.y + ViewHeight/2, Center.y - ViewHeight/2, 0.0, 100.0);

		/* Temporaire ! :) */
		glDrawPolygon(G->Lvl->P1->Shape);
		wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		window.display();
	}
}
