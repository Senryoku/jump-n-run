#include "OpenGL.h"

void glDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);
	glBegin(GL_LINES);
	glVertex2f(X1, Y1);
	glVertex2f(X2, Y2);
	glEnd();
}

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

	if(polyIsConvexe(P)) glColor4f(1.f, 1.f, 1.f, 0.2f);
		else glColor4f(1.f, 0.f, 0.f, 0.2f);
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

	/* BBox
	BBox B = polyGetBBox(P);
	glColor4f(1.f, 0.f, 0.f, 0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(B.Left, B.Top);
		glVertex2f(B.Right, B.Top);
		glVertex2f(B.Right, B.Bottom);
		glVertex2f(B.Left, B.Bottom);
	glEnd(); */
}

void glDrawCircle(float x, float y, float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i=0; i<=16; i++)
	{
		glVertex2f(radius*cos((2.0*M_PI)*(i/static_cast<double>(16))) + x,
				   radius*sin((2.0*M_PI)*(i/static_cast<double>(16))) + y);
	}
	glEnd();
}

Texture glTexLoad(const char* Path)
{
	GLuint texture;
	{
		sf::Image image;
		#ifdef SFML_SYSTEM_MACOS
		image.loadFromFile(Path); //À rajouter ResourcePath selon compilation 
		#else
		image.loadFromFile(Path);
		#endif

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getWidth(), image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	return texture;
}

void glTexFree(Texture T)
{
	glDeleteTextures(1, &T);
}

void glDisplayTex(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL)
{
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, T);
	glBegin(GL_QUADS);
	glTexCoord2d(TexUL.x, TexUL.y);  glVertex2f(UL.x, UL.y);
	glTexCoord2d(TexUR.x, TexUR.y);  glVertex2f(UR.x, UR.y);
	glTexCoord2d(TexDR.x, TexDR.y);  glVertex2f(DR.x, DR.y);
	glTexCoord2d(TexDL.x, TexDL.y);  glVertex2f(DL.x, DL.y);
	glEnd();
}
