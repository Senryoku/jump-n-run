#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <Rendering/OpenGL.h> // Pour le type Texture (adapté à l'API)
#include <Physics/Physics.h>
#include <Objects/Player.h>
#include <string.h>

/** @defgroup Level
 *
 * Décrit un niveau de jeu
 * Préfixe des méthodes : lvl
 * @todo Chargement des Textures depuis le fichier de description du niveau
 **/

/** @brief Structure définissant un niveau de jeu
 *
 * Les fonctions de callback dépendent de l'API graphique
**/
typedef struct
{
	World* W; /**< Monde physique **/
	Vec2 Spawn; /**< Point d'apparition du joueur **/
	Vec2 Goal; /**< Objectif **/
	Texture Background; /**< Texture de fond **/
	Texture Layer1; /**< Texture située immédiatement derrière les objets dynamiques **/
	Texture Layer2; /**< Texture située immédiatement devant les objets dynamiques **/
 	Texture Foreground; /**< Texture servant de premier plan **/
 	DynArr Textures; /**< Liste de textures utilisables par les objets **/
 	DynArr Objects; /**< Liste d'objets texturés **/
 	Player* P1; /** Joueur 1 **/

	Texture (*lvlTexLoad)(const char* Path); /** Pointeur de fonction servant à charger en mémoire une texture **/
	void (*lvlTexFree)(Texture Img); /** Pointeur de fonction libèrant une texture **/
	void (*lvlDisplayTex)(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL); /** Pointeur de fonction affichant une texture **/
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

void lvlDisplayL1(Level* Lvl);

void lvlDisplayL2(Level* Lvl);

void lvlDisplayFG(Level* Lvl);

typedef enum enum_objets
{
	o_end,
	o_poly,
	o_vertex,
	o_rigid,
	o_elastic,
	o_texture,
	o_object
} e_led_objets;



/** @}
**/

#endif

