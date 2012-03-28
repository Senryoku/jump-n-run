#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Level/Level.h"

/** @defgroup Game
 *
 * Structure du jeu
 * Préfixe des méthodes : gm
 **/
typedef struct
{
	Level* Lvl;
} Game;

void gmInit(Game*);

void gmMenu(Game*);

void gmPlay(Game*);

void gmSetLvl(Game*, Level*);

/** @}
**/

/* Temporaire : Game n'a pas besoin des fonctions de debug */
void glDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A);
void glDrawVertex(Vertex* V, float R, float G, float B, float A);
void glDrawElastic(Elastic* E);
void glDrawRigid(Rigid* E);
void glDrawPolygon(Polygon *P);

#endif

