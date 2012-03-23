#include <Physics/Physics.h>
#include <Physics/Angular.h>
#include <Physics/Length.h>
#include <Level/LevelEditor.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

void glDrawPolygon(Polygon *P);
void glDrawWorld(World* W);
Vertex* GetNearest(World* W, float MouseX, float MouseY);

int main(int argc, char** argv)
{
	unsigned int i, ViewPort;
	float ViewX = 0.f, ViewY = 0.f, ViewSpeed, WindowWidth = 1600.f,
		WindowHeight = 720.f, MapWidth = WindowWidth/10.f, MapHeight = WindowHeight/10.f,
		OldMouseX = 0.f, MouseX, OldMouseY = 0.f, MouseY, toViewX = ViewX, toViewY = ViewY,
		ViewXSpeed = 0.f, ViewYSpeed = 0.f;

	//vec2RegressionTest();

	//vxRegressionTest();

	//polyRegressionTest();

	LevelEditor LvlEd;
	lvledInit(&LvlEd, 3200.f, 1600.f);

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "window");
	window.setFramerateLimit(60.f);
	window.setKeyRepeatEnabled(0);
	window.setMouseCursorVisible(0);

	MapWidth = lvlGetWorld(LvlEd.Lvl)->Width*(100.f / lvlGetWorld(LvlEd.Lvl)->Height);
	MapHeight = 100.f;

//	glMatrixMode(GL_PROJECTION); //On va ainsi définir le viewport
//	glLoadIdentity();
//	glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 100.0);
	glDisable(GL_DEPTH_TEST);

	//glEnable(GL_LINE_SMOOTH); // Anti-Alliasing pour les lignes

	// Start the game loop
	while (window.isOpen())
	{
		MouseX = sf::Mouse::getPosition(window).x + ViewX;
		MouseY = sf::Mouse::getPosition(window).y + ViewY;
		// Process events
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

			/* Prémices de ce que seront les fonctions de LevelEditor */

			if(event.type == sf::Event::MouseButtonPressed)
			{
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left :
						/* Création d'un polygone Fixe */
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
							lvledNewPolyFixeAddV(&LvlEd);
						/* Création d'un polygone Dynamique */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
							lvledNewPolyAddV(&LvlEd);
						/* Ajout d'Elastic */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
							lvledNewElasticAddV(&LvlEd);
						/* Ajout de Rigid */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
							lvledNewRigidAddV(&LvlEd);
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
						lvledRelease(&LvlEd);
						break;
					case sf::Mouse::Right :
						lvledReleaseEl(&LvlEd);
						break;
					default :
						break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::B :
					{V10 = newVertex();
						vxSetPosition(V10, vec2(MouseX, MouseY));
						V11 = newVertex();
						vxSetPosition(V11, vec2(MouseX+50.f, MouseY));
						V12 = newVertex();
						vxSetPosition(V12, vec2(MouseX+50.f, MouseY+50.f));
						V13 = newVertex();
						vxSetPosition(V13, vec2(MouseX, MouseY+50.f));
						wdAddVertex(W, V10); wdAddVertex(W, V11); wdAddVertex(W, V12); wdAddVertex(W, V13);
						Polygon* Rectangle2 = polyRectangle(V10, V11, V12, V13);
						wdAddPolygon(W, Rectangle2);
					}
						break;
					case sf::Keyboard::G :
						lvledGrab(&LvlEd);
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

		/* Déplacement de la vue */

		(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? ViewSpeed = 30.f : ViewSpeed = 15.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			toViewY-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			toViewY+=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			toViewX-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			toViewX+=ViewSpeed;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			toViewX += (OldMouseX - MouseX)*10.f;
			toViewY += (OldMouseY - MouseY)*10.f;
		}

		Wobble(&ViewX, toViewX, 0.5f, 0.5f, &ViewXSpeed);
		Wobble(&ViewY, toViewY, 0.5f, 0.5f, &ViewYSpeed);

		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		glClearColor(0.0f, 0.f, 0.f, 1.f); //Ici optionnel car par défaut couleur est rouge
		//glClear(GL_DEPTH_BUFFER_BIT);

		//On prepare la projection
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Test Viewport : Minimap !
		glMatrixMode(GL_PROJECTION); //On va ainsi définir le viewport
		glLoadIdentity();
		glOrtho(0.0, WindowWidth, WindowHeight, 0.0, 0.0, 100.0);

		for(ViewPort = 0; ViewPort < 2; ViewPort++)
		{
			if(ViewPort == 0)
			{
				glViewport(0.f, 0.f, WindowWidth, WindowHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.f+ViewX, WindowWidth+ViewX, WindowHeight+ViewY, 0.f+ViewY, 0.0, 100.0);

				wdApplyForce(lvlGetWorld(LvlEd.Lvl), vec2(0.f, 0.6f));
				wdResolveVextex(lvlGetWorld(LvlEd.Lvl));

				for(i=0; i<4; i++)
				{
					wdResolveRigid(lvlGetWorld(LvlEd.Lvl));
					wdResolveElastic(lvlGetWorld(LvlEd.Lvl));
					//lnResolve(&L);
					wdHandleCollision(lvlGetWorld(LvlEd.Lvl));
				}
			}
			else if(ViewPort == 1)
			{
				glViewport(WindowWidth - MapWidth - 10.f, WindowHeight - MapHeight - 10.f, MapWidth, MapHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, lvlGetWorld(LvlEd.Lvl)->Width, lvlGetWorld(LvlEd.Lvl)->Height, 0.0, 0.0, 100.0);
				glColor3f(0.5f, 0.5f, 0.5f);
				glLineStipple(1, 0xCCCC);
				glEnable(GL_LINE_STIPPLE);
				glBegin(GL_LINE_LOOP);
					glVertex2f(ViewX, ViewY);
					glVertex2f(ViewX + WindowWidth, ViewY);
					glVertex2f(ViewX + WindowWidth, ViewY + WindowHeight);
					glVertex2f(ViewX, ViewY + WindowHeight);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
			}

			glDrawWorld(lvlGetWorld(LvlEd.Lvl));

			/* Limites */
			glColor3f(0.5f, 0.5f, 0.5f);
			glLineStipple(1, 0xCCCC);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.f, 1.f);
				glVertex2f(lvlGetWorld(LvlEd.Lvl)->Width-1.f, 0.f);
				glVertex2f(lvlGetWorld(LvlEd.Lvl)->Width-1.f, lvlGetWorld(LvlEd.Lvl)->Height);
				glVertex2f(0.f, lvlGetWorld(LvlEd.Lvl)->Height);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			//Une regle
			glColor3f(0.f, 0.f, 1.f);
			glBegin(GL_LINES);
			glVertex2f(0.f,MouseY);
			glVertex2f(lvlGetWorld(LvlEd.Lvl)->Width,MouseY);
			glEnd();

			glBegin(GL_LINES);
			for (float i=0.f; i<lvlGetWorld(LvlEd.Lvl)->Width; i+=10.f)
			{
				glVertex2f(i,MouseY);
				if (static_cast<int>(i)%100 == 0)
					glVertex2f(i,MouseY-5.f);
				else if (static_cast<int>(i)%50 == 0)
					glVertex2f(i,MouseY-3.5f);
				else
					glVertex2f(i,MouseY-2.5f);
			}
			glEnd();

			glBegin(GL_LINES);
			glVertex2f(MouseX, 0.f);
			glVertex2f(MouseX, lvlGetWorld(LvlEd.Lvl)->Height);
			glEnd();

			glBegin(GL_LINES);
			for (float i=0.f; i<lvlGetWorld(LvlEd.Lvl)->Height; i+=10.f)
			{
				glVertex2f(MouseX, i);
				if (static_cast<int>(i)%100 == 0)
					glVertex2f(MouseX+5.f, i);
				else if (static_cast<int>(i)%50 == 0)
					glVertex2f(MouseX+3.5f, i);
				else
					glVertex2f(MouseX+2.5f, i);
			}
			glEnd();

		}

		OldMouseX = MouseX;
		OldMouseY = MouseY;

		// Update the window
		window.display();
	}

	delWorld(lvlGetWorld(LvlEd.Lvl));

	return 0;
}


void glDrawPolygon(Polygon* P)
{
	unsigned int i;

	glColor4f(1.f, 1.f, 1.f, 0.2f);
	glBegin(GL_POLYGON);
	for(i = 0; i<daGetSize(&P->Vertices); i++)
		glVertex2f(vxGetPosition((Vertex*)daGet(&P->Vertices, i)).x, vxGetPosition((Vertex*)daGet(&P->Vertices, i)).y);
	glEnd();

	glColor4f(0.f, 1.f, 0.f, 0.5f);
	glBegin(GL_LINES);
	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
			glVertex2f(vxGetPosition(rdGetV1((Rigid*)daGet(&P->Rigids, i))).x, vxGetPosition(rdGetV1((Rigid*)daGet(&P->Rigids, i))).y);
			glVertex2f(vxGetPosition(rdGetV2((Rigid*)daGet(&P->Rigids, i))).x, vxGetPosition(rdGetV2((Rigid*)daGet(&P->Rigids, i))).y);
	}

	glColor3f(0.f, 0.f, 1.f);
	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
			glVertex2f(vxGetPosition(rdGetV1((Rigid*)daGet(&P->InternalRigids, i))).x, vxGetPosition(rdGetV1((Rigid*)daGet(&P->InternalRigids, i))).y);
			glVertex2f(vxGetPosition(rdGetV2((Rigid*)daGet(&P->InternalRigids, i))).x, vxGetPosition(rdGetV2((Rigid*)daGet(&P->InternalRigids, i))).y);
	}
	glEnd();

	if(polyIsFixe(P)) glColor3f(1.f, 0.f, 0.f); else glColor3f(0.f, 0.f, 1.f);
	Vec2 Center = polyComputeCenter(P);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(Center.x, Center.y);
		for (int i=0; i<=16; i++)
		{
			glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + Center.x,
					1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + Center.y);
		}
		glEnd();
	
	//BBox
	BBox B = polyGetBBox(P);
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(B.Left, B.Top);
	glVertex2f(B.Right, B.Top);
	glVertex2f(B.Right, B.Bottom);
	glVertex2f(B.Left, B.Bottom);
	glEnd();
}


void glDrawWorld(World* W)
{
	Node* it = lstFirst(&W->Vertices);
	while(!nodeEnd(it))
	{
		if(vxIsFixe((Vertex*) nodeGetData(it)))
			glColor4f(1.f, 0.f, 0.f, 0.3f);
		else
			glColor4f(0.f, 1.f, 0.f, 0.3f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(vxGetPosition((Vertex*) nodeGetData(it)).x, vxGetPosition((Vertex*) nodeGetData(it)).y);
		for (int i=0; i<=16; i++)
		{
			glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition((Vertex*) nodeGetData(it)).x,
					1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition((Vertex*) nodeGetData(it)).y);
		}
		it = nodeGetNext(it);
		glEnd();
	}

	glBegin(GL_LINES);

	glColor3f(1.f, 0.f, 0.f);
	it = lstFirst(&W->Elastics);
	while(!nodeEnd(it))
	{
		glVertex2f(vxGetPosition(elasticGetV1((Elastic*) nodeGetData(it))).x, vxGetPosition(elasticGetV1((Elastic*) nodeGetData(it))).y);
		glVertex2f(vxGetPosition(elasticGetV2((Elastic*) nodeGetData(it))).x, vxGetPosition(elasticGetV2((Elastic*) nodeGetData(it))).y);
		it = nodeGetNext(it);
	}

	glColor3f(0.f, 1.f, 0.f);
	it = lstFirst(&W->Rigids);
	while(!nodeEnd(it))
	{
		glVertex2f(vxGetPosition(rdGetV1((Rigid*) nodeGetData(it))).x, vxGetPosition(rdGetV1((Rigid*) nodeGetData(it))).y);
		glVertex2f(vxGetPosition(rdGetV2((Rigid*) nodeGetData(it))).x, vxGetPosition(rdGetV2((Rigid*) nodeGetData(it))).y);
		it = nodeGetNext(it);
	}

	glEnd();

	it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		glDrawPolygon((Polygon*) nodeGetData(it));
		it = nodeGetNext(it);
	}
}



