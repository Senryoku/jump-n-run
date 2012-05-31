#include <Level/LevelEditorApp.h>
#include <Objects/Animation.h>
#include <Game/Message.h>

// Fonctions d'usage interne uniquement
void appShowEscapeMenu(LevelEditorApp* App);
void appShowLevelMenu(LevelEditorApp* App);
void appShowVertexMenu(LevelEditorApp* App);
void appShowRigidMenu(LevelEditorApp* App);
void appShowElasticMenu(LevelEditorApp* App);
void appShowPolygonMenu(LevelEditorApp* App);

void appInit(LevelEditorApp* App, SharedResources* SR)
{
	App->ViewX = 0.f;
	App->ViewY = 0.f;
	App->ViewWidth = -1.f;
	App->ViewHeight = -1.f;
	App->SR = SR;

	appWindowInit(App);

	lvledInit(&App->Led, 4000.f, 1600.f, SR);
	lvledSetLineDraw(&App->Led, &glDrawLine);
	lvledSetVxDraw(&App->Led, &glDrawVertex);
	lvledSetElDraw(&App->Led, &glDrawElastic);
	lvledSetRdDraw(&App->Led, &glDrawRigid);
	lvledSetPolyDraw(&App->Led, &glDrawPolygon);
	lvledLoad(&App->Led, "levels/tmpEditor.lvl");
	strcpy(App->WorkingPath, "levels/tmpEditor.lvl");
	App->WindowIsActive = TRUE;



	/*
	sndmLoadMusicFile(shSoundManager(App->SR), "music0", "data/music.ogg");
	sndmLoadMusicFile(shSoundManager(App->SR), "music1", "data/music1.ogg");
	sndmLoadMusicFile(shSoundManager(App->SR), "music2", "data/music2.ogg");
	sndmLoadMusicFile(shSoundManager(App->SR), "music3", "data/music3.ogg");
	sndmLoadSoundFile(shSoundManager(App->SR), "meat", "data/sfx/snd_meat.ogg");


	sndmPlayMusic(shSoundManager(App->SR), "music3");
	 */

	//sndmPlay("meat");
	//Temporel

	App->MenuUsed = 0;
}

void appWindowInit(LevelEditorApp* App)
{
	Config Cfg = GetConfiguration();

	App->WindowWidth = Cfg.WindowWidth;
	App->WindowHeight = Cfg.WindowHeight;
	App->Window.create(sf::VideoMode(App->WindowWidth, App->WindowHeight), "Jump n'Run Level Editor", sf::Style::Close, sf::ContextSettings(32));

	if (App->ViewWidth<0.f)
	{
		App->ViewWidth = App->WindowWidth;
		App->ViewHeight = App->WindowHeight;
	}

	App->Window.setKeyRepeatEnabled(0);
	App->Window.setMouseCursorVisible(0);
	/* On ne peut utiliser  qu'une des deux */
	if(Cfg.VerticalSync != 0.f)
		App->Window.setVerticalSyncEnabled(1);
	else
		App->Window.setFramerateLimit((unsigned int) Cfg.FPSLimit);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glEnable(GL_ALPHA_TEST) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(Cfg.AntiAliasing != 0.f)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}
}


void appFree(LevelEditorApp* App)
{
	lvledFree(&App->Led);
	App->Window.close();
}

void appRun(LevelEditorApp* App)
{
	float ViewSpeed,
		 MouseX, MouseY, toViewX = App->ViewX, toViewY = App->ViewY,
		ViewXSpeed = 0.f, ViewYSpeed = 0.f,
		WindowRatio = App->WindowWidth/App->WindowHeight, MouseWinX, MouseWinY, MiniMapScale = 0.05f, MouseMiniMapDragX = 0.f, MouseMiniMapDragY = 0.f, MouseDragX, MouseDragY;
	Bool Paused = TRUE, DispDebug = TRUE, DispL1 = TRUE, DispL2 = TRUE, DispObjects = TRUE, DispBack = FALSE, DispFore = FALSE, InsideMiniMap = FALSE, DragMiniMap = FALSE;
	FPSCounter fps;


	Menu* M = (Menu*)malloc(sizeof(Menu));
	mnInit(M);


	mnAddMenu(M, "Main Menu", 2);
	mnAddMenu(M, "menu 2!", 2);
	mnAddItem(M, 0, "Item 1", ITEM_BUTTON, NULL, NULL);
	mnAddItemMenuSwitcher(M, 0, "goo", 1);
	mnAddItem(M, 1, "hooo", ITEM_LABEL, NULL, NULL);
	mnAddItemMenuSwitcher(M, 1, "gooo2", 0);
	mnSetHide(M, TRUE);

	//unsigned int CurrentMusic = 0;

	/* Code temporel permettant de créer des states d'animation */

	/*Vertex* Neck = newVertex(), *HeadLeft = newVertex(), * HeadRight = newVertex(), * Base = newVertex(), * LeftArm1 = newVertex(), * LeftArm2 = newVertex(), * RightArm1 = newVertex(), * RightArm2 = newVertex(), * LeftLeg1 = newVertex(), * LeftLeg2 = newVertex(), * RightLeg1 = newVertex(), * RightLeg2 = newVertex();

	vxSetPosition(Base, vec2(150.f, 330.f));
	Vec2 B = vxGetPosition(Base);
	vxSetPosition(Neck, vec2(B.x, B.y - 60.f));
	vxSetPosition(HeadLeft, vec2Add(vxGetPosition(Neck), vec2(-20.f, -20.f)));
	vxSetPosition(HeadRight, vec2Add(vxGetPosition(Neck), vec2(20.f, -20.f)));
	vxSetPosition(LeftArm1, vec2Add(vxGetPosition(Neck), vec2(25.f, 0.f)));
	vxSetPosition(LeftArm2, vec2Add(vxGetPosition(Neck), vec2(50.f, 0.f)));
	vxSetPosition(RightArm1, vec2Add(vxGetPosition(Neck), vec2(-25.f, 0.f)));
	vxSetPosition(RightArm2, vec2Add(vxGetPosition(Neck), vec2(-50.f, 0.f)));
	vxSetPosition(LeftLeg1, vec2Add(vxGetPosition(Base), vec2(0.f, 30.f)));
	vxSetPosition(LeftLeg2, vec2Add(vxGetPosition(Base), vec2(0.f, 60.f)));
	vxSetPosition(RightLeg1, vec2Add(vxGetPosition(Base), vec2(0.f, 30.f)));
	vxSetPosition(RightLeg2, vec2Add(vxGetPosition(Base), vec2(0.f, 60.f)));



	wdAddVertex(App->Led.Lvl->W, Base);
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

	/* Fin du code temporaire pour les states des animations */
	fpsInit(&fps);
	while (App->Window.isOpen())
	{


		//On verifie si on a pas mis le curseur sur la minimap
		if (MouseWinX >= App->WindowWidth-20.f-(wdGetWidth(lvlGetWorld(App->Led.Lvl)))*MiniMapScale &&
			MouseWinX <= App->WindowWidth-20.f &&
			MouseWinY >= 20.f &&
			MouseWinY <= 20.f+wdGetHeight(lvlGetWorld(App->Led.Lvl))*MiniMapScale)
			InsideMiniMap = TRUE;
		else
			InsideMiniMap = FALSE;

		lvledUpdateNearestPoly(&App->Led);

		sf::Event event;
		while (App->Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				App->Window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				appShowEscapeMenu(App);
			}

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
						/* Création d'un polygone Fixed */
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
							lvledNewPolyFixedAddV(&App->Led);
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
							if (!InsideMiniMap)
								lvledGrab(&App->Led);
							else
							{
								DragMiniMap = TRUE;
								//Si le curseur n'est pas sur la vue dans la minimap on va pas avoir un drag, on va directement bouger la vue (plus intuitif)
								if (MouseWinX >= App->WindowWidth-20.f-(wdGetWidth(lvlGetWorld(App->Led.Lvl))-App->ViewX)*MiniMapScale &&
									MouseWinX <= App->WindowWidth-20.f-(wdGetWidth(lvlGetWorld(App->Led.Lvl))-App->ViewX-App->ViewWidth)*MiniMapScale &&
									MouseWinY >= 20.f+App->ViewY*MiniMapScale &&
									MouseWinY <= 20.f+(App->ViewY+App->ViewHeight)*MiniMapScale)
								{
									MouseMiniMapDragX = (MouseWinX-(App->WindowWidth-20.f-(wdGetWidth(lvlGetWorld(App->Led.Lvl))-App->ViewX)*MiniMapScale));
									MouseMiniMapDragY = (MouseWinY-(20.f+App->ViewY*MiniMapScale));
								}
								else
								{
									MouseMiniMapDragX = 0.f;
									MouseMiniMapDragY = 0.f;
								}

							}
						}
						break;
					case sf::Mouse::Right :
						//lvledGrabEl(&App->Led);
						if (lvledGetNearestPoly(&App->Led) != NULL)
							appShowPolygonMenu(App);
						else {
							appShowLevelMenu(App);
							//appShowVertexMenu(App);
							//appShowRigidMenu(App);
							//appShowElasticMenu(App);
						}
						break;
					case sf::Mouse::Middle :
						MouseDragX = MouseX;
						MouseDragY = MouseY;
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
						DragMiniMap = FALSE;
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
				App->ViewWidth -= event.mouseWheel.delta*WindowRatio*20;
				App->ViewHeight -= event.mouseWheel.delta*20;
				App->ViewWidth = MAX(App->WindowWidth*0.25f, App->ViewWidth);
				App->ViewHeight = MAX(App->WindowHeight*0.25f, App->ViewHeight);

				float fc = App->WindowWidth/App->WindowHeight;

				if (App->ViewWidth > wdGetWidth(lvlGetWorld(App->Led.Lvl))+20.f/MiniMapScale)
				{
					App->ViewWidth = wdGetWidth(lvlGetWorld(App->Led.Lvl))+20.f/MiniMapScale;
					App->ViewHeight = App->ViewWidth/fc;
				}
				if (App->ViewHeight > wdGetHeight(lvlGetWorld(App->Led.Lvl))+20.f/MiniMapScale)
				{
					App->ViewHeight = wdGetHeight(lvlGetWorld(App->Led.Lvl))+20.f/MiniMapScale;
					App->ViewWidth = App->ViewHeight*fc;
				}

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
						App->ViewWidth = App->WindowWidth;
						App->ViewHeight = App->WindowHeight;
						break;
					case sf::Keyboard::B :
						lvledCreateBox(&App->Led, 100);
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
						lvledNewPolyFixedInit(&App->Led);
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
						{
							lvledResetNearestPoly(&App->Led);
							lvledLoad(&App->Led, App->WorkingPath);
						}
						else
							DispDebug = !DispDebug;
						break;
					case sf::Keyboard::F1 :
						DispBack = !DispBack;
						break;
					case sf::Keyboard::F2 :
						DispL1 = !DispL1;
						break;
					case sf::Keyboard::F3 :
						DispL2 = !DispL2;
						break;
					case sf::Keyboard::F4 :
						DispFore = !DispFore;
						break;
					case sf::Keyboard::F :
						lvledToogleNearestFixed(&App->Led);
						break;
					case sf::Keyboard::C :
						if (event.key.control)
							lvledCopyObject(&App->Led);
						else
							lvledToogleNearestPolyFixed(&App->Led);
						break;
					case sf::Keyboard::V :
						if (event.key.control)
							lvledPasteObject(&App->Led);
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
						fprintf(f, "%u %u %u 1\n", 1, (unsigned int)ANIM_POSITIONS, (unsigned int)ANIM_ALL_TRIGGERS);
						//[ANIM_FREE?] [x] [y] #Part of the body
						V = Neck;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "Neck");

						Pos = vec2Prod(vec2Add(vec2Add(vxGetPosition(HeadLeft), vxGetPosition(HeadRight)), vxGetPosition(Neck)), 1.f/3.f);
						Pos = vec2Sub(Pos, vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", 0, Pos.x, 0, Pos.y, "Head");

						V = LeftArm1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "LeftArm1");
						V = LeftArm2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "LeftArm2");
						V = RightArm1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "RightArm1");
						V = RightArm2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "RightArm2");
						V = LeftLeg1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "LeftLeg1");
						V = LeftLeg2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "LeftLeg2");
						V = RightLeg1;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "RightLeg1");
						V = RightLeg2;
						Pos = vec2Sub(vxGetPosition(V), vxGetPosition(Base));
						fprintf(f, "%u %f %u %f #%s\n", (unsigned int)vxIsFixed(V), Pos.x, (unsigned int)vxIsFixed(V), Pos.y, "RightLeg2");

						fclose(f);

						f = fopen("animAngles.txt", "w");
						Vertex* V1, *V2;
						//[nb of states] [AnimType]
						fprintf(f, "%u %u %u 1\n", 1, (unsigned int)ANIM_ANGLES, (unsigned int)ANIM_ALL_TRIGGERS);


						V1 = Neck; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "Neck");

						V1 = HeadLeft; V2 = HeadRight;
						Pos = vec2Prod(vec2Add(vec2Add(vxGetPosition(HeadLeft), vxGetPosition(HeadRight)), vxGetPosition(Neck)), 1.f/3.f);
						Pos = vec2Sub(Pos, vxGetPosition(Neck));
						Angle = vec2Angle(Pos);
						fprintf(f, "%u %f #%s\n", (unsigned int)(vxIsFixed(V1) || !vxIsFixed(V2)), RAD2DEG(Angle), "Head");

						V1 = LeftArm1; V2 = Neck;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "LeftArm1");

						V1 = LeftArm2; V2 = LeftArm1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "LeftArm2");

						V1 = RightArm1; V2 = Neck;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "RightArm1");
						V1 = RightArm2; V2 = RightArm1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "RightArm2");

						V1 = LeftLeg1; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "LeftLeg1");
						V1 = LeftLeg2; V2 = LeftLeg1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "LeftLeg2");

						V1 = RightLeg1; V2 = Base;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "RightLeg1");
						V1 = RightLeg2; V2 = RightLeg1;
						Angle = vec2Angle(vec2Sub(vxGetPosition(V1), vxGetPosition(V2)));
						fprintf(f, "%u %f #%s\n", (unsigned int)vxIsFixed(V1), RAD2DEG(Angle), "RightLeg2");

						fclose(f);
						break;
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
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lvledNewPolyCreate(&App->Led);
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lvledNewPolyFixedCreate(&App->Led);
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

			mnHandleEvent(M, event);
		}

		if (!App->Window.isOpen()) //On sort de la boucle si on a fermé la fenêtre
			break;

		lvledSetMousePosition(&App->Led, MouseX, MouseY);
		lvledGrabUpdate(&App->Led);
		lvledNewBoxUpdate(&App->Led);
		mnUpdate(M, vec2(100.f, 100.f), vec2(-300.f, 100.f));

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
				toViewX += (MouseDragX - MouseX)/5.f;
				toViewY += (MouseDragY - MouseY)/5.f;
			}

			if (DragMiniMap && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				MouseWinX = MIN(MAX(MouseWinX-MouseMiniMapDragX, App->WindowWidth-20.f-wdGetWidth(lvlGetWorld(App->Led.Lvl))*MiniMapScale-10.f), App->WindowWidth-App->ViewWidth*MiniMapScale-10.f);

				MouseWinY = MIN(MAX(MouseWinY-MouseMiniMapDragY, 10.f), (wdGetHeight(lvlGetWorld(App->Led.Lvl))-App->ViewHeight)*MiniMapScale+30.f);

				toViewX = (MouseWinX - (App->WindowWidth-20.f-(wdGetWidth(lvlGetWorld(App->Led.Lvl)))*MiniMapScale))/(wdGetWidth(lvlGetWorld(App->Led.Lvl))*MiniMapScale)*wdGetWidth(lvlGetWorld(App->Led.Lvl));
				toViewY = (MouseWinY - 20.f)/(wdGetHeight(lvlGetWorld(App->Led.Lvl))*MiniMapScale)*wdGetHeight(lvlGetWorld(App->Led.Lvl));


			}
		}


		//On ajuste la vue
		toViewX = MIN(MAX(toViewX, -wdGetWidth(lvlGetWorld(App->Led.Lvl))*MiniMapScale), wdGetWidth(lvlGetWorld(App->Led.Lvl))+10.f/MiniMapScale-App->ViewWidth);
		toViewY = MIN(MAX(toViewY, -10.f/MiniMapScale), wdGetHeight(lvlGetWorld(App->Led.Lvl))+10.f/MiniMapScale-App->ViewHeight);

		Wobble(&App->ViewX, toViewX, 0.5f, 0.5f, &ViewXSpeed);
		Wobble(&App->ViewY, toViewY, 0.5f, 0.5f, &ViewYSpeed);

		/* == Mise à jour du niveau == */
		lvlUpdate(App->Led.Lvl, Paused);

		sndmUpdate(shSoundManager(App->SR));

		/* == Affichage == */

		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0.f, 0.f, App->WindowWidth, App->WindowHeight);
		// Quelle portion de la scène afficher ?
		glOrtho(0.f+App->ViewX, App->ViewWidth+App->ViewX, App->ViewHeight+App->ViewY, 0.f+App->ViewY, 0.0, 100.0);

		if(DispBack) lvlDisplayBG(App->Led.Lvl, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);
		if(DispDebug) gridDraw(&lvlGetWorld(App->Led.Lvl)->CollisionGrid);
		if(DispL1) lvlDisplayL1(App->Led.Lvl);
		if(DispL2) lvlDisplayL2(App->Led.Lvl);
		lvlDispGoalFlag(App->Led.Lvl);
		if(DispObjects) lvlDispAllObj(App->Led.Lvl);
		if(DispFore) lvlDisplayFG(App->Led.Lvl, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);
		if(DispDebug) lvledDraw(&App->Led, LVLED_RULE | LVLED_LIMITS);
		glDrawPolyFromList(&App->Led.tmpLstDyn, vec2(MouseX, MouseY)); /** @todo C'est pas terrible ça... **/
		glDrawPolyFromList(&App->Led.tmpLstFixed, vec2(MouseX, MouseY));

		if (lvledGetNearestPoly(&App->Led) != NULL)
			glDrawPolygon(lvledGetNearestPoly(&App->Led));

		//Minimap
		glDrawMinimap(App->Led.Lvl, App->SR, App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);
		if(DispDebug) lvledDraw(&App->Led, LVLED_RULE | LVLED_LIMITS);

		glPopMatrix();

		MouseX = App->ViewWidth*sf::Mouse::getPosition(App->Window).x/App->WindowWidth + App->ViewX;
		MouseY = App->ViewHeight*sf::Mouse::getPosition(App->Window).y/App->WindowHeight + App->ViewY;
		MouseWinX = sf::Mouse::getPosition(App->Window).x;
		MouseWinY = sf::Mouse::getPosition(App->Window).y;

		fpsStep(&fps);

		glDrawMenuBox(App->SR, App->Window, M, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

		glDrawMenuItems(App->SR, App->Window, M, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

		glDrawFPS(App->SR, App->Window, fpsGetString(&fps));

		if (InsideMiniMap || DragMiniMap)
		{
			App->Window.setMouseCursorVisible(0);
			if (DragMiniMap)
				glDrawCursor(App->Window, App->ViewWidth, App->ViewHeight, MouseWinX, MouseWinY, shGetCursorSprite(App->SR, SPR_CURSOR_DRAG));
			else
				glDrawCursor(App->Window, App->ViewWidth, App->ViewHeight, MouseWinX, MouseWinY, shGetCursorSprite(App->SR, SPR_CURSOR_NORMAL));
		}
		else
			glDrawCursor(App->Window, App->ViewWidth, App->ViewHeight, MouseWinX, MouseWinY, shGetCursorSprite(App->SR, SPR_CURSOR));

		// Update the App->Window
		App->Window.display();
	}
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


	mnFree(M);
	free(M);
}

void appSetWorkingPath(LevelEditorApp* App, const char* Path)
{
	strcpy(App->WorkingPath, Path);
}

void appShowVertexMenu(LevelEditorApp* App)
{
	Vertex* V = lvledGetNearest(&App->Led);
	if(V == NULL) return;

	ItemID IID;
	msgCreateMenu(shMessageManager(App->SR), 4);
	msgAddCloseItem(shMessageManager(App->SR), "Toogle Status (Fixed/Dynamic)");
	msgAddItem(shMessageManager(App->SR), "Mass", ITEM_INPUT_VALUE, NULL, &V->Mass);
	msgAddCloseItem(shMessageManager(App->SR), "Delete this Vertex");
	msgAddCloseItem(shMessageManager(App->SR), "Return");

	IID = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

	switch (IID) {
		case 0:
			lvledToogleNearestFixed(&App->Led);
			break;
		case 1:
			break;
		case 2:
			lvledDelVertex(&App->Led);
			App->Led.Grab = NULL;
			V = NULL;
			break;
		default:
			break;
	}
}

void appShowRigidMenu(LevelEditorApp* App)
{
	Rigid* R = lvledGetNearestRigid(&App->Led);
	if(R == NULL) return;

	ItemID IID;
	msgCreateMenu(shMessageManager(App->SR), 3);
	msgAddItem(shMessageManager(App->SR), "Length", ITEM_INPUT_VALUE, NULL, &R->Length);
	msgAddCloseItem(shMessageManager(App->SR), "Delete this Rigid");
	msgAddCloseItem(shMessageManager(App->SR), "Cancel");

	IID = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

	switch (IID) {
		case 0:
			break;
		case 1:
		{
			lvledDelRigid(&App->Led);
			R = NULL;
			break;
		}
		case 2:
			break;
		default:
			break;
	}
}

void appShowElasticMenu(LevelEditorApp* App)
{
	Elastic* E = lvledGetNearestElastic(&App->Led);
	if(E == NULL) return;

	ItemID IID;
	msgCreateMenu(shMessageManager(App->SR), 4);
	msgAddItem(shMessageManager(App->SR), "Length", ITEM_INPUT_VALUE, NULL, &E->Length);
	msgAddItem(shMessageManager(App->SR), "Spring", ITEM_INPUT_VALUE, NULL, &E->Spring);
	msgAddCloseItem(shMessageManager(App->SR), "Delete this Elastic");
	msgAddCloseItem(shMessageManager(App->SR), "Cancel");

	IID = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

	switch (IID) {
		case 0:
			break;
		case 1:
			break;
		case 2:
		{
			lvledDelElastic(&App->Led);
			E = NULL;
			break;
		}
		case 3:
			break;
		default:
			break;
	}
}

void appShowPolygonMenu(LevelEditorApp* App)
{
	ItemID i;
	msgCreateMenu(shMessageManager(App->SR), 3);
	msgAddCloseItem(shMessageManager(App->SR), "Toogle Status (Fixed/Dynamic)");
	msgAddCloseItem(shMessageManager(App->SR), "Delete this Polygon");
	msgAddCloseItem(shMessageManager(App->SR), "Cancel");

	i = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

	switch (i) {
		case 0:
			lvledToogleNearestPolyFixed(&App->Led);
			break;
		case 1:
		{
			lvledDelPoly(&App->Led);
			App->Led.Grab = NULL;
			lvledUpdateNearestPoly(&App->Led);
			break;
		}
		case 2:
			break;
		default:
			break;
	}
}

void appShowLevelMenu(LevelEditorApp* App)
{
	ItemID i;
	ItemID IID;
	msgCreateMenu(shMessageManager(App->SR), 8);
	msgAddCloseItem(shMessageManager(App->SR), "Add a Box (B)");
	msgAddCloseItem(shMessageManager(App->SR), "Place Spawning Point Here (J)");
	msgAddCloseItem(shMessageManager(App->SR), "Place Goal Here (K)");
	msgAddCloseItem(shMessageManager(App->SR), "Select Background");
	msgAddCloseItem(shMessageManager(App->SR), "Select Layer1");
	msgAddCloseItem(shMessageManager(App->SR), "Select Layer2");
	msgAddCloseItem(shMessageManager(App->SR), "Select Foreground");
	msgAddCloseItem(shMessageManager(App->SR), "Cancel");

	i = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);

	switch (i) {
		case 0:
		{
			lvledCreateBox(&App->Led, 100);
			break;
		}
		case 1 :
			lvledSetSpawn(&App->Led);
			break;
		case 2 :
			lvledSetGoal(&App->Led);
			break;
		case 3:
			msgCreateMessage(shMessageManager(App->SR), "Select Background", 3);
			msgAddItem(shMessageManager(App->SR), "Enter an image path. Save and Reload the level to see the change.", ITEM_LABEL, NULL, NULL);
			IID = msgAddItem(shMessageManager(App->SR), "Path", ITEM_INPUT, NULL, NULL);
			mniSetInput(mnGetItem(msgGetMenu(shMessageManager(App->SR)), 0, IID), App->Led.backPath);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			strcpy(App->Led.backPath, msgGetInput(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight));
			break;
		case 4:
			msgCreateMessage(shMessageManager(App->SR), "Select Layer1", 3);
			msgAddItem(shMessageManager(App->SR), "Enter an image path. Save and Reload the level to see the change.", ITEM_LABEL, NULL, NULL);
			IID = msgAddItem(shMessageManager(App->SR), "Path", ITEM_INPUT, NULL, NULL);
			mniSetInput(mnGetItem(msgGetMenu(shMessageManager(App->SR)), 0, IID), App->Led.layer1Path);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			strcpy(App->Led.layer1Path, msgGetInput(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight));
			break;
		case 5:
			msgCreateMessage(shMessageManager(App->SR), "Select Layer2", 3);
			msgAddItem(shMessageManager(App->SR), "Enter an image path. Save and Reload the level to see the change.", ITEM_LABEL, NULL, NULL);
			IID = msgAddItem(shMessageManager(App->SR), "Path", ITEM_INPUT, NULL, NULL);
			mniSetInput(mnGetItem(msgGetMenu(shMessageManager(App->SR)), 0, IID), App->Led.layer2Path);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			strcpy(App->Led.layer2Path, msgGetInput(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight));
			break;
		case 6:
			msgCreateMessage(shMessageManager(App->SR), "Select Foreground", 3);
			msgAddItem(shMessageManager(App->SR), "Enter an image path. Save and Reload the level to see the change.", ITEM_LABEL, NULL, NULL);
			IID = msgAddItem(shMessageManager(App->SR), "Path", ITEM_INPUT, NULL, NULL);
			mniSetInput(mnGetItem(msgGetMenu(shMessageManager(App->SR)), 0, IID), App->Led.forePath);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			strcpy(App->Led.forePath, msgGetInput(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight));
			break;
		default:
			break;
	}
}

void appShowEscapeMenu(LevelEditorApp* App)
{
	ItemID IID;
	msgCreateMessage(shMessageManager(App->SR), "Menu", 4);
	msgAddCloseItem(shMessageManager(App->SR), "Set Working Path");
	msgAddCloseItem(shMessageManager(App->SR), "Set World Size");
	msgAddCloseItem(shMessageManager(App->SR), "Quit");
	msgAddCloseItem(shMessageManager(App->SR), "Cancel");
	ItemID Choice = msgGetChoice(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);
	switch (Choice)
	{
		case 0 :
			msgCreateMessage(shMessageManager(App->SR), "Set Working Path", 2);
			IID = msgAddItem(shMessageManager(App->SR), "WorkingPath", ITEM_INPUT, NULL, NULL);
			mniSetInput(mnGetItem(msgGetMenu(shMessageManager(App->SR)), 0, IID), App->WorkingPath);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			strcpy(App->WorkingPath, msgGetInput(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight));
			break;
		case 1 :
			msgCreateMessage(shMessageManager(App->SR), "Set World Width", 3);
			msgAddItem(shMessageManager(App->SR), "World Width", ITEM_INPUT_VALUE, NULL, &App->Led.Lvl->W->Width);
			msgAddItem(shMessageManager(App->SR), "World Height", ITEM_INPUT_VALUE, NULL, &App->Led.Lvl->W->Height);
			msgAddCloseItem(shMessageManager(App->SR), "Ok");
			msgDisplay(shMessageManager(App->SR), App->Window, App->ViewX, App->ViewY, App->ViewWidth, App->ViewHeight);
			wdResetGrid(lvlGetWorld(App->Led.Lvl));
			break;
		case 2 :
			App->Window.close();
			break;
		default :
			break;
	}
}
