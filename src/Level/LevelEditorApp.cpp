#include <Level/LevelEditorApp.h>

void appInit(LevelEditorApp* App)
{
	appWindowInit(App);

	lvledInit(&App->Led, 4000.f, 1600.f);
	lvledSetLineDraw(&App->Led, &glDrawLine);
	lvledSetVxDraw(&App->Led, &glDrawVertex);
	lvledSetElDraw(&App->Led, &glDrawElastic);
	lvledSetRdDraw(&App->Led, &glDrawRigid);
	lvledSetPolyDraw(&App->Led, &glDrawPolygon);
	lvledLoad(&App->Led, "levels/tmpEditor.lvl");
	strcpy(App->WorkingPath, "levels/tmpEditor.lvl");
}

void appWindowInit(LevelEditorApp* App)
{
	Config Cfg = GetConfiguration();

	App->WindowWidth = Cfg.WindowWidth;
	App->WindowHeight = Cfg.WindowHeight;
	App->Window = new sf::RenderWindow(sf::VideoMode(App->WindowWidth, App->WindowHeight), "Jump n'Run Level Editor", sf::Style::Default, sf::ContextSettings(32));

	App->Window->setKeyRepeatEnabled(0);
	App->Window->setMouseCursorVisible(1);
	/* On ne peut utiliser  qu'une des deux */
	if(Cfg.VerticalSync == 1.f)
		App->Window->setVerticalSyncEnabled(1);
	else
		App->Window->setFramerateLimit((unsigned int) Cfg.FPSLimit);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(Cfg.AntiAliasing == 1.f) glEnable(GL_LINE_SMOOTH);
}

void appWindowFree(LevelEditorApp* App)
{
	App->Window->close();
	delete App->Window;
}

void appFree(LevelEditorApp* App)
{
	lvledFree(&App->Led);
	appWindowFree(App);
}

void appRun(LevelEditorApp* App)
{
	unsigned int ViewPort, frames = 0;
	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, MapWidth = App->WindowWidth/10.f, MapHeight = App->WindowHeight/10.f,
		OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
		ViewXSpeed = 0.f, ViewYSpeed = 0.f, ViewWidth = App->WindowWidth, ViewHeight = App->WindowHeight,
		WindowRatio = App->WindowWidth/App->WindowHeight, FPS = 60.f;
	sf::Clock Clock;
	Bool L1 = FALSE;

	while (App->Window->isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(*App->Window).x/App->WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(*App->Window).y/App->WindowHeight + ViewY;

		sf::Event event;
		while (App->Window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return;

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return; /** @todo Faire apparaitre un menu ici **/

			if (event.type == sf::Event::Resized)
				printf("Resized ! %u, %u \n", event.size.width, event.size.height);

			if(event.type == sf::Event::MouseButtonPressed)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left :
						/* Création d'un polygone Fixe */
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
							lvledNewPolyFixeAddV(&App->Led);
						/* Création d'un polygone Dynamique */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
							lvledNewPolyAddV(&App->Led);
						/* Ajout d'Elastic */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
							lvledNewElasticAddV(&App->Led);
						/* Ajout de Rigid */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
							lvledNewRigidAddV(&App->Led);
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							lvledNewBoxInit(&App->Led);
						} else {
							lvledGrab(&App->Led);
						}
						break;
					case sf::Mouse::Right :
						/** @todo Si une autre touche est appuyé avec le clic droit
						 * (ex : E pour Elastic), faire appel à wdGetNearestElastic
						 * et faire apparaître un menu pour l'édition de cet Elastic **/
						lvledGrabEl(&App->Led);
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
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							lvledNewBoxCreate(&App->Led);
						} else {
							lvledRelease(&App->Led);
						}
						break;
					case sf::Mouse::Right :
						lvledReleaseEl(&App->Led);
						break;
					default :
						break;
				}
			}

			if(event.type == sf::Event::MouseWheelMoved)
			{
				ViewWidth -= event.mouseWheel.delta*WindowRatio*20;
				ViewHeight -= event.mouseWheel.delta*20;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::T :
						appWindowFree(App);
						lvledTestLevel(&App->Led);
						appWindowInit(App);
						break;
					case sf::Keyboard::Space :
						ViewWidth = App->WindowWidth;
						ViewHeight = App->WindowHeight;
						break;
					case sf::Keyboard::B :
					{
						float boxSize = 100.f;
						Vertex *V10, *V11, *V12, *V13;
						V10 = newVertex();
						vxSetPosition(V10, vec2(MouseX, MouseY));
						V11 = newVertex();
						vxSetPosition(V11, vec2(MouseX+boxSize, MouseY));
						V12 = newVertex();
						vxSetPosition(V12, vec2(MouseX+boxSize, MouseY+boxSize));
						V13 = newVertex();
						vxSetPosition(V13, vec2(MouseX, MouseY+boxSize));
						wdAddVertex(lvlGetWorld(App->Led.Lvl), V10); wdAddVertex(lvlGetWorld(App->Led.Lvl), V11); wdAddVertex(lvlGetWorld(App->Led.Lvl), V12); wdAddVertex(lvlGetWorld(App->Led.Lvl), V13);
						Polygon* Rectangle2 = polyRectangle(V10, V11, V12, V13);
						wdAddPolygon(lvlGetWorld(App->Led.Lvl), Rectangle2);
					}
						break;
					case sf::Keyboard::G :
						lvledGrab(&App->Led);
						break;
					case sf::Keyboard::N :
						printf("Number of Vertices: %u\nNumber of Polygons: %u\n", lstCount(&lvlGetWorld(App->Led.Lvl)->Vertices), lstCount(&lvlGetWorld(App->Led.Lvl)->Polygons));
						break;
					case sf::Keyboard::E :
						lvledGrabEl(&App->Led);
						break;
					case sf::Keyboard::Delete :
						if(!event.key.shift)
						{
							lvledDelPoly(&App->Led);
						} else {
							lvledDelVertex(&App->Led);
						}
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lvledNewPolyFixeInit(&App->Led);
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lvledNewPolyInit(&App->Led);
						break;
					case sf::Keyboard::S :
						if (event.key.control)
							lvledSave(&App->Led, App->WorkingPath);
						break;
					case sf::Keyboard::L :
						if (event.key.control)
							lvledLoad(&App->Led, App->WorkingPath);
						break;
					case sf::Keyboard::X :
						plGetUp(App->Led.Lvl->P1);
						break;
					case sf::Keyboard::V :
						L1 = !L1;
						break;
					default:
						break;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::G :
						lvledRelease(&App->Led);
						break;
					case sf::Keyboard::E :
						lvledReleaseEl(&App->Led);
						break;
					case sf::Keyboard::F :
						lvledToogleNearestFixe(&App->Led);
						break;
					case sf::Keyboard::C :
						lvledToogleNearestPolyFixe(&App->Led);
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lvledNewPolyCreate(&App->Led);
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lvledNewPolyFixeCreate(&App->Led);
						break;
					case sf::Keyboard::Num3 :
						lvledNewElasticCreate(&App->Led);
						break;
					case sf::Keyboard::Num4 :
						lvledNewRigidCreate(&App->Led);
						break;
					default:
						break;
				}
			}
		}

		lvledSetMousePosition(&App->Led, MouseX, MouseY);
		lvledGrabUpdate(&App->Led);
		lvledNewBoxUpdate(&App->Led);

		/* Déplacement de la vue */
		(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? ViewSpeed = 30.f : ViewSpeed = 15.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			toViewY-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			toViewY+=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			toViewX-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			toViewX+=ViewSpeed;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			toViewX += (OldMouseX - MouseX)*10.f;
			toViewY += (OldMouseY - MouseY)*10.f;
		}

		Wobble(&ViewX, toViewX, 0.5f, 0.5f, &ViewXSpeed);
		Wobble(&ViewY, toViewY, 0.5f, 0.5f, &ViewYSpeed);

		/* == Mise à jour du niveau == */
		lvlUpdate(App->Led.Lvl);

		/* == Affichage == */

		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		glClearColor(0.0f, 0.f, 0.f, 1.f); //Ici optionnel car par défaut couleur est rouge

		for(ViewPort = 0; ViewPort < 2; ViewPort++)
		{
			glLoadIdentity();
			if(ViewPort == 0)
			{
				/* Où afficher ? */
				glViewport(0.f, 0.f, App->WindowWidth, App->WindowHeight);
				/* Quelle portion de la scène afficher ? */
				glOrtho(0.f+ViewX, ViewWidth+ViewX, ViewHeight+ViewY, 0.f+ViewY, 0.0, 100.0);

				if(L1)
					lvlDisplayL1(App->Led.Lvl);

				/* Affichage de la Grille */
				gridDraw(&lvlGetWorld(App->Led.Lvl)->CollisionGrid);
			}
			else if(ViewPort == 1)
			{
				glViewport(App->WindowWidth - MapWidth - 10.f, App->WindowHeight - MapHeight - 10.f, MapWidth, MapHeight);
				/* La minimap affiche tout le monde */
				glOrtho(0.0, lvlGetWorld(App->Led.Lvl)->Width, lvlGetWorld(App->Led.Lvl)->Height, 0.0, 0.0, 100.0);

				/* Rectangle de la vue */
				glColor4f(0.5f, 0.5f, 0.5f, 1.f);
				glLineStipple(1, 0xCCCC);
				glEnable(GL_LINE_STIPPLE);
				glBegin(GL_LINE_LOOP);
					glVertex2f(ViewX, ViewY);
					glVertex2f(ViewX + ViewWidth, ViewY);
					glVertex2f(ViewX + ViewWidth, ViewY + ViewHeight);
					glVertex2f(ViewX, ViewY + ViewHeight);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
			}

			lvledDraw(&App->Led, LVLED_RULE | LVLED_LIMITS);
		}

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		// Update the App->Window
		App->Window->display();
		frames++;
		if (Clock.getElapsedTime().asSeconds()>=1.f)
		{
			FPS = frames/Clock.getElapsedTime().asSeconds();
			printf("FPS: %.1f\n", round(FPS));
			frames = 0;
			Clock.restart();
		}
	}
}

void appSetWorkingPath(LevelEditorApp* App, const char* Path)
{
	strcpy(App->WorkingPath, Path);
}