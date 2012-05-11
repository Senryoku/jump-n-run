#include "Game.h"
#include <Objects/Animation.h>


void gmInit(Game* G)
{
	Config Cfg = GetConfiguration();

	G->WindowWidth = Cfg.WindowWidth;
	G->WindowHeight = Cfg.WindowHeight;
	G->Window = new sf::RenderWindow(sf::VideoMode(G->WindowWidth, G->WindowHeight), "Jump n'Run", sf::Style::Default, sf::ContextSettings(32));

	G->Window->setKeyRepeatEnabled(0);
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
	mnSetItemNormalZoomFactor(&G->GameMenu, 0.75f);

	mnAddMenu(&G->GameMenu, "Main Menu", 9);
	mnAddItem(&G->GameMenu, 0, "Item 1", ITEM_BUTTON, NULL, NULL);
	IID = mnAddItem(&G->GameMenu, 0, "Value", ITEM_VALUE, NULL, &G->testy);
	mniSetIncr(mnGetItem(&G->GameMenu, 0, IID), 10000000.f);
	//mniSetMinMaxValues(mnGetItem(&G->GameMenu, 0, IID), -10.f, 110.f);
	G->testy = 0.f;
	MenuID MID = 1;
	mnAddItem(&G->GameMenu, 0, "Input", ITEM_INPUT, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Input multiligne", ITEM_INPUT_MULTILINE, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Input value", ITEM_INPUT_VALUE, NULL, NULL);
	mnAddItemMenuSwitcher(&G->GameMenu, 0, "go to Options", 1);
	mnAddItem(&G->GameMenu, 0, "Label 1", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "------\n------", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Checkbox", ITEM_CHECKBOX, NULL, &G->testyBool);

	mnAddMenu(&G->GameMenu, "Options", 4);
	mnAddItem(&G->GameMenu, 1, "Whaaaaaow", ITEM_BUTTON, NULL, NULL);
	mnAddItem(&G->GameMenu, 1, "CraAAaAzYy!!", ITEM_BUTTON, NULL, NULL);
	MID = 2;
	mnAddItemMenuSwitcher(&G->GameMenu, 1, "A Secreeeet", 2);
	MID = 0;
	mnAddItemMenuSwitcher(&G->GameMenu, 1, "Back", 0);

	mnAddMenu(&G->GameMenu, "", 3);
	mnAddItem(&G->GameMenu, 2, "I HAVE NO TITLE!!!! HA HA HA!", ITEM_LABEL, NULL, NULL);
	mnAddItemMenuSwitcher(&G->GameMenu, 2, "Go to main MENU!!!!!!", 0);
	MID=1;
	mnAddItemMenuSwitcher(&G->GameMenu, 2, "Go Back!", 1);


	G->Window->setActive();
}

void gmFree(Game* G)
{
	if(G->Lvl != NULL) delLevel(G->Lvl);
	G->Window->setActive(0);
	G->Window->close();
	//delete G->Window; // Provoque une segfault sous Windows
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

	Animation* A = newAnimation(ANIM_ANGLES, ANIM_ALL_TRIGGERS, TRUE);
	aniLoadFromFile(A, "data/animRun.txt");
	aniUpdateForCurrentState(A);

	float ViewX = 0.f, ViewY = 0.f, MouseX, MouseY, ViewWidth = G->WindowWidth, ViewHeight = G->WindowHeight;

	sf::Clock Clk;
	FPSCounter fps;
	fpsInit(&fps);
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
					case sf::Mouse::Right :
						plGrabR(G->Lvl->P1, lvlGetWorld(G->Lvl), MouseX, MouseY);
						break;
					case sf::Mouse::Left :
						plGrabL(G->Lvl->P1, lvlGetWorld(G->Lvl), MouseX, MouseY);
						break;
					default :
						break;
				}
			}

			if(event.type == sf::Event::MouseButtonReleased)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Right :
						plReleaseR(G->Lvl->P1, lvlGetWorld(G->Lvl));
						break;
					case sf::Mouse::Left :
						plReleaseL(G->Lvl->P1, lvlGetWorld(G->Lvl));
						break;
					default :
						break;
				}
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M)
				mnSetHide(&G->GameMenu, !mnGetHide(&G->GameMenu));

			mnHandleEvent(&G->GameMenu, event);
		}

		lvlUpdate(G->Lvl, FALSE);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            plJump(G->Lvl->P1);
		else
			plResetJump(G->Lvl->P1);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			plGetUp(G->Lvl->P1);
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            plMoveL(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            plMoveR(G->Lvl->P1);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            plRotateR(G->Lvl->P1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            plRotateL(G->Lvl->P1);


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
		ViewX = MAX(0, MIN(Center.x - ViewWidth/2, wdGetWidth(lvlGetWorld(G->Lvl)) - ViewWidth));
		ViewY = MAX(0, MIN(Center.y - ViewHeight/2, wdGetHeight(lvlGetWorld(G->Lvl)) - ViewHeight));
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);

		lvlDisplayBG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight);
		lvlDisplayL1(G->Lvl);
		lvlDispAllObj(G->Lvl);
		lvlDisplayL2(G->Lvl);
		lvlDisplayFG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight);
		aniUpdate(A, G->Lvl->P1, 1.f);

		sndmUpdate();

		/**@todo Temporaire ! A remplacer par les vraies fonctions d'affichage :) */
		glDrawPolygon(G->Lvl->P1->Shape);
		wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);

		mnUpdate(&G->GameMenu, vec2(100.f, 100.f), vec2(100.f, -mnGetHeight(&G->GameMenu) - 100.f));
		glDrawMenu(*G->Window, &G->GameMenu, ViewX, ViewY);
		glDrawFPS(*G->Window, fpsGetString(&fps));
		fpsStep(&fps);

		G->Window->display();
	}
	delAnimation(A);
}


