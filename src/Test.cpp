#include <Physics/Physics.h>
#include <Physics/Angular.h>
#include <Physics/Length.h>
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

	World* W = newWorld(3200.f, 1200.f);


	Vertex* V1 = newVertex();
	vxSetPosition(V1, vec2(0.f, 500.f));
	Vertex* V2 = newVertex();
	vxSetPosition(V2, vec2(30.f, 500.f));
	Vertex* V3 = newVertex();
	vxSetPosition(V3, vec2(0.f, 550.f));
	wdAddVertex(W, V1); wdAddVertex(W, V2); wdAddVertex(W, V3);
	/*Rigid* R1 = newRigid(V1, V2, vec2Length(vec2Sub(vxGetPosition(V1), vxGetPosition(V2))));
	Rigid* R2 = newRigid(V3, V2, vec2Length(vec2Sub(vxGetPosition(V3), vxGetPosition(V2))));

	wdAddRigid(W, R1); wdAddRigid(W, R2);*/

	Polygon* Pang1 = newPolygon(2, V1, V2);
	Polygon* Pang2 = newPolygon(2, V3, V2);
	wdAddPolygon(W, Pang1); wdAddPolygon(W, Pang2);

	Angular A;
	angInit(&A, V2, V1, V3, (M_PI/180.f)*25.f, 110.f*(M_PI/180.f));
	//Lenght L;
	//lnInit(&L, V1, V3, 30.f, 50.f);

	Vertex* V10 = newVertex();
	vxSetPosition(V10, vec2(50.f, 500.f));
	Vertex* V11 = newVertex();
	vxSetPosition(V11, vec2(100.f, 500.f));
	Vertex* V12 = newVertex();
	vxSetPosition(V12, vec2(100.f, 550.f));
	Vertex* V13 = newVertex();
	vxSetPosition(V13, vec2(50.f, 550.f));
	wdAddVertex(W, V10);
	wdAddVertex(W, V11); wdAddVertex(W, V12); wdAddVertex(W, V13);

	//Polygon* Poly = newPolygon(3, V1, V2, V3);
	//wdAddPolygon(W, Poly);
	Polygon* Rectangle = polyRectangle(V10, V11, V12, V13);
	wdAddPolygon(W, Rectangle);



	V10 = newVertex();
	vxSetPosition(V10, vec2(50.f, 400.f));
	V11 = newVertex();
	vxSetPosition(V11, vec2(100.f, 400.f));
	V12 = newVertex();
	vxSetPosition(V12, vec2(100.f, 450.f));
	V13 = newVertex();
	vxSetPosition(V13, vec2(50.f, 450.f));
	wdAddVertex(W, V10); wdAddVertex(W, V11); wdAddVertex(W, V12); wdAddVertex(W, V13);
	Polygon* Rectangle2 = polyRectangle(V10, V11, V12, V13);
	wdAddPolygon(W, Rectangle2);




	//Un rectangle fixe
	Vertex* VF1 = newVertex();
	vxSetPosition(VF1, vec2(300.f, 400.f));
	Vertex* VF2 = newVertex();
	vxSetPosition(VF2, vec2(550.f, 400.f));
	Vertex* VF3 = newVertex();
	vxSetPosition(VF3, vec2(550.f, 500.f));
	Vertex* VF4 = newVertex();
	vxSetPosition(VF4, vec2(300.f, 500.f));

	wdAddVertex(W, VF1);wdAddVertex(W, VF2);wdAddVertex(W, VF3);wdAddVertex(W, VF4);
	Polygon* FixeRect = polyRectangle(VF1, VF2, VF3, VF4);
	polySetFixe(FixeRect, 1);
	wdAddPolygon(W, FixeRect);


	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "window");
	window.setFramerateLimit(60.f);
	window.setKeyRepeatEnabled(0);
	window.setMouseCursorVisible(0);

	MapWidth = W->Width*(100.f / W->Height);
	MapHeight = 100.f;

//	glMatrixMode(GL_PROJECTION); //On va ainsi définir le viewport
//	glLoadIdentity();
//	glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 100.0);
	glDisable(GL_DEPTH_TEST);

	//glEnable(GL_LINE_SMOOTH); // Anti-Alliasing pour les lignes

	Vertex *grab=NULL;
	Vertex *grabEl=NULL;
	Vertex* Mouse = newVertex();
	Vertex* tmpVertex = NULL;
	Vertex *tmpElastic1 = NULL, *tmpElastic2 = NULL, *tmpRigid1 = NULL, *tmpRigid2 = NULL;
	Polygon* tmpPoly = NULL;
	Elastic* GrabElastic = newElastic(grabEl, Mouse, 30.f, 0.2f);
	List L, L2;

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
							tmpVertex = newVertex();
							vxSetPosition(tmpVertex, vec2(MouseX, MouseY));
							vxSetFixe(tmpVertex, 1);
							lstAdd(&L, tmpVertex);
						/* Création d'un polygone Dynamique */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
							tmpVertex = newVertex();
							vxSetPosition(tmpVertex, vec2(MouseX, MouseY));
							vxSetFixe(tmpVertex, 1);
							lstAdd(&L2, tmpVertex);
						/* Ajout d'Elastic */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
							if(tmpElastic1 == NULL) {
								tmpElastic1 = wdGetNearest(W, MouseX, MouseY);
							} else if(tmpElastic2 == NULL) {
								tmpElastic2 = wdGetNearest(W, MouseX, MouseY);
							}
						/* Ajout de Rigid */
						} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
							if(tmpRigid1 == NULL)
								tmpRigid1 = wdGetNearest(W, MouseX, MouseY);
							else if(tmpRigid2 == NULL)
								tmpRigid2 = wdGetNearest(W, MouseX, MouseY);
						} else {
							grab = wdGetNearest(W, MouseX, MouseY);
						}
						break;
					case sf::Mouse::Right :
						grabEl = wdGetNearest(W, MouseX, MouseY);
						if(grabEl != NULL)
						{
							GrabElastic->V1 = grabEl;
							wdAddElastic(W, GrabElastic);
						}
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
						grab = NULL;
						break;
					case sf::Mouse::Right :
						grabEl = NULL;
						wdDelElastic(W, GrabElastic);
						break;
					default :
						break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::G :
						grab = wdGetNearest(W, MouseX, MouseY);
						break;
					case sf::Keyboard::E :
						grabEl = wdGetNearest(W, MouseX, MouseY);
						if(grabEl != NULL)
						{
							GrabElastic->V1=grabEl;
							wdAddElastic(W, GrabElastic);
						}
						break;
					case sf::Keyboard::Delete :
						if(!event.key.shift)
						{
							/* lvledDelPoly() */
							tmpPoly = wdGetNearestPoly(W, MouseX, MouseY);
							if(tmpPoly != NULL)
							{
								wdDelPolygon(W, tmpPoly);
								delPolygon(tmpPoly);
							}
						} else {
							/* lvledDelVertex() */
							Bool IsLonely = 1;
							tmpVertex = wdGetNearest(W, MouseX, MouseY);
							if(tmpVertex != NULL)
							{
								/* On s'assure que le Vertex ne fait partie d'aucune structure plus grande */
								Node* it = lstFirst(&W->Rigids);
								while(!nodeEnd(it))
								{
									if(rdGetV1((Rigid*) nodeGetData(it)) == tmpVertex || rdGetV2((Rigid*) nodeGetData(it)) == tmpVertex)
										IsLonely = 0;
									it = nodeGetNext(it);
								}
								it = lstFirst(&W->Elastics);
								while(!nodeEnd(it))
								{
									if(elasticGetV1((Elastic*) nodeGetData(it)) == tmpVertex || elasticGetV2((Elastic*) nodeGetData(it)) == tmpVertex)
										IsLonely = 0;
									it = nodeGetNext(it);
								}
								it = lstFirst(&W->Polygons);
								while(!nodeEnd(it))
								{
									for(i = 0; i < daGetSize(&((Polygon*) nodeGetData(it))->Vertices); i++)
									{
										if(daGet(&((Polygon*) nodeGetData(it))->Vertices, i) == tmpVertex)
											IsLonely = 0;
									}
									it = nodeGetNext(it);
								}

								if(IsLonely)
									wdDelVertex(W, tmpVertex),
									delVertex(tmpVertex);
							}
						}
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						lstInit(&L);
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						lstInit(&L2);
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
						grab = NULL;
						break;
					case sf::Keyboard::E :
						grabEl=NULL;
						wdDelElastic(W, GrabElastic);
						break;
					case sf::Keyboard::F :
						/* lvledSetFixe */
						tmpVertex = wdGetNearest(W, MouseX, MouseY);
						vxSetFixe(tmpVertex, !vxIsFixe(tmpVertex));
						break;
					case sf::Keyboard::C :
						/* lvledSetFixePoly */
						tmpPoly = wdGetNearestPoly(W, MouseX, MouseY),
						polySetFixe(tmpPoly, !polyIsFixe(tmpPoly));
						break;
					case sf::Keyboard::O :
					case sf::Keyboard::Num2 :
						if(lstCount(&L2) > 0)
						{
							wdAddVxFromList(W, L2);

							if(lstCount(&L2) == 4)
							{
								tmpPoly = polyRectangleL(L2);
							}
							else if (lstCount(&L2) <= 3)
							{
								tmpPoly = newPolygonL(L2);
							}
							else
							{
								tmpPoly = polyNGone(L2);
							}
							polySetFixe(tmpPoly, 0);
							wdAddPolygon(W, tmpPoly);
						}
						lstFree(&L2);
						break;
					case sf::Keyboard::P :
					case sf::Keyboard::Num1 :
						if(lstCount(&L) > 0)
						{
							wdAddVxFromList(W, L);
							tmpPoly = newPolygonL(L);
							polySetFixe(tmpPoly, 1);
							wdAddPolygon(W, tmpPoly);
						}
						lstFree(&L);
						break;
					case sf::Keyboard::Num3 :
							if(tmpElastic1 != NULL && tmpElastic2 != NULL && tmpElastic1 != tmpElastic2)
								wdAddElastic(W, newElastic(tmpElastic1, tmpElastic2,
														-1.f, 0.1f));
							tmpElastic1 = tmpElastic2 = NULL;
						break;
					case sf::Keyboard::Num4 :
						if(tmpRigid1 != NULL && tmpRigid2 != NULL)
							wdAddRigid(W, newRigid(tmpRigid1, tmpRigid2, -1.f));
						tmpRigid1 = tmpRigid2 = NULL;
						break;
					default:
						break;
				}
			}
		}

		if (grab!=NULL)
			vxSetPosition(grab, vec2(MouseX,MouseY));
		if(grabEl != NULL)
			vxSetPosition(Mouse, vec2(MouseX,MouseY));

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
			toViewX += roundf(OldMouseX - MouseX)*((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 1.5f : 1);
			toViewY += roundf(OldMouseY - MouseY)*((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 1.5f : 1);
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

				wdApplyForce(W, vec2(0.f, 0.6f));
				wdResolveVextex(W);
				angResolve(&A);

				for(i=0; i<4; i++)
				{
					wdResolveRigid(W);
					wdResolveElastic(W);
					//lnResolve(&L);
					wdHandleCollision(W);
				}
			}
			else if(ViewPort == 1)
			{
				glViewport(WindowWidth - MapWidth - 10.f, WindowHeight - MapHeight - 10.f, MapWidth, MapHeight);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, W->Width, W->Height, 0.0, 0.0, 100.0);
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

			if(grabEl != NULL)
			{
				glBegin(GL_LINES);
				glVertex2f(vxGetPosition(elasticGetV1(GrabElastic)).x, vxGetPosition(elasticGetV1(GrabElastic)).y);
				glVertex2f(vxGetPosition(elasticGetV2(GrabElastic)).x, vxGetPosition(elasticGetV2(GrabElastic)).y);
				glEnd();
			}

			glDrawWorld(W);

			/* Limites */
			glColor3f(0.5f, 0.5f, 0.5f);
			glLineStipple(1, 0xCCCC);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINE_LOOP);
				glVertex2f(0.f, 1.f);
				glVertex2f(W->Width-1.f, 0.f);
				glVertex2f(W->Width-1.f, W->Height);
				glVertex2f(0.f, W->Height);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			//Une regle
			glColor3f(0.f, 0.f, 1.f);
			glBegin(GL_LINES);
			glVertex2f(0.f,MouseY);
			glVertex2f(W->Width,MouseY);
			glEnd();

			glBegin(GL_LINES);
			for (float i=0.f; i<W->Width; i+=10.f)
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
			glVertex2f(MouseX, W->Height);
			glEnd();

			glBegin(GL_LINES);
			for (float i=0.f; i<W->Height; i+=10.f)
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

	delWorld(W);

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



