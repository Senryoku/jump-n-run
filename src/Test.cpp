#include <Physics/Physics.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

void glDrawPolygon(Polygon *P);

int main(int argc, char** argv)
{

	vec2TestRegression();

	vxTestRegression();

	polyTestRegression();
	
	Vertex* V1 = newVertex();
	Vertex* V2 = newVertex();
	vxSetPosition(V2, vec2(30.f, 0.f));
	Vertex* V3 = newVertex();
	vxSetPosition(V3, vec2(0.f, 50.f));
	
	Vertex* V10 = newVertex();
	Vertex* V11 = newVertex();
	Vertex* V12 = newVertex();
	Vertex* V13 = newVertex();
	
	Polygon* Poly = newPolygon(3, V1, V2, V3);
	Polygon* Rectangle = polyRectangle(V10, V11, V12, V13);
	
	
	
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
	
		
		glClear(GL_COLOR_BUFFER_BIT); //On efface le fond. Color car on est en 2D
		glClearColor(0.0f, 0.f, 0.f, 1.f); //Ici optionnel car par défaut couleur est rouge
										   //glClear(GL_DEPTH_BUFFER_BIT);
		
		//On prepare la projection
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity(); 
		
		glColor3f(1.f, 1.f, 1.f);
		glDrawPolygon(Rectangle);
		glDrawPolygon(Poly);
		
		// Update the window
		window.display();
	}

	delPolygon(Rectangle);
	delPolygon(Poly);
	
	delVertex(V13);
	delVertex(V12);
	delVertex(V11);
	delVertex(V10);
	
	delVertex(V3);
	delVertex(V2);
	delVertex(V1);

	return 0;
}


void glDrawPolygon(Polygon* P)
{
	glBegin(GL_POLYGON);
	for (int i=0; i<daGetSize(&P->Vertices); i++)
		glVertex2f(vxGetPosition((Vertex*)daGet(&P->Vertices, i)).x, vxGetPosition((Vertex*)daGet(&P->Vertices, i)).y);
	glEnd();
}
