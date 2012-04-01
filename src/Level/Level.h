#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Rendering/OpenGL.h" // Pour les types
#include "Physics/Physics.h"
#include "Objects/Player.h"

/** @defgroup Level
 *
 * Décrit un niveau de jeu
 * Préfixe des méthodes : lvl
 **/

typedef struct
{
	World* W;
	Vec2 Spawn;
	Vec2 Goal;
	Texture Background;
	Texture Layer1; // Juste derrière les objets
	Texture Layer2; // Juste devant les objets
 	Texture Foreground;
 	Player* P1;

	Texture (*lvlTexLoad)(char* Path);
	void (*lvlTexFree)(Texture Img);
	void (*lvlDisplayTex)(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL);
} Level;

/** @brief Constructeur
**/
Level* newLevel(float Width, float Height);

/** @brief Initialise Lvl
**/
void lvlInit(Level* Lvl, float Width, float Height);

/** @brief Libère les ressources utilisées par Lvl
**/
void lvlFree(Level* Lvl);

/** @brief Destructeur
**/
void delLevel(Level* lvl);

/** @brief Charge un niveau à partir d'un fichier
 *
**/
Bool lvlLoad(Level* Lvl, const char* File);

/** @brief Initialise un niveau APRES son chargement complet
 *
 * Le niveau est alors prêt à être joué
**/
void lvlLoadedInit(Level* Lvl);

/** @brief Avance d'une frame.
 *
 * Met à jour la simulation physique.
 * Le(s) joueur(s) pouvant avoir des réactions spécifiques,
 * notament aux collisions, ils ne sont pas ajoutés directement à la
 * simulation, il faut donc effectuer certaines vérification manuellement.
 * @param Lvl Level à mettre à jour
**/
void lvlUpdate(Level* Lvl);

/** @brief Accesseur de W
**/
World* lvlGetWorld(Level* Lvl);

void lvlDisplayBG(Level* Lvl);

typedef enum enum_objets
{
	o_end,
	o_poly,
	o_vertex,
	o_rigid,
	o_elastic
} e_led_objets;

 typedef struct {
	 Vertex* V;
	 unsigned long long ID;
} ItemVertex ;

Vertex* GetVertexFromID(List* L, unsigned long long ID);

/** @}
**/

#endif

