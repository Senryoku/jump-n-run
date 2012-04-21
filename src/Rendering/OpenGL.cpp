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
	Bool LoadSuccess = 0;
	GLuint texture = 0;

	sf::Image image;
	#ifdef SFML_SYSTEM_MACOS
	LoadSuccess = image.loadFromFile(Path); //À rajouter ResourcePath selon compilation
	#else
	LoadSuccess = image.loadFromFile(Path);
	#endif

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if(LoadSuccess)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		// Génération de Mipmaps... Pas très utile en 2D, non ?
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getWidth(), image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		/* Texture transparente si le fichier n'a pas pu être chargé */
		GLubyte TexNull[4] =
		{
			0,0,0,0
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexNull);
	}
	glDisable(GL_TEXTURE_2D);

	return texture;
}

void glTexFree(Texture T)
{
	glDeleteTextures(1, &T);
}

void glDisplayTex(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL)
{
	glColor4f(1.f, 1.f, 1.f, 1.f); /* Chaque pixel de la texture est multiplié par cette couleur */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	glBegin(GL_QUADS);
	glTexCoord2d(TexUL.x, TexUL.y);  glVertex2f(UL.x, UL.y);
	glTexCoord2d(TexUR.x, TexUR.y);  glVertex2f(UR.x, UR.y);
	glTexCoord2d(TexDR.x, TexDR.y);  glVertex2f(DR.x, DR.y);
	glTexCoord2d(TexDL.x, TexDL.y);  glVertex2f(DL.x, DL.y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void glDispTexPoly(Texture T, Polygon* P, List* L)
{
	Node* it;
	unsigned int i = 0;
	Vec2 VPos, CoordTex;
	if(polyGetVxCount(P) != lstCount(L)) return;

	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	glBegin(GL_POLYGON);
	it = lstFirst(L);
	while(!nodeEnd(it))
	{
		VPos = vxGetPosition(polyGetVertex(P, i));
		CoordTex = *((Vec2*) nodeGetData(it));
		glTexCoord2d(CoordTex.x, CoordTex.y);  glVertex2f(VPos.x, VPos.y);
		it = nodeGetNext(it);
		i++;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void glDrawMenu(sf::RenderTarget& win, Menu* M, float ViewX, float ViewY)
{
	MenuOfItems* moi = mnGetCurrentMenu(M);
	Vec2 Size = moiGetSize(moi),
	Position = mnGetPosition(M);
	unsigned short i;
	MenuItem* I;
	float MaxTextWidth = 0.f;
	


	glPushMatrix();

	float heigth = 5.f; //offset
	for (i=0; i<moiGetItemCount(moi); i++)
	{
		I = moiGetItem(moi, i);
		heigth+=mnGetItemHeight(M)*(*mniGetZoom(I));
	}


	//On dessine le carré, ça sera fai avec des images plus tard
	glColor4f(0.2f, 0.2f, 0.2f, 0.75f);
	glTranslatef(ViewX, ViewY, 0.f);
	glBegin(GL_QUADS);
	glVertex2f(Position.x, Position.y-5.f);
	glVertex2f(Position.x + Size.x, Position.y-5.f);
	glVertex2f(Position.x +Size.x, Position.y + heigth);
	glVertex2f(Position.x, Position.y + heigth);
	glEnd();

	sf::Text ItemText;
	ItemText.setFont(sf::Font::getDefaultFont());

	float yoffset = 5.f, selOffset;;
	for (i=0; i<moiGetItemCount(moi); i++)
	{
		I = moiGetItem(moi, i);
		if (strcmp(mniGetText(I), "")==0)
			ItemText.setString(" "); /* une chaîne vide donne une erreur */
		else
			ItemText.setString(std::string(mniGetText(I)));

		switch (mniGetType(I))
		{
			case ITEM_CHECKBOX:
				if (*(Bool*)mniGetData(I))
					ItemText.setString(ItemText.getString() + ": Yes");
				else
					ItemText.setString(ItemText.getString() + ": No");
				break;

			case ITEM_INPUT:
			case ITEM_INPUT_VALUE:
				ItemText.setString(ItemText.getString() + ": " + *(std::string*)mniGetData(I));
				/** @todo bug quand on fait backspace alors que la chaine est vide, étrange... */
				break;
			case ITEM_VALUE:
				char ValueText[300];
				sprintf(ValueText, ": %.f", *(float*)mniGetData(I));
				ItemText.setString(ItemText.getString() + std::string(ValueText));
				break;
			default:
				break;
		}

		ItemText.setScale(1.f, 1.f);
		ItemText.setPosition(Position.x+5.f, Position.y+yoffset-1.5f-10.f);

		ItemText.setScale(*mniGetZoom(I), *mniGetZoom(I));

		if (i != moiGetItemSelectedID(moi))
		{
			win.pushGLStates();
			ItemText.setColor(sf::Color(0,0,0));
			win.draw(ItemText);
			ItemText.move(0.f, 1.5f);
			ItemText.setColor(sf::Color(0,255,255));
			win.draw(ItemText);
			win.popGLStates();
		}
		else
			selOffset = yoffset;

		yoffset+=ItemText.getCharacterSize()*(*mniGetZoom(I));
		
		float Width = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
		if (Width>MaxTextWidth)
			MaxTextWidth=Width;
		
		mnSetItemHeight(M, ItemText.getCharacterSize());

	}

	if (moiGetItemSelectedID(moi) != INVALID_ITEM_ID)
	{
		I = moiGetItemSelected(moi);
		if (strcmp(mniGetText(I), "")==0)
			ItemText.setString(" "); /* une chaîne vide donne une erreur */
		else
			ItemText.setString(std::string(mniGetText(I)));
		
		switch (mniGetType(I))
		{
			case ITEM_CHECKBOX:
				if (*(Bool*)mniGetData(I))
					ItemText.setString(ItemText.getString() + ": Yes");
				else
					ItemText.setString(ItemText.getString() + ": No");
				break;
				
			case ITEM_INPUT:
			case ITEM_INPUT_VALUE:
				ItemText.setString(ItemText.getString() + ": " + *(std::string*)mniGetData(I));
				break;
			case ITEM_VALUE:
				char ValueText[300];
				sprintf(ValueText, ": %.f", *(float*)mniGetData(I));
				ItemText.setString(ItemText.getString() + std::string(ValueText));
				break;
			default:
				break;
		}
		
		ItemText.setScale(1.f, 1.f);
		ItemText.setPosition(Position.x+5.f, Position.y+selOffset-1.5f-10.f);
		
		yoffset+=mnGetItemHeight(M)*(*mniGetZoom(I));
		ItemText.setScale(*mniGetZoom(I), *mniGetZoom(I));
		
		win.pushGLStates();
		ItemText.setColor(sf::Color(0,0,0));
		win.draw(ItemText);
		ItemText.move(0.f, 1.5f);
		ItemText.setColor(sf::Color(0,255,255));
		win.draw(ItemText);
		win.popGLStates();
		
		float Width = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
		if (Width>MaxTextWidth)
			MaxTextWidth=Width;
		
	}
	
	moiSetSize(moi, vec2(MaxTextWidth, Size.y));

	glPopMatrix();
}


void glDrawCloth(Cloth* C, Texture T)
{
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	//vxGetPosition(C->Points
	glBegin(GL_QUADS);
	unsigned int i, j;
	
	for (i=0; i<C->HCells-1; i++)
		for (j=0; j<C->VCells-1; j++)
		{
			//if (i>=C->HCells-1 || j>=C->VCells-1) continue;
			
			//left top
			glTexCoord2f(static_cast<float>(i)/static_cast<float>(C->HCells-1), static_cast<float>(j)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i][j]).x, vxGetPosition(C->Points[i][j]).y);
			
			//right top
			glTexCoord2f(static_cast<float>(i+1)/static_cast<float>(C->HCells-1), static_cast<float>(j)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i+1][j]).x, vxGetPosition(C->Points[i+1][j]).y);
			
			//right bottom
			glTexCoord2f(static_cast<float>(i+1)/static_cast<float>(C->HCells-1), static_cast<float>(j+1)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i+1][j+1]).x, vxGetPosition(C->Points[i+1][j+1]).y);
			
			//Left bottom
			glTexCoord2f(static_cast<float>(i)/static_cast<float>(C->HCells-1), static_cast<float>(j+1)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i][j+1]).x, vxGetPosition(C->Points[i][j+1]).y);
		}
	
	glEnd();
	
}
