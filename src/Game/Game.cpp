#include "Game.h"

void gmInit(Game* G)
{
	G->Lvl = NULL;
	G->WindowWidth = 1200.f;
	G->WindowHeight = 600.f;
	G->Window = new sf::RenderWindow(sf::VideoMode(G->WindowWidth, G->WindowHeight), "Jump'n'Run");
	G->Window->setFramerateLimit(60.f);
	G->Window->setKeyRepeatEnabled(0);
	G->Window->setMouseCursorVisible(0);
	G->Window->setActive(1);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);
}

void gmFree(Game* G)
{
	G->Window->setActive(0);
	delete G->Window;
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

	G->Lvl->lvlTexLoad = &glTexLoad;
	G->Lvl->lvlTexFree = &glTexFree;
	G->Lvl->lvlDisplayTex = &glDisplayTex;
	G->Lvl->Layer1 = (*G->Lvl->lvlTexLoad)("Pano.jpg");

	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, MapWidth = G->WindowWidth/10.f, MapHeight = G->WindowHeight/10.f,
	OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
	ViewXSpeed = 0.f, ViewYSpeed = 0.f, ViewWidth = G->WindowWidth, ViewHeight = G->WindowHeight,
	WindowRatio = G->WindowWidth/G->WindowHeight;

	while (G->Window->isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(*G->Window).x/G->WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(*G->Window).y/G->WindowHeight + ViewY;

		sf::Event event;
		while (G->Window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				G->Window->close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				G->Window->close();

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

		/* Temporaire ! A remplacer par les vraies fonctions d'affichage :) */
		lvlDisplayL1(G->Lvl);

		glDrawPolygon(G->Lvl->P1->Shape);
		wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		G->Window->display();
	}
}
