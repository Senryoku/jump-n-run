#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Physics/Physics.h>

typedef GLuint Texture;

void glDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A);
void glDrawVertex(Vertex* V, float R, float G, float B, float A);
void glDrawElastic(Elastic* E);
void glDrawRigid(Rigid* E);
void glDrawPolygon(Polygon *P);
void glDrawCircle(float x, float y, float radius);

/** @brief Charge une texture en mémoire
 *
 * @param Path Chemin du fichier image
**/
Texture glTexLoad(const char* Path);

/** @brief Libère la texture OpenGL
 *
 * @param T Texture à libérer
**/
void glTexFree(Texture T);

/** @brief Affiche la texture à l'écran
 *
 * @param T Texture OpenGL
 * @param TexUL Vecteur Normalisé désignant un point de la texture
 * @param TexUR Vecteur Normalisé désignant un point de la texture
 * @param TexDR Vecteur Normalisé désignant un point de la texture
 * @param TexDL Vecteur Normalisé désignant un point de la texture
 * @param UL Vec2 désignant le point de la scène ou sera dessiné TexUL
 * @param UR Vec2 désignant le point de la scène ou sera dessiné TexUR
 * @param DR Vec2 désignant le point de la scène ou sera dessiné TexDR
 * @param DL Vec2 désignant le point de la scène ou sera dessiné TexDL
**/
void glDisplayTex(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL);


#endif
