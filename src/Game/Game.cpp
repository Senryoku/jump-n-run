#include "Game.h"
#include <Objects/Animation.h>
#include <Game/Message.h>


void gmInit(Game* G, SharedResources* SR)
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

	G->SR = SR;

	ItemID IID;
	mnInit(&G->GameMenu);
	mnSetItemSelectedZoomFactor(&G->GameMenu, 1.f);
	mnSetItemNormalZoomFactor(&G->GameMenu, 0.75f);

	mnAddMenu(&G->GameMenu, "Main Menu", 9);
	mnAddItem(&G->GameMenu, 0, "Item 1", ITEM_BUTTON, NULL, NULL);
	IID = mnAddItem(&G->GameMenu, 0, "Value", ITEM_VALUE, NULL, &G->testy);
	mniSetIncr(mnGetItem(&G->GameMenu, 0, IID), 10000000.f);
	mniSetFloatPrecision(mnGetItem(&G->GameMenu, 0, IID), 3);
	//mniSetMinMaxValues(mnGetItem(&G->GameMenu, 0, IID), -10.f, 110.f);&&&&&&&&&&&&&&&&&&&
	G->testy = 0.f;
	mnAddItem(&G->GameMenu, 0, "Input", ITEM_INPUT, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Input multiligne", ITEM_INPUT_MULTILINE, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Input value", ITEM_INPUT_VALUE, NULL, &G->testy);
	mnAddItemMenuSwitcher(&G->GameMenu, 0, "go to Options", 1);
	mnAddItem(&G->GameMenu, 0, "Label 1", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "------\n------", ITEM_LABEL, NULL, NULL);
	mnAddItem(&G->GameMenu, 0, "Checkbox", ITEM_CHECKBOX, NULL, &G->testyBool);

	mnAddMenu(&G->GameMenu, "Options", 4);
	mnAddItem(&G->GameMenu, 1, "Whaaaaaow", ITEM_BUTTON, NULL, NULL);
	mnAddItem(&G->GameMenu, 1, "CraAAaAzYy!!", ITEM_BUTTON, NULL, NULL);
	mnAddItemMenuSwitcher(&G->GameMenu, 1, "A Secreeeet", 2);
	mnAddItemMenuSwitcher(&G->GameMenu, 1, "Back", 0);

	mnAddMenu(&G->GameMenu, "", 3);
	mnAddItem(&G->GameMenu, 2, "I HAVE NO TITLE!!!! HA HA HA!", ITEM_LABEL, NULL, NULL);
	mnAddItemMenuSwitcher(&G->GameMenu, 2, "Go to main MENU!!!!!!", 0);
	mnAddItemMenuSwitcher(&G->GameMenu, 2, "Go Back!", 1);

	mnSetHide(&G->GameMenu, TRUE);

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
	Bool DispDebug = FALSE;

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
						plGrabR(lvlGetP1(G->Lvl), lvlGetWorld(G->Lvl), MouseX, MouseY);
						break;
					case sf::Mouse::Left :
						plGrabL(lvlGetP1(G->Lvl), lvlGetWorld(G->Lvl), MouseX, MouseY);
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
						plReleaseR(lvlGetP1(G->Lvl), lvlGetWorld(G->Lvl));
						break;
					case sf::Mouse::Left :
						plReleaseL(lvlGetP1(G->Lvl), lvlGetWorld(G->Lvl));
						break;
					default :
						break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::Comma :
						DispDebug = !DispDebug;
						break;
					case sf::Keyboard::M :
						mnSetHide(&G->GameMenu, !mnGetHide(&G->GameMenu));
						break;
					case sf::Keyboard::J :
					{
						msgCreateMessage(shMessageManager(G->SR), "test message", 4);
						msgAddItem(shMessageManager(G->SR), "Hello I'm a message! Do you like me?", ITEM_LABEL, NULL, NULL);
						msgAddCloseItem(shMessageManager(G->SR), "Yes");
						msgAddCloseItem(shMessageManager(G->SR), "Bah...");
						msgAddCloseItem(shMessageManager(G->SR), "No");
						//msgAddItemWithArg(shMessageManager(G->SR), "Yes", &CloseMessage, shMessageManager(G->SR));
						ItemID i = msgGetChoice(shMessageManager(G->SR), *G->Window, ViewX, ViewY, ViewWidth, ViewHeight);

						msgCreateMessage(shMessageManager(G->SR), "Alert", 3);
						if (i==1)
							msgAddItem(shMessageManager(G->SR), "Coool :D!", ITEM_LABEL, NULL, NULL);
						else if (i==2)
							msgAddItem(shMessageManager(G->SR), "that makes me sad...", ITEM_LABEL, NULL, NULL);
						else if (i==3)
							msgAddItem(shMessageManager(G->SR), "D: You're an orrible person", ITEM_LABEL, NULL, NULL);
						else
							msgAddItem(shMessageManager(G->SR), "Y U NO ANSWER?", ITEM_LABEL, NULL, NULL);

						//msgAddItem(shMessageManager(G->SR), "Why", ITEM_INPUT, NULL, NULL);
						float a=3.f;
						ItemID IID = msgAddItem(shMessageManager(G->SR), "floty", ITEM_VALUE, NULL, &a);
						MenuItem* I = mnGetItem(msgGetMenu(shMessageManager(G->SR)), 0, IID);
						mniSetMinMaxValues(I, -10.f, 10.f);
						mniSetIncr(I, 0.5f);
						mniSetFloatPrecision(I, 1); //Toujours à la fin car ça fait la mise à jour sur le texte


						msgAddCloseItem(shMessageManager(G->SR), "Dismiss");

						const char* t = msgGetInput(shMessageManager(G->SR), *G->Window, ViewX, ViewY, ViewWidth, ViewHeight);

						msgCreateMessage(shMessageManager(G->SR), "Alert", 2);
						msgAddItem(shMessageManager(G->SR), t, ITEM_LABEL, NULL, NULL);
						msgAddCloseItem(shMessageManager(G->SR), "Dismiss");
						msgDisplay(shMessageManager(G->SR), *G->Window, ViewX, ViewY, ViewWidth, ViewHeight);
						break;
					}
					default:
						break;
				}
			}

			mnHandleEvent(&G->GameMenu, event);
			//msgHandleEvent(event);
		}

		//msgUpdate();

		lvlUpdate(G->Lvl, FALSE);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            plJump(lvlGetP1(G->Lvl));
		else
			plResetJump(lvlGetP1(G->Lvl));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			plGetUp(lvlGetP1(G->Lvl));
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            plMoveL(lvlGetP1(G->Lvl));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            plMoveR(lvlGetP1(G->Lvl));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            plRotateR(lvlGetP1(G->Lvl));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            plRotateL(lvlGetP1(G->Lvl));


		if(lvlIsGoalReached(G->Lvl))
		{
			lvlSetFinished(G->Lvl, 1);
			/** @todo Menu demandant le Pseudo et la confirmation de l'envoi du score */
			scInit(&Sc, "Senryoku", lvlGetFilename(G->Lvl), lvlGetMD5(G->Lvl), Clk.getElapsedTime().asMilliseconds()/10.f);
			// if(scSend(&Sc) == 1) { MenuErreur } else { MenuEnvoiReussi }
		}


		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Center = polyComputeCenter(lvlGetP1(G->Lvl)->Shape);
		ViewX = MAX(0, MIN(Center.x - ViewWidth/2, wdGetWidth(lvlGetWorld(G->Lvl)) - ViewWidth));
		ViewY = MAX(0, MIN(Center.y - ViewHeight/2, wdGetHeight(lvlGetWorld(G->Lvl)) - ViewHeight));
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);


		lvlDisplayBG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight, *G->Window);
		lvlDisplayL1(G->Lvl);
		lvlDispAllObj(G->Lvl);
		lvlDispGoalFlag(G->Lvl);
		lvlDisplayL2(G->Lvl);
		lvlDisplayFG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight);
		aniUpdate(A, lvlGetP1(G->Lvl), 1.f);

		sndmUpdate(shSoundManager(G->SR));

		if(DispDebug)
		{
			glDrawPolygon(lvlGetP1(G->Lvl)->Shape);
			wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);
			glDrawFPS(G->SR, *G->Window, fpsGetString(&fps));
		}

		float menuPosx = G->Window->getSize().x/2.f - moiGetSize(mnGetCurrentMenu(&G->GameMenu)).x/2.f;
		mnUpdate(&G->GameMenu, vec2(menuPosx, 100.f), vec2(menuPosx, -mnGetHeight(&G->GameMenu) - 100.f));
		glDrawMenuBox(G->SR, *G->Window, &G->GameMenu, ViewX, ViewY, ViewWidth, ViewHeight);

		//if (msgCanBeDrawn()) glDrawMenuBox(*G->Window, msgGetMenu(), ViewX, ViewY, ViewWidth, ViewHeight);

		fpsStep(&fps);

		//SFML
		glDrawMenuItems(G->SR, *G->Window, &G->GameMenu, ViewX, ViewY, ViewWidth, ViewHeight);


		//if (msgCanBeDrawn())
		//	glDrawMenuItems(*G->Window, msgGetMenu(), ViewX, ViewY, ViewWidth, ViewHeight);


		G->Window->display();

		//if (msgCanDisplay()) msgDisplay(*G->Window, ViewX, ViewY, ViewWidth, ViewHeight);
	}
	delAnimation(A);
}


