#include <Physics/Physics.h>
#include <Physics/Angular.h>
#include <Physics/Joint.h>
#include <Physics/Length.h>
#include <Level/LevelEditor.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Rendering/OpenGL.h>

int main(int argc, char** argv)
{
	unsigned int ViewPort, i;
	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, WindowWidth = 1200.f,
		WindowHeight = 600.f, MapWidth = WindowWidth/10.f, MapHeight = WindowHeight/10.f,
		OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
		ViewXSpeed = 0.f, ViewYSpeed = 0.f, ViewWidth = WindowWidth, ViewHeight = WindowHeight,
		WindowRatio = WindowWidth/WindowHeight;
	sf::Clock Clock;
	Bool L1 = FALSE;

	//vec2RegressionTest();

	//vxRegressionTest();

	//polyRegressionTest();

	//� faire qqpart d'autre xDD
	if (!DirectoryExists("levels"))
		CreateDirectory("levels");

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Jump n'Run");
	window.setFramerateLimit(6000.f);
	window.setKeyRepeatEnabled(0);
	window.setMouseCursorVisible(0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	LevelEditor LvlEd;
	lvledInit(&LvlEd, 4000.f, 1480.f);

	lvledSetLineDraw(&LvlEd, &glDrawLine);
	lvledSetVxDraw(&LvlEd, &glDrawVertex);
	lvledSetElDraw(&LvlEd, &glDrawElastic);
	lvledSetRdDraw(&LvlEd, &glDrawRigid);
	lvledSetPolyDraw(&LvlEd, &glDrawPolygon);


	LvlEd.Lvl->lvlTexLoad = &glTexLoad;
	LvlEd.Lvl->lvlTexFree = &glTexFree;
	LvlEd.Lvl->lvlDisplayTex = &glDisplayTex;
	LvlEd.Lvl->Layer1 = (*LvlEd.Lvl->lvlTexLoad)("Pano.jpg");

	MapWidth = lvlGetWorld(LvlEd.Lvl)->Width*(100.f / lvlGetWorld(LvlEd.Lvl)->Height);
	MapHeight = 100.f;

	//lvledSave(&LvlEd, "levels/Test.txt");
	lvledLoad(&LvlEd, "levels/Test.txt");

	Joint J; Vec2 off = vec2(150.f, 150.f);
	Vertex *C = newVertex(), *M = newVertex(), *S = newVertex();
	wdAddVertex(lvlGetWorld(LvlEd.Lvl), C);
	wdAddVertex(lvlGetWorld(LvlEd.Lvl), S);
	wdAddVertex(lvlGetWorld(LvlEd.Lvl), M);
	vxSetPosition(C, off);
	vxSetPosition(M, vec2(off.x, off.y-20.f));
	vxSetPosition(S, vec2(off.x, off.y+35.f));
	vxSetFixe(C, 1); vxSetFixe(M, 1); vxSetFixe(S, 1);
	Rigid *R1 = newRigid(C, M, -1.f), *R2 = newRigid(C, S, -1.f);
	wdAddRigid(lvlGetWorld(LvlEd.Lvl), R1);
	wdAddRigid(lvlGetWorld(LvlEd.Lvl), R2);
	Vec2 CPos = vxGetPosition(C),  MPos = vxGetPosition(M), SPos = vxGetPosition(S);
	printf("C: %f, %f; M: %f, %f; S: %f, %f\n",CPos.x, CPos.y, MPos.x, MPos.y, SPos.x, SPos.y);
	
	jnInit(&J, C, M, S);

	/*
	Vertex *C = newVertex(), *M = newVertex(), *S = newVertex();
	vxSetPosition(M, vec2(50.f, 100.f)); vxSetPosition(C, vec2(100.f, 100.f)); vxSetPosition(S, vec2(150.f, 100.f));
	wdAddRigid(lvlGetWorld(LvlEd.Lvl), newRigid(C, M, 50.f));
	wdAddRigid(lvlGetWorld(LvlEd.Lvl), newRigid(C, S, 50.f));
	vxSetFixe(C, 1);
	vxSetFixe(M, 1);
	wdAddVertex(lvlGetWorld(LvlEd.Lvl), C); wdAddVertex(lvlGetWorld(LvlEd.Lvl), M); wdAddVertex(lvlGetWorld(LvlEd.Lvl), S);
	Angular Ang;
	angInit(&Ang, C, M, S, -M_PI_4, M_PI_4);
	*/

	//glEnable(GL_LINE_SMOOTH); // Anti-Alliasing pour les lignes
	
	Clock.restart(); unsigned int frames = 0;
	while (window.isOpen())
	{
		MouseX = ViewWidth*sf::Mouse::getPosition(window).x/WindowWidth + ViewX;
		MouseY = ViewHeight*sf::Mouse::getPosition(window).y/WindowHeight + ViewY;

		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Escape pressed : exit
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

			if (event.type == sf::Event::Resized)
				printf("Resized ! %u, %u \n", event.size.width, event.size.height);

			if(event.type == sf::Event::MouseButtonPressed)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left :
						/* Cr�ation d'un polygone Fixe */
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
							lvledNewPolyFixeAddV(&LvlEd);
						/* Cr�ation d'un polygone Dynamique */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
							lvledNewPolyAddV(&LvlEd);
						/* Ajout d'Elastic */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
							lvledNewElasticAddV(&LvlEd);
						/* Ajout de Rigid */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
							lvledNewRigidAddV(&LvlEd);
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							lvledNewBoxInit(&LvlEd);
						} else {
							lvledGrab(&LvlEd);
						}
						break;
					case sf::Mouse::Right :
						lvledGrabEl(&LvlEd);
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
							lvledNewBoxCreate(&LvlEd);
						} else {
							lvledRelease(&LvlEd);
						}
						break;
					case sf::Mouse::Right :
						lvledReleaseEl(&LvlEd);
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
						window.setActive(0);
						lvledTestLevel(&LvlEd);
						window.setActive(1);
						break;
					case sf::Keyboard::Space :
						ViewWidth = WindowWidth;
						ViewHeight = WindowHeight;
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
						wdAddVertex(lvlGetWorld(LvlEd.Lvl), V10); wdAddVertex(lvlGetWorld(LvlEd.Lvl), V11); wdAddVertex(lvlGetWorld(LvlEd.Lvl), V12); wdAddVertex(lvlGetWorld(LvlEd.Lvl), V13);
						Polygon* Rectangle2 = polyRectangle(V10, V11, V12, V13);
						wdAddPolygon(lvlGetWorld(LvlEd.Lvl), Rectangle2);
					}
						break;
					case sf::Keyboard::G :
						lvledGrab(&LvlEd);
						break;
					case sf::Keyboard::N :
						printf("Number of Vertices: %u\nNumber of Polygons: %u\n", lstCount(&lvlGetWorld(LvlEd.Lvl)->Vertices), lstCount(&lvlGetWorld(LvlEd.Lvl)->Polygons));
						break;
					case sf::Keyboard::E :
						lvledGrabEl(&LvlEd);
						break;
					case sf::Keyboard::Delete :
						if(!event.key.shift)
						{
							lvledDelPoly(&LvlEd);
						} else {
							lvledDelVertex(&LvlEd);
						}
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lvledNewPolyFixeInit(&LvlEd);
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lvledNewPolyInit(&LvlEd);
						break;
					case sf::Keyboard::S :
						if (event.key.control)
							lvledSave(&LvlEd, "levels/Test.txt");
						break;
					case sf::Keyboard::L :
						if (event.key.control)
							lvledLoad(&LvlEd, "levels/Test.txt");
						break;
					case sf::Keyboard::X :
						plGetUp(LvlEd.Lvl->P1);
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
						lvledRelease(&LvlEd);
						break;
					case sf::Keyboard::E :
						lvledReleaseEl(&LvlEd);
						break;
					case sf::Keyboard::F :
						lvledToogleNearestFixe(&LvlEd);
						break;
					case sf::Keyboard::C :
						lvledToogleNearestPolyFixe(&LvlEd);
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lvledNewPolyCreate(&LvlEd);
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lvledNewPolyFixeCreate(&LvlEd);
						break;
					case sf::Keyboard::Num3 :
						lvledNewElasticCreate(&LvlEd);
						break;
					case sf::Keyboard::Num4 :
						lvledNewRigidCreate(&LvlEd);
						break;
					default:
						break;
				}
			}
		}

		lvledSetMousePosition(&LvlEd, MouseX, MouseY);
		lvledGrabUpdate(&LvlEd);
		lvledNewBoxUpdate(&LvlEd);

		/* D�placement de la vue */
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

		/* == Mise � jour du niveau == */
		for (i=0; i<4; i++)
		jnResolve(&J);
		lvlUpdate(LvlEd.Lvl);

		/* == Affichage == */

		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		//glClearColor(0.0f, 0.f, 0.f, 1.f); //Ici optionnel car par d�faut couleur est rouge
		//glClear(GL_DEPTH_BUFFER_BIT);

		// On prepare la projection
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Test Viewport : Minimap !
		glMatrixMode(GL_PROJECTION); // On va ainsi d�finir le viewport
		glViewport(0.f, 0.f, WindowWidth, WindowHeight);
		glLoadIdentity();
		glOrtho(0.0, WindowWidth, WindowHeight, 0.0, 0.0, 100.0);

		for(ViewPort = 0; ViewPort < 2; ViewPort++)
		{
			if(ViewPort == 0)
			{
				glViewport(0.f, 0.f, WindowWidth, WindowHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.f+ViewX, ViewWidth+ViewX, ViewHeight+ViewY, 0.f+ViewY, 0.0, 100.0);

				if(L1) lvlDisplayL1(LvlEd.Lvl);

				/* Affichage de la Grille */
				gridDraw(&lvlGetWorld(LvlEd.Lvl)->CollisionGrid);
			}
			else if(ViewPort == 1)
			{
				glViewport(WindowWidth - MapWidth - 10.f, WindowHeight - MapHeight - 10.f, MapWidth, MapHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				/* La minimap affiche tout le monde */
				glOrtho(0.0, lvlGetWorld(LvlEd.Lvl)->Width, lvlGetWorld(LvlEd.Lvl)->Height, 0.0, 0.0, 100.0);

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

			lvledDraw(&LvlEd, LVLED_RULE | LVLED_LIMITS);
		}

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		// Update the window
		window.display();
		frames++;
		if (Clock.getElapsedTime().asSeconds()>=1.f)
		{
			printf("FPS: %.1f\n", round(frames/Clock.getElapsedTime().asSeconds()));
			Clock.restart();
			frames = 0;
		}
	}

	lvledFree(&LvlEd);

	return 0;
}

/*
void glDrawVertex(Vertex* V, float R, float G, float B, float A)
{
	unsigned int i;
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(R, G, B, A);
	glVertex2f(vxGetPosition(V).x, vxGetPosition(V).y);
	for (i = 0; i <= 16; i++)
	{
		glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(V).x,
				1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(V).y);
	}
	glEnd();
}
 

void glDrawElastic(Elastic* E)
{
	glColor4f(fabs(vec2SqLength(elVector(E)) - elGetLength(E)*elGetLength(E))/vec2SqLength(elVector(E)) + 0.2f, 0.f, 0.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(vxGetPosition(elGetV1(E)).x, vxGetPosition(elGetV1(E)).y);
	glVertex2f(vxGetPosition(elGetV2(E)).x, vxGetPosition(elGetV2(E)).y);
	glEnd();
}

void glDrawRigid(Rigid* R)
{
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(vxGetPosition(rdGetV1(R)).x, vxGetPosition(rdGetV1(R)).y);
	glVertex2f(vxGetPosition(rdGetV2(R)).x, vxGetPosition(rdGetV2(R)).y);
	glEnd();
}

void glDrawPolygon(Polygon* P)
{
	unsigned int i;

	glColor4f(1.f, 1.f, 1.f, 0.2f);
	glBegin(GL_POLYGON);
	for(i = 0; i < daGetSize(&P->Vertices); i++)
		glVertex2f(vxGetPosition((Vertex*)daGet(&P->Vertices, i)).x, vxGetPosition((Vertex*)daGet(&P->Vertices, i)).y);
	glEnd();

	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		glDrawRigid((Rigid*)daGet(&P->Rigids, i));
	}

	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
		glDrawRigid((Rigid*)daGet(&P->InternalRigids, i));
	}

	if(polyIsFixe(P)) glColor4f(1.f, 0.f, 0.f, 1.f); else glColor4f(0.f, 0.f, 1.f, 1.f);
	Vec2 Center = polyComputeCenter(P);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(Center.x, Center.y);
	for (i = 0; i <= 16; i++)
	{
		glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + Center.x,
				1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + Center.y);
	}
	glEnd();

	 //BBox
	BBox B = polyGetBBox(P);
	glColor4f(1.f, 0.f, 0.f, 0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(B.Left, B.Top);
		glVertex2f(B.Right, B.Top);
		glVertex2f(B.Right, B.Bottom);
		glVertex2f(B.Left, B.Bottom);
	glEnd(); 
}
 */



