#include "Game.h"


void gmInit(Game* G)
{
	Config Cfg = GetConfiguration();

	G->WindowWidth = Cfg.WindowWidth;
	G->WindowHeight = Cfg.WindowHeight;
	G->Window = new sf::RenderWindow(sf::VideoMode(G->WindowWidth, G->WindowHeight), "Jump n'Run", sf::Style::Default, sf::ContextSettings(32));

	//G->Window->setKeyRepeatEnabled(0);
	G->Window->setMouseCursorVisible(1);
	/* On ne peut utiliser  qu'une des deux */
	if(Cfg.VerticalSync == 1.f)
		G->Window->setVerticalSyncEnabled(1);
	else
		G->Window->setFramerateLimit((unsigned int) Cfg.FPSLimit);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(Cfg.AntiAliasing == 1.f) glEnable(GL_LINE_SMOOTH);

	G->Lvl = newLevel(0.f, 0.f);

	ItemID IID;
	mnInit(&G->GameMenu);
	mnSetItemSelectedZoomFactor(&G->GameMenu, 1.f);

	mnAddMenu(&G->GameMenu, "Main Menu", 8);
	mnAddItem(&G->GameMenu, 0, "Item 1", ITEM_BUTTON, NULL, NULL);
	IID = mnAddItem(&G->GameMenu, 0, "Value", ITEM_VALUE, NULL, &G->testy);
	mniSetIncr(mnGetItem(&G->GameMenu, 0, IID), 10.f);
	mniSetMinMaxValues(mnGetItem(&G->GameMenu, 0, IID), -10.f, 110.f);
	G->testy = 0.f;
	MenuID MID = 1;
	mnAddItem(&G->GameMenu, 0, "Input", ITEM_INPUT, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Input value", ITEM_INPUT_VALUE, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "go to Options", ITEM_MENU_SWITCHER, NULL, &MID);
	mnAddItem(&G->GameMenu, 0, "Label 1", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Label", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Checkbox", ITEM_CHECKBOX, NULL, &G->testyBool);

	mnAddMenu(&G->GameMenu, "Options", 4);
	mnAddItem(&G->GameMenu, 1, "full", ITEM_BUTTON, NULL, NULL);
	mnAddItem(&G->GameMenu, 1, "naaa", ITEM_BUTTON, NULL, NULL);
	mnAddItem(&G->GameMenu, 1, "noooo", ITEM_BUTTON, NULL, NULL);
	MID = 0;
	mnAddItem(&G->GameMenu, 1, "Back", ITEM_MENU_SWITCHER, NULL, &MID);



	G->Window->setActive();

}

void gmFree(Game* G)
{
	if(G->Lvl != NULL) delLevel(G->Lvl);
	G->Window->setActive(0);
	G->Window->close();
	delete G->Window;
	mnFree(&G->GameMenu);
}

void gmLoadLvl(Game* G, const char* Path)
{
	lvlLoad(G->Lvl, Path);
}

void gmPlay(Game* G)
{
	if(G->Lvl == NULL) return;
	Vec2 Center;
	Score Sc;

	lvlLoadedInit(G->Lvl);


	float ViewX = 0.f, ViewY = 0.f, MouseX, MouseY, ViewWidth = G->WindowWidth, ViewHeight = G->WindowHeight;

	sf::Clock Clk;
	while (G->Window->isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(*G->Window).x/G->WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(*G->Window).y/G->WindowHeight + ViewY;

		sf::Event event;

		while (G->Window->pollEvent(event))
		{
			mnHandleEvent(&G->GameMenu, event);

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
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            plMoveL(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            plMoveR(G->Lvl->P1);

		plUpdate(G->Lvl->P1);

		if(lvlIsGoalReached(G->Lvl))
		{
			/** @todo Menu demandant le Pseudo et la confirmation de l'envoi du score */
			scInit(&Sc, "Senryoku", G->Lvl->Name, G->Lvl->MD5, Clk.getElapsedTime().asMilliseconds()/10.f);
			// if(scSend(&Sc) == 1) { MenuErreur } else { MenuEnvoiReussi }
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Center = polyComputeCenter(G->Lvl->P1->Shape);
		ViewX = Center.x - ViewWidth/2;
		ViewY = Center.y - ViewHeight/2;
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);

		lvlDisplayL1(G->Lvl);
		lvlDispAllObj(G->Lvl);

		/**@todo Temporaire ! A remplacer par les vraies fonctions d'affichage :) */
		glDrawPolygon(G->Lvl->P1->Shape);
		wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);

		mnUpdate(&G->GameMenu, vec2(100.f, 100.f), vec2(100.f, -mnGetHeight(&G->GameMenu) - 50.f));
		glDrawMenu(*G->Window, &G->GameMenu, ViewX, ViewY);

		G->Window->display();
	}
}


