#include <Level/LevelEditorApp.h>
#include <Objects/Animation.h>


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
	App->WindowIsActive = TRUE;
	sndmInit();

	/*
	sndmLoadMusicFile("music0", "data/music.ogg");
	sndmLoadMusicFile("music1", "data/music1.ogg");
	sndmLoadMusicFile("music2", "data/music2.ogg");
	sndmLoadMusicFile("music3", "data/music3.ogg");
	sndmLoadSoundFile("meat", "data/sfx/snd_meat.ogg");


	sndmPlayMusic("music3");
	 */

	//sndmPlay("meat");
	//Temporel
	txBoxCorner = glTexLoad("data/s_boxcorner.png");
	txBoxSide = glTexLoad("data/s_boxside.png");
	txBoxShadow = glTexLoad("data/s_box_shadow.png");
	txBoxBackAnim = glTexLoad("data/box_anim_strip20.png");
	txBoxGloss = glTexLoad("data/s_box_gloss.png");
	FntMenu.loadFromFile("data/fnt_menu.ttf");

	App->MenuUsed = 0;
}

void appWindowInit(LevelEditorApp* App)
{
	Config Cfg = GetConfiguration();

	App->WindowWidth = Cfg.WindowWidth;
	App->WindowHeight = Cfg.WindowHeight;
	App->Window.create(sf::VideoMode(App->WindowWidth, App->WindowHeight), "Jump n'Run Level Editor", sf::Style::Default, sf::ContextSettings(32));

	App->Window.setKeyRepeatEnabled(0);
	App->Window.setMouseCursorVisible(1);
	/* On ne peut utiliser  qu'une des deux */
	if(Cfg.VerticalSync == 1.f)
		App->Window.setVerticalSyncEnabled(1);
	else
		App->Window.setFramerateLimit((unsigned int) Cfg.FPSLimit);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glEnable(GL_ALPHA_TEST) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(Cfg.AntiAliasing == 1.f) glEnable(GL_LINE_SMOOTH);

}


void appFree(LevelEditorApp* App)
{
	sndmFree();
	lvledFree(&App->Led);
	App->Window.close();
	glTexFree(txBoxSide);
	glTexFree(txBoxCorner);
	glTexFree(txBoxShadow);
	glTexFree(txBoxBackAnim);
	glTexFree(txBoxGloss);
}

void appRun(LevelEditorApp* App)
{
	unsigned int ViewPort;
	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, MapWidth = App->WindowWidth/10.f, MapHeight = App->WindowHeight/10.f,
		OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
		ViewXSpeed = 0.f, ViewYSpeed = 0.f, ViewWidth = App->WindowWidth, ViewHeight = App->WindowHeight,
		WindowRatio = App->WindowWidth/App->WindowHeight;
	Bool Paused = TRUE, DispDebug = TRUE, DispL1 = FALSE, DispL2 = FALSE, DispObjects = FALSE;//, DispBack = FALSE, DispFore = FALSE;
	FPSCounter fps;

//	int clothSize = 15;
//	Cloth* C = newCloth(lvlGetWorld(App->Led.Lvl), CLOTH_RIGID, clothSize, clothSize, 10.f, 10.f);
//	//clSetPointsMass(C, 0.01f);
//	vxSetFixe(clGetVertex(C, 0, 0), 1);
//	vxSetFixe(clGetVertex(C, clothSize-1, 0), 1);
//	vxSetFixe(clGetVertex(C, 0, clothSize-1), 1);
//	vxSetFixe(clGetVertex(C, clothSize-1, clothSize-1), 1);
//	Texture Tx = glTexLoad("data/trollface.jpg");

	//unsigned int CurrentMusic = 0;

	/* Code temporel permettant de créer des states d'animation */

	/*Vertex* Neck = newVertex(), *HeadLeft = newVertex(), * HeadRight = newVertex(), * Base = newVertex(), * LeftArm1 = newVertex(), * LeftArm2 = newVertex(), * RightArm1 = newVertex(), * RightArm2 = newVertex(), * LeftLeg1 = newVertex(), * LeftLeg2 = newVertex(), * RightLeg1 = newVertex(), * RightLeg2 = newVertex();

	vxSetPosition(Base, vec2(150.f, 330.f));
	Vec2 B = vxGetPosition(Base);
	vxSetPosition(Neck, vec2(B.x, B.y - 90.f));
	vxSetPosition(HeadLeft, vec2Add(vxGetPosition(Neck), vec2(-30.f, -40.f)));
	vxSetPosition(HeadRight, vec2Add(vxGetPosition(Neck), vec2(30.f, -40.f)));
	vxSetPosition(LeftArm1, vec2Add(vxGetPosition(Neck), vec2(0.f, 35.f)));
	vxSetPosition(LeftArm2, vec2Add(vxGetPosition(Neck), vec2(0.f, 70.f)));
	vxSetPosition(RightArm1, vec2Add(vxGetPosition(Neck), vec2(0.f, 35.f)));
	vxSetPosition(RightArm2, vec2Add(vxGetPosition(Neck), vec2(0.f, 70.f)));
	vxSetPosition(LeftLeg1, vec2Add(vxGetPosition(Base), vec2(0.f, 40.f)));
	vxSetPosition(LeftLeg2, vec2Add(vxGetPosition(Base), vec2(10.f, 80.f)));
	vxSetPosition(RightLeg1, vec2Add(vxGetPosition(Base), vec2(0.f, 40.f)));
	vxSetPosition(RightLeg2, vec2Add(vxGetPosition(Base), vec2(-10.f, 80.f)));


	vxSetFixe(Neck, TRUE);
	vxSetFixe(Base, TRUE);
	vxSetFixe(LeftArm1, TRUE);
	vxSetFixe(LeftArm2, TRUE);
	vxSetFixe(LeftLeg1, TRUE);
	vxSetFixe(LeftLeg2, TRUE);
	vxSetFixe(RightArm1, TRUE);
	vxSetFixe(RightArm2, TRUE);
	vxSetFixe(RightLeg1, TRUE);
	vxSetFixe(RightLeg2, TRUE);
	vxSetFixe(HeadLeft, TRUE);
	vxSetFixe(HeadRight, TRUE);
	 */

	/*wdAddVertex(App->Led.Lvl->W, Base);
	wdAddVertex(App->Led.Lvl->W, Neck);
	wdAddVertex(App->Led.Lvl->W, HeadLeft);
	wdAddVertex(App->Led.Lvl->W, HeadRight);
	wdAddVertex(App->Led.Lvl->W, LeftArm1);
	wdAddVertex(App->Led.Lvl->W, LeftArm2);
	wdAddVertex(App->Led.Lvl->W, LeftLeg1);
	wdAddVertex(App->Led.Lvl->W, LeftLeg2);
	wdAddVertex(App->Led.Lvl->W, RightArm1);
	wdAddVertex(App->Led.Lvl->W, RightArm2);
	wdAddVertex(App->Led.Lvl->W, RightLeg1);
	wdAddVertex(App->Led.Lvl->W, RightLeg2);

	Rigid *LA1, *LA2, *RA1, *RA2, *Body, *LL1, *LL2, *RL1, *RL2, *H1, *H2, *H3;
	LA1 = newRigid(Neck, LeftArm1, -1.f);
	LA2 = newRigid(LeftArm1, LeftArm2, -1.f);
	RA1 = newRigid(Neck, RightArm1, -1.f);
	RA2 = newRigid(RightArm1, RightArm2, -1.f);

	LL1 = newRigid(Base, LeftLeg1, -1.f);
	LL2 = newRigid(LeftLeg1, LeftLeg2, -1.f);
	RL1 = newRigid(Base, RightLeg1, -1.f);
	RL2 = newRigid(RightLeg1, RightLeg2, -1.f);


	Body = newRigid(Base, Neck, -1.f);

	H1 = newRigid(Neck, HeadLeft, -1.f);
	H2 = newRigid(Neck, HeadRight, -1.f);
	H3 = newRigid(HeadLeft, HeadRight, -1.f);


	wdAddRigid(App->Led.Lvl->W, Body);
	wdAddRigid(App->Led.Lvl->W, LA1);
	wdAddRigid(App->Led.Lvl->W, LA2);
	wdAddRigid(App->Led.Lvl->W, RA1);
	wdAddRigid(App->Led.Lvl->W, RA2);
	wdAddRigid(App->Led.Lvl->W, LL1);
	wdAddRigid(App->Led.Lvl->W, LL2);
	wdAddRigid(App->Led.Lvl->W, RL1);
	wdAddRigid(App->Led.Lvl->W, RL2);
	wdAddRigid(App->Led.Lvl->W, H1);
	wdAddRigid(App->Led.Lvl->W, H2);
	wdAddRigid(App->Led.Lvl->W, H3);*/

	/* Fin du code temporel pour les states des animations */
	fpsInit(&fps);
	while (App->Window.isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(App->Window).x/App->WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(App->Window).y/App->WindowHeight + ViewY;

		sf::Event event;
		while (App->Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return;

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return; /** @todo Faire apparaitre un menu ici **/

			if (event.type == sf::Event::LostFocus)
				App->WindowIsActive = FALSE;
			if (event.type == sf::Event::GainedFocus)
				App->WindowIsActive = TRUE;

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
						//lvledGrabEl(&App->Led);

						if(App->MenuUsed) mnFree(&App->M);

						mnInit(&App->M);
						mnSetItemSelectedZoomFactor(&App->M, 1.f);
						mnSetItemNormalZoomFactor(&App->M, 0.75f);
						mnAddItem(&App->M, 0, "Input a real", ITEM_INPUT_VALUE, NULL, NULL);

						App->MenuUsed = 1;
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
							lvledRelease(&App->Led, Paused);
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
						App->Window.close();
						lvledTestLevel(&App->Led);
						appWindowInit(App);
						break;
					case sf::Keyboard::J :
						lvledSetSpawn(&App->Led);
						break;
					case sf::Keyboard::K :
						lvledSetGoal(&App->Led);
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
						else DispDebug = !DispDebug;
						break;
					case sf::Keyboard::V :
						DispL1 = !DispL1;
						break;
					case sf::Keyboard::M :
						DispObjects = !DispObjects;
						break;
					case sf::Keyboard::Comma :
						Paused = !Paused;
						break;

					/* code de sauvegarde de l'animation */
				/*case sf::Keyboard::A :
						FILE* f; Vertex* V; Vec2 Pos; float Angle;
						f = fopen("animPos.txt", "w");
						//[nb of states] [AnimType] [Triggers]
						fprintf(f, "%u %u %u\n", 1, (unsigned int)ANIM_POSITIONS, (unsigned int)ANIM_ALL_TRIGGERS);
						//[ANIM_FREE?] [x] [y] #Part of the body
						V = Neck;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "Neck");

						Pos = vec2Prod(vec2Add(vec2Add(vxGetPosition(HeadLeft), vxGetPosition(HeadRight)), vxGetPosition(Neck)), 1.f/3.f);
						Pos = vec2Sub(Pos, vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", 0, Pos.x, 0, Pos.y, "Head");

						V = LeftArm1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "LeftArm1");
						V = LeftArm2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "LeftArm2");
						V = RightArm1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "RightArm1");
						V = RightArm2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "RightArm2");
						V = LeftLeg1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "LeftLeg1");
						V = LeftLeg2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "LeftLeg2");
						V = RightLeg1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "RightLeg1");
						V = RightLeg2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)!vxIsFixe(V), Pos.x, (unsigned int)!vxIsFixe(V), Pos.y, "RightLeg2");

						fclose(f);

						f = fopen("animAngles.txt", "w");
						Vertex* V1, *V2;
						//[nb of states] [AnimType]
						fprintf(f, "%u %u %u\n", 1, (unsigned int)ANIM_ANGLES, (unsigned int)ANIM_ALL_TRIGGERS);


						V1 = Neck; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "Neck");

						V1 = HeadLeft; V2 = HeadRight;
						Pos = vec2Prod(vec2Add(vec2Add(vxGetPosition(HeadLeft), vxGetPosition(HeadRight)), vxGetPosition(Neck)), 1.f/3.f);
						Pos = vec2Sub(Pos, vxGetPosition(Neck));
						Angle = vec2Angle(Pos);
						fprintf(f, "%u %f #%s\n", (unsigned int)(!vxIsFixe(V1) || !vxIsFixe(V2)), RAD2DEG(Angle), "Head");

						V1 = LeftArm1; V2 = Neck;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "LeftArm1");

						V1 = LeftArm2; V2 = LeftArm1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "LeftArm2");

						V1 = RightArm1; V2 = Neck;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "RightArm1");
						V1 = RightArm2; V2 = RightArm1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "RightArm2");

						V1 = LeftLeg1; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "LeftLeg1");
						V1 = LeftLeg2; V2 = LeftLeg1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "LeftLeg2");

						V1 = RightLeg1; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "RightLeg1");
						V1 = RightLeg2; V2 = RightLeg1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)!vxIsFixe(V1), RAD2DEG(Angle), "RightLeg2");

						fclose(f);
						break;
>>>>>>> .r230
				 */
					default:
						break;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::G :
						lvledRelease(&App->Led, Paused);
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
		if (App->WindowIsActive)
		{
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
		}


		Wobble(&ViewX, toViewX, 0.5f, 0.5f, &ViewXSpeed);
		Wobble(&ViewY, toViewY, 0.5f, 0.5f, &ViewYSpeed);

		/* == Mise à jour du niveau == */
		lvlUpdate(App->Led.Lvl, Paused);

		sndmUpdate();

		/*
		 //Ça c'est la façon manuelle, j'ai cependant rajouté dans SoundManager des trucs pour faire que ça se fasse seul. Je rajouterai d'autre choses pour le personaliser un peu plus tard
		if (!sndmIsInFading() && sndmGetPlayCount() > 0)
		{
			CurrentMusic++;
			if (CurrentMusic>3)
				CurrentMusic = 0;
			char Key[30];
			sprintf(Key, "music%u", CurrentMusic);
			sndmMusicFade(Key, 2.f);
		}
		 */

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

				/* Affichage de la Grille */
				if(DispDebug) gridDraw(&lvlGetWorld(App->Led.Lvl)->CollisionGrid);
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

			if(DispL1) lvlDisplayL1(App->Led.Lvl);
			if(DispL2) lvlDisplayL2(App->Led.Lvl);
			lvlDispGoalFlag(App->Led.Lvl);
			if(DispObjects) lvlDispAllObj(App->Led.Lvl);
//			glDrawCloth(C, Tx);
			if(DispDebug) lvledDraw(&App->Led, LVLED_RULE | LVLED_LIMITS);
			/* je n'arrive pas à afficher les fps en faisant marcher la vue. J'ai pas tenté trop de trucs mais bon xD */
			//if (ViewPort == 0) glDrawFPS(App->Window, fpsGetString(&fps));
			glDrawPolyFromList(&App->Led.tmpLstDyn, vec2(MouseX, MouseY));
			glDrawPolyFromList(&App->Led.tmpLstFixe, vec2(MouseX, MouseY));
		}

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		if(App->MenuUsed)
		{
			mnUpdate(&App->M, vec2(100.f, 100.f), vec2(100.f, -mnGetHeight(&App->M) - 100.f));
			glDrawMenu(App->Window, &App->M, ViewX, ViewY);
		}
		//glDrawFPS(App->Window, fpsGetString(&fps));

		// Update the App->Window
		App->Window.display();
		fpsStep(&fps);
		if (fpsChanged(&fps))
			printf("FPS: %s\n", fpsGetChar(&fps));
	}
//	delCloth(C);
//	glTexFree(Tx);
	/*
	delVertex(Neck);
	delVertex(Base);
	delVertex(HeadLeft);
	delVertex(HeadRight);
	delVertex(LeftArm1);
	delVertex(LeftArm2);
	delVertex(RightArm1);
	delVertex(RightArm2);
	delVertex(LeftLeg1);
	delVertex(LeftLeg2);
	delVertex(RightLeg1);
	delVertex(RightLeg2);
	 */
}

void appSetWorkingPath(LevelEditorApp* App, const char* Path)
{
	strcpy(App->WorkingPath, Path);
}
