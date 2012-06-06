#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <SFML/Graphics.hpp>
#include <Physics/Physics.h>
#include <Objects/Cloth.h>
#include <Menu/Menu.h>
#include <SFML/OpenGL.hpp>
#include <Game/SharedResources.h>
#include <Objects/Player.h>
#include <sstream>

typedef GLuint Texture;
struct s_Flag;
struct s_Level;

void glDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A);
void glDrawVertex(Vertex* V, float R, float G, float B, float A);
void glDrawElastic(Elastic* E);
void glDrawRigid(Rigid* E);
void glDrawPolygon(Polygon *P);
void glDrawCircle(float x, float y, float radius);

//One ne peut pas combiner OGL et SFML correctement donc il faut faire en deux étapes...
void glDrawMenuItems(SharedResources* SR, sf::RenderTarget& win, Menu* M, float ViewX, float ViewY, float ViewWidth, float ViewHeight);
void glDrawMenuBox(SharedResources* SR, sf::RenderTarget& win, Menu* M, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

void glDrawCloth(Cloth* C, Texture T);
void glDrawFPS(SharedResources* SR, sf::RenderTarget& win, const std::string& FPS);
void glDrawPolyFromList(List* L, Vec2 MousePos);
void glDrawBox(SharedResources* SR, Vec2 Position, Vec2 Size, int SubAnim);
void glDrawTitleBox(SharedResources* SR, Vec2 Position, Vec2 Size);

void glDrawMinimap(s_Level* Lvl, SharedResources* SR, const sf::RenderTarget& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight);

void glDrawCursor(sf::RenderTarget& win, float ViewWidth, float ViewHeight, float MouseX, float MouseY, sf::Sprite& sprCursor);

void glDispPlayer(Player* P, SharedResources* SR);

void glDispGrass(Polygon* P, SharedResources* SR);

void glDispRope(const Elastic* E, SharedResources* SR);

void glDispChain(const Rigid* R, SharedResources* SR);

void glDispSpawn(Vec2 Pos, SharedResources* SR);

void glDispGrab(Vec2 V1, Vec2 V2, float Lenght, SharedResources* SR);


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

/** @brief Affiche une texture sur un Polygon
 *
 * Utilise les positions des Vertex du Polygon pour bind la texture à l'écran
 * @param T Texture OpenGL
 * @param P Polygon sur lequel afficher T
 * @param L List de Vec2, coordonnées ordonées des points de liaison sur la texture
 **/
void glDispTexPoly(Texture T, Polygon* P, List* L);

/** @brief Affiche un Flag
 *
**/
void glDispFlag(s_Flag* F, float X, float Y);

/** @brief Affiche le Temps
 *
**/
void glDrawTime(SharedResources* SR, sf::RenderTarget& win, unsigned int Time);
#endif

