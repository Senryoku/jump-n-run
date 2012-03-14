#include <Physics/Physics.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

void glDrawPolygon(Polygon *P);

int main(int argc, char** argv)
{
	unsigned int i;

	vec2TestRegression();

	vxTestRegression();

	polyTestRegression();

	World* W = newWorld(800.f, 600.f);

	Vertex* V1 = newVertex();
	vxSetPosition(V1, vec2(0.f, 0.f));
	Vertex* V2 = newVertex();
	vxSetPosition(V2, vec2(30.f, 0.f));
	Vertex* V3 = newVertex();
	vxSetPosition(V3, vec2(0.f, 50.f));
	wdAddVertex(W, V1); wdAddVertex(W, V2); wdAddVertex(W, V3);

	Vertex* V10 = newVertex();
	vxSetPosition(V10, vec2(50.f, 0.f));
	Vertex* V11 = newVertex();
	vxSetPosition(V11, vec2(100.f, 0.f));
	Vertex* V12 = newVertex();
	vxSetPosition(V12, vec2(100.f, 50.f));
	Vertex* V13 = newVertex();
	vxSetPosition(V13, vec2(50.f, 50.f));
	wdAddVertex(W, V10); wdAddVertex(W, V11); wdAddVertex(W, V12); wdAddVertex(W, V13);

	Polygon* Poly = newPolygon(3, V1, V2, V3);
	Polygon* Rectangle = polyRectangle(V10, V11, V12, V13);
	wdAddPolygon(W, Rectangle);

	V10 = newVertex();
	vxSetPosition(V10, vec2(50.f, 100.f));
	V11 = newVertex();
	vxSetPosition(V11, vec2(100.f, 100.f));
	V12 = newVertex();
	vxSetPosition(V12, vec2(100.f, 150.f));
	V13 = newVertex();
	vxSetPosition(V13, vec2(50.f, 150.f));
	wdAddVertex(W, V10); wdAddVertex(W, V11); wdAddVertex(W, V12); wdAddVertex(W, V13);
	Polygon* Rectangle2 = polyRectangle(V10, V11, V12, V13);
	wdAddPolygon(W, Rectangle2);

	wdAddPolygon(W, Poly);

	sf::RenderWindow window(sf::VideoMode(800, 600), "window");
	window.setFramerateLimit(60.f);

	glMatrixMode(GL_PROJECTION); //On va ainsi définir le viewport
	glLoadIdentity();
	glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 100.0);
	glDisable(GL_DEPTH_TEST);



	// Start the game loop
	while (window.isOpen())
	{
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
		}

		wdApplyForce(W, vec2(0.f, 2.f));
		wdResolveVextex(W);
		for(i=0; i<10; i++)
		{
			wdResolveRigid(W);
			wdHandleCollision(W);
		}

		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		glClearColor(0.0f, 0.f, 0.f, 1.f); //Ici optionnel car par défaut couleur est rouge
										   //glClear(GL_DEPTH_BUFFER_BIT);

		//On prepare la projection
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(1.f, 1.f, 1.f);
		glDrawPolygon(Rectangle);
		glDrawPolygon(Rectangle2);
		glDrawPolygon(Poly);

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

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_POLYGON);
	for(i = 0; i<daGetSize(&P->Vertices); i++)
		glVertex2f(vxGetPosition((Vertex*)daGet(&P->Vertices, i)).x, vxGetPosition((Vertex*)daGet(&P->Vertices, i)).y);
	glEnd();

	glColor3f(1.f, 0.f, 0.f);
	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		glBegin(GL_LINES);
			glVertex2f(vxGetPosition(rdGetV1((Rigid*)daGet(&P->Rigids, i))).x, vxGetPosition(rdGetV1((Rigid*)daGet(&P->Rigids, i))).y);
			glVertex2f(vxGetPosition(rdGetV2((Rigid*)daGet(&P->Rigids, i))).x, vxGetPosition(rdGetV2((Rigid*)daGet(&P->Rigids, i))).y);
		glEnd();
	}

	glColor3f(0.f, 0.f, 1.f);
	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
		glBegin(GL_LINES);
			glVertex2f(vxGetPosition(rdGetV1((Rigid*)daGet(&P->InternalRigids, i))).x, vxGetPosition(rdGetV1((Rigid*)daGet(&P->InternalRigids, i))).y);
			glVertex2f(vxGetPosition(rdGetV2((Rigid*)daGet(&P->InternalRigids, i))).x, vxGetPosition(rdGetV2((Rigid*)daGet(&P->InternalRigids, i))).y);
		glEnd();
	}
}
