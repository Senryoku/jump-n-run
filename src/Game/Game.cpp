#include "Game.h"

void gmInit(Game* G)
{
	char line[255];
	char id[255];
	float value;

	/* Options par défaut */
	float WindowWidth = 800.f;
	float WindowHeight = 600.f;
	float FPSLimit = 60.f;
	float AA = 1.f;

	G->Lvl = newLevel(0.f, 0.f);

	FILE* f;
	f = fopen("Config.cfg", "r");
	if(f != NULL)
	{
		while (fgets(line, 255, f) != NULL)
		{
			sscanf(line, "%s %f\n", id, &value);
			printf("Lu : %s, %f\n", id, value);
			if(strcmp(id, "WindowWidth") == 0) WindowWidth = value;
			if(strcmp(id, "WindowHeigth") == 0) WindowHeight = value;
			if(strcmp(id, "FPSLimit") == 0) FPSLimit = value;
			if(strcmp(id, "AA") == 0) AA = value;
		}
		fclose(f);
	}

	G->WindowWidth = WindowWidth;
	G->WindowHeight = WindowHeight;
	G->Window = new sf::RenderWindow(sf::VideoMode(G->WindowWidth, G->WindowHeight), "Jump'n'Run");
	G->Window->setFramerateLimit(FPSLimit);
	G->Window->setKeyRepeatEnabled(0);
	G->Window->setMouseCursorVisible(1);
	if (G->Window->setActive(1))
		printf("G->windows activated\n");

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	if(AA) glEnable(GL_LINE_SMOOTH);
}

void gmFree(Game* G)
{
	if(G->Lvl != NULL) delLevel(G->Lvl);
	G->Window->setActive(0);
	G->Window->close();
	delete G->Window;
}

void gmLoadLvl(Game* G, const char* Path)
{
	lvlLoad(G->Lvl, Path);
}

void gmPlay(Game* G)
{
	if(G->Lvl == NULL) return;
	Vec2 Center;

	lvlLoadedInit(G->Lvl);


	float ViewX = 0.f, ViewY = 0.f, MouseX, MouseY, ViewWidth = G->WindowWidth, ViewHeight = G->WindowHeight;

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

			if(event.type == sf::Event::MouseButtonPressed)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left :
						plGrab(G->Lvl->P1, lvlGetWorld(G->Lvl), MouseX, MouseY);
						break;
					default :
						break;
				}
			}

			if(event.type == sf::Event::MouseButtonReleased)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left :
						plRelease(G->Lvl->P1, lvlGetWorld(G->Lvl));
						break;
					default :
						break;
				}
			}


		}

		lvlUpdate(G->Lvl);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            plJump(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            plJump(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            plMoveL(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            plMoveR(G->Lvl->P1);

		Center = polyComputeCenter(G->Lvl->P1->Shape);

		G->Window->setActive();

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		ViewX = Center.x - ViewWidth/2;
		ViewY = Center.y - ViewHeight/2;
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);

		/* Temporaire ! A remplacer par les vraies fonctions d'affichage :) */
		lvlDisplayL1(G->Lvl);

		glDrawPolygon(G->Lvl->P1->Shape);
		wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);

		G->Window->display();
	}
}
