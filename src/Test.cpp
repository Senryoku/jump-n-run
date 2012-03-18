#include <Physics/Physics.h>
#include <Physics/Angular.h>
#include <Physics/Lenght.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

void glDrawPolygon(Polygon *P);
void glDrawWorld(World* W);
Vertex* GetNearest(World* W, float MouseX, float MouseY);

int main(int argc, char** argv)
{
	unsigned int i, ViewPort;
	float ViewX = 0.f, ViewY = 0.f, ViewSpeed = 5.f,
		WindowWidth = 800.f, WindowHeight = 600.f, MouseX, MouseY;

	//vec2TestRegression();

	//vxTestRegression();

	//polyTestRegression();

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
	Lenght L;
	lnInit(&L, V1, V3, 30.f, 50.f);

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


	sf::RenderWindow window(sf::VideoMode(800, 600), "window");
	window.setFramerateLimit(60.f);
	window.setKeyRepeatEnabled(0);
	window.setMouseCursorVisible(0);

//	glMatrixMode(GL_PROJECTION); //On va ainsi définir le viewport
//	glLoadIdentity();
//	glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 100.0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH); // Anti-Alliasing pour les lignes

	Vertex *grab=NULL;
	Vertex *grabEl=NULL;
	Elastic* Elastic = NULL;
	Vertex* Mouse = newVertex();

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

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
			{
				grab=GetNearest(W, MouseX, MouseY);
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G)
				grab=NULL;

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
			{
				grabEl = GetNearest(W, MouseX, MouseY);
				Elastic = newElastic(grabEl, Mouse, 30.f, 0.2f);
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::E)
				grabEl=NULL,
				delElastic(Elastic), Elastic = NULL;
		}

		if (grab!=NULL)
			vxSetPosition(grab, vec2(MouseX,MouseY));
		if(grabEl != NULL)
		vxSetPosition(Mouse, vec2(MouseX,MouseY));


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			ViewY-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			ViewY+=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			ViewX-=ViewSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			ViewX+=ViewSpeed;


		wdApplyForce(W, vec2(0.f, 0.6f));
		if(Elastic != NULL) elasticResolve(Elastic);

		wdResolveVextex(W);

		for(i=0; i<10; i++)
		{
			wdResolveRigid(W);
			lnResolve(&L);
			wdHandleCollision(W, i==0);
		}

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
		for(ViewPort = 0; ViewPort < 2; ViewPort++) {
		if(ViewPort == 0)
		{
			glViewport(0.f, 0.f, WindowWidth, WindowHeight);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.f+ViewX, WindowWidth+ViewX, WindowHeight+ViewY, 0.f+ViewY, 0.0, 100.0);
		} else if(ViewPort == 1) {
			glViewport(650.f, 500.f, 100.f, 75.f);
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
				glVertex2f(vxGetPosition(elasticGetV1(Elastic)).x, vxGetPosition(elasticGetV1(Elastic)).y);
				glVertex2f(vxGetPosition(elasticGetV2(Elastic)).x, vxGetPosition(elasticGetV2(Elastic)).y);
			glEnd();
		}

		glDrawWorld(W);

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

		// Update the window
		window.display();
	}

	/*delPolygon(Rectangle);
	delPolygon(Poly);

	delVertex(V13);
	delVertex(V12);
	delVertex(V11);
	delVertex(V10);

	delVertex(V3);
	delVertex(V2);
	delVertex(V1);*/
	delWorld(W);

	return 0;
}


void glDrawPolygon(Polygon* P)
{
	unsigned int i;

	glColor4f(1.f, 1.f, 1.f, 0.3f);
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

	glColor3f(0.f, 0.f, 1.f);
	Vec2 Center = polyGetCenter(P);
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
	glColor4f(0.f, 1.f, 0.f, 0.2f);
	while(!nodeEnd(it))
	{
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

	it = lstFirst(&W->Polygons);
	while(!nodeEnd(it))
	{
		glDrawPolygon((Polygon*) nodeGetData(it));
		it = nodeGetNext(it);
	}
}


Vertex* GetNearest(World* W, float MouseX, float MouseY)
{
	if (lstEmpty(&W->Vertices)) return NULL;
	float dist = INFINITY;
	Vertex* grab=NULL;

	Node* it = lstFirst(&W->Vertices);
	while(!nodeEnd(it))
	{
		Vec2 vertex=vxGetPosition((Vertex*)nodeGetData(it));
		Vec2 v=vec2(MouseX - vertex.x, MouseY - vertex.y);
		if (vec2Length(v)<dist)
		{
			dist=vec2Length(v);
			grab=(Vertex*)nodeGetData(it);
		}

		it = nodeGetNext(it);
	}

	return grab;

}
