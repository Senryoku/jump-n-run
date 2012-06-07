#include "Game.h"

/* Fonctions d'usage interne **/
void gmShowEscapeMenu(Game* G);

void gmInit(Game* G, SharedResources* SR)
{
	Config Cfg = GetConfiguration();

	G->WindowWidth = Cfg.WindowWidth;
	G->WindowHeight = Cfg.WindowHeight;
	G->Window = new sf::RenderWindow(sf::VideoMode(G->WindowWidth, G->WindowHeight), "Jump n'Run", sf::Style::Default, sf::ContextSettings(32));
	G->WindowIsActive = TRUE;

	G->Window->setKeyRepeatEnabled(0);
	G->Window->setMouseCursorVisible(1);
	/* On ne peut utiliser  qu'une des deux */
	if(Cfg.VerticalSync != 0.f)
		G->Window->setVerticalSyncEnabled(1);
	else
		G->Window->setFramerateLimit((unsigned int) Cfg.FPSLimit);

	shSetWindowIcon(SR, *G->Window);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(Cfg.AntiAliasing != 0.f)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}

	G->Lvl = newLevel(0.f, 0.f);

	G->Time = 0;

	G->SR = SR;

	strcpy(G->Path, "");

	G->Window->setActive();
}

void gmFree(Game* G)
{
	if(G->Lvl != NULL) delLevel(G->Lvl);
	G->Window->setActive(0);
	G->Window->close();
	//delete G->Window; // Provoque une segfault sous Windows
}

Bool gmLoadLvl(Game* G, const char* Path)
{
	strcpy(G->Path, Path);
	return lvlLoad(G->Lvl, Path);
}

void gmMenu(Game* G)
{
	ItemID IID;
	msgCreateMessage(shMessageManager(G->SR), "JumpNRun", 4);
	msgAddCloseItem(shMessageManager(G->SR), "Select Level from List");
	IID = msgAddItem(shMessageManager(G->SR), "Level Path", ITEM_INPUT, NULL, NULL);
	mniSetInput(mnGetItem(msgGetMenu(shMessageManager(G->SR)), 0, IID), G->Path);
	msgAddCloseItem(shMessageManager(G->SR), "Play");
	msgAddCloseItem(shMessageManager(G->SR), "Quit");
	ItemID Choice = msgGetChoice(shMessageManager(G->SR), *G->Window, 0, 0, G->WindowWidth, G->WindowHeight);
	switch (Choice)
	{
		case 0:
		{
			std::vector<std::string> files;
			GetLevels("levels", files);

			msgCreateMessage(shMessageManager(G->SR), "Level List", (unsigned int)files.size()+1);
			for (int i=0; i<(int)files.size(); i++)
				msgAddCloseItem(shMessageManager(G->SR), files[i].c_str());

			msgAddCloseItem(shMessageManager(G->SR), "Cancel");

			Choice = msgGetChoice(shMessageManager(G->SR), *G->Window, 0.f, 0.f, G->WindowWidth, G->WindowHeight);

			if (Choice < (ItemID)files.size())
			{
				if(gmLoadLvl(G, ("levels/"+files[Choice]).c_str())) { lvlLoadedInit(G->Lvl); gmResetClk(G); } else { gmMenu(G); }
			}

			files.clear();
			break;
		}
		case 1 :
			break;
		case 2 :
			if(gmLoadLvl(G, msgGetLastInput(shMessageManager(G->SR)))) lvlLoadedInit(G->Lvl), gmResetClk(G); else gmMenu(G);
			break;
		case 3 :
			G->Window->close();
			break;
		default :
			break;
	}
}

void gmPlay(Game* G)
{
	Bool DispDebug = FALSE;
	Vec2 Center;
	Score Sc;
	FPSCounter fps;

	float ViewX = 0.f, ViewY = 0.f, MouseX = 0.f, MouseY = 0.f, ViewWidth = G->WindowWidth, ViewHeight = G->WindowHeight;

	if(G->Lvl == NULL) return;
	if (strcmp(G->Path, "") != 0) lvlLoadedInit(G->Lvl), gmResetClk(G); // Cas du lancement via l'éditeur
	while(strcmp(G->Path, "") == 0) gmMenu(G);

	fpsInit(&fps);
	while (G->Window->isOpen())
	{

		sf::Event event;

		while (G->Window->pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				G->Window->close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				gmShowEscapeMenu(G);

			if (event.type == sf::Event::Resized)
				printf("Resized ! %u, %u \n", event.size.width, event.size.height);

			if (event.type == sf::Event::LostFocus)
				G->WindowIsActive = FALSE;

			if (event.type == sf::Event::GainedFocus)
				G->WindowIsActive = TRUE;

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
					default:
						break;
				}
			}
		}

		lvlUpdate(G->Lvl, FALSE, G->SR);

		if (G->WindowIsActive)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				plJump(lvlGetP1(G->Lvl), G->SR);
			else
				plResetJump(lvlGetP1(G->Lvl));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				plGetUp(lvlGetP1(G->Lvl));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				plMoveL(lvlGetP1(G->Lvl));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				plMoveR(lvlGetP1(G->Lvl));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				plRotateR(lvlGetP1(G->Lvl));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				plRotateL(lvlGetP1(G->Lvl));
		}


		if(lvlIsGoalReached(G->Lvl))
		{
			gmPauseClk(G);
			lvlSetFinished(G->Lvl, 1);
			char Name[255];
			msgCreateMessage(shMessageManager(G->SR), "Congrat's !", 5);
			msgAddItem(shMessageManager(G->SR), "Player Name", ITEM_INPUT, NULL, NULL);
			msgAddCloseItem(shMessageManager(G->SR), "Send Score");
			msgAddCloseItem(shMessageManager(G->SR), "Restart Level");
			msgAddCloseItem(shMessageManager(G->SR), "Main Menu");
			msgAddCloseItem(shMessageManager(G->SR), "Quit");
			strcpy(Name, msgGetInput(shMessageManager(G->SR), *G->Window, ViewX, ViewY, ViewWidth, ViewHeight));
			ItemID Choice = msgGetLastChoice(shMessageManager(G->SR));
			switch (Choice)
			{
				case 0 :
					break;
				case 1 :
					scInit(&Sc, Name, lvlGetName(G->Lvl), lvlGetMD5(G->Lvl), G->Time);
					if(scSend(&Sc) == 1) { printf("Error sending score.\n"); }
					scFree(&Sc);
					// Pas de break exprès
				case 2 :
					if(gmReloadLevel(G)) lvlLoadedInit(G->Lvl), gmResetClk(G); else gmShowEscapeMenu(G);
					break;
				case 3:
					gmMenu(G);
					break;
				case 4 :
					G->Window->close();
					break;
				default :
					gmMenu(G);
					break;
			}
		}


		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Center = polyComputeCenter(plGetShape(lvlGetP1(G->Lvl)));
		ViewX = MAX(0, MIN(Center.x - ViewWidth/2, wdGetWidth(lvlGetWorld(G->Lvl)) - ViewWidth));
		ViewY = MAX(0, MIN(Center.y - ViewHeight/2, wdGetHeight(lvlGetWorld(G->Lvl)) - ViewHeight));
		glOrtho(ViewX, ViewX + ViewWidth, ViewY + ViewHeight, ViewY, 0.0, 100.0);


		lvlDisplayBG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight);
		lvlDisplayL1(G->Lvl);
		lvlDispGoalFlag(G->Lvl);
		lvlDispAllObj(G->Lvl);
		lvlDisplayElastics(G->Lvl, G->SR);
		lvlDisplayRigids(G->Lvl, G->SR);

		lvlDisplayPlayer(G->Lvl, G->SR);

		lvlDisplayGrass(G->Lvl, G->SR);

		lvlDisplayL2(G->Lvl);
		lvlDisplayFG(G->Lvl, ViewX, ViewY, ViewWidth, ViewHeight);

		sndmUpdate(shSoundManager(G->SR));

		if(DispDebug)
		{
			glDrawPolygon(plGetShape(lvlGetP1(G->Lvl)));
			wdDraw(lvlGetWorld(G->Lvl), &glDrawVertex, &glDrawElastic, &glDrawRigid, &glDrawPolygon);
		}

		fpsStep(&fps);

		MouseX = ViewWidth*sf::Mouse::getPosition(*G->Window).x/G->WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(*G->Window).y/G->WindowHeight + ViewY;

		if (DispDebug)
			glDrawFPS(G->SR, *G->Window, fpsGetString(&fps));

		glDrawTime(G->SR, *G->Window, G->Time + G->Clk.getElapsedTime().asMilliseconds()/10.f);

		glDrawCursor(*G->Window,ViewWidth, ViewHeight, sf::Mouse::getPosition(*G->Window).x, sf::Mouse::getPosition(*G->Window).y, shGetCursorSprite(G->SR, SPR_CURSOR));

		G->Window->display();
	}
}

void gmShowEscapeMenu(Game* G)
{
	gmPauseClk(G);
	msgCreateMessage(shMessageManager(G->SR), "Menu", 5);
	msgAddCloseItem(shMessageManager(G->SR), "Main Menu");
	msgAddCloseItem(shMessageManager(G->SR), "Restart Level");
	msgAddCloseItem(shMessageManager(G->SR), "Show Scores");
	msgAddCloseItem(shMessageManager(G->SR), "Quit");
	msgAddCloseItem(shMessageManager(G->SR), "Return");
	ItemID Choice = msgGetChoice(shMessageManager(G->SR), *G->Window, G->WindowWidth, G->WindowHeight, G->WindowWidth, G->WindowHeight);
	switch (Choice)
	{
		case 0 :
			gmMenu(G);
			break;
		case 1 :
			if(gmReloadLevel(G)) lvlLoadedInit(G->Lvl), gmResetClk(G); else gmShowEscapeMenu(G);
			break;
		case 2 :
			gmShowScores(G);
			break;
		case 3 :
			G->Window->close();
			break;
		default :
			break;
	}
	gmRestartClk(G);
}

void gmResetClk(Game* G)
{
	G->Time = 0;
	G->Clk.restart();
}

void gmPauseClk(Game* G)
{
	G->Time += G->Clk.getElapsedTime().asMilliseconds()/10.f;
}

void gmRestartClk(Game* G)
{
	G->Clk.restart();
}

Bool gmReloadLevel(Game* G)
{
	return gmLoadLvl(G, G->Path);
}

void gmShowScores(Game* G)
{
	DynArr *DA = scCollect(lvlGetName(G->Lvl), lvlGetMD5(G->Lvl));
	Score* sc;
	char Text[1024];

	if (daGetSize(DA) == 0)
	{
		msgCreateMessage(shMessageManager(G->SR), "Scores", 2);
		msgAddItem(shMessageManager(G->SR), "There are no scores for this level!", ITEM_LABEL, NULL, NULL);
		msgAddCloseItem(shMessageManager(G->SR), "Ok");

		msgDisplay(shMessageManager(G->SR), *G->Window, 0.f, 0.f, G->WindowWidth, G->WindowHeight);

		scCollectFree(DA);
		return;
	}
	
	unsigned int maxScores = MIN(daGetSize(DA), 20);

	msgCreateMessage(shMessageManager(G->SR), "Scores", maxScores+1);

	for (unsigned int i=0; i< maxScores; i++)
	{
		sc = (Score*)daGet(DA, i);
		std::ostringstream oss;
		oss << "Time : ";
		if(sc->Time/6000 < 10) oss << "0";
		oss << sc->Time/6000 << ":";
		if((sc->Time/100)%60 < 10) oss << "0";
		oss << (sc->Time/100)%60 << ":";
		if(sc->Time%100 < 10) oss << "0";
		oss << sc->Time%100;
		sprintf(Text, "%u. %s    %s",i+1, sc->Player, oss.str().c_str());
		msgAddItem(shMessageManager(G->SR), Text, ITEM_LABEL, NULL, NULL);
	}

	msgAddCloseItem(shMessageManager(G->SR), "Close");
	scCollectFree(DA);
	msgDisplay(shMessageManager(G->SR), *G->Window, 0.f, 0.f, G->WindowWidth, G->WindowHeight);

}
