#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <Rendering/OpenGL.h> // Pour le type Texture (adapté à l'API)
#include <Physics/Physics.h>
#include <Objects/Player.h>
#include <string.h>
#include <Core/md5.h>
#include <Objects/Object.h>
#include <Objects/Flag.h>

/** @defgroup Level
 *
 * Décrit un niveau de jeu
 * Préfixe des méthodes : lvl
 *
 * Voir FormatLvl.txt pour des informations sur le format de fichier lvl
 **/

/** @brief Structure définissant un niveau de jeu
 *
 * Les fonctions de callback dépendent de l'API graphique
**/
typedef struct
{
	char Filename[255]; /**< Nom du fichier chargé **/
	char Name[255]; /**< Nom du Niveau **/
	char Desc[255]; /**< Description du Niveau**/
	char MD5[255]; /**< MD5 du fichier chargé **/
	World* W; /**< Monde physique **/
	Vec2 Spawn; /**< Point d'apparition du joueur **/
	Vec2 Goal; /**< Objectif **/
	Texture Background; /**< Texture de fond **/
	Texture Layer1; /**< Texture située immédiatement derrière les objets dynamiques **/
	Texture Layer2; /**< Texture située immédiatement devant les objets dynamiques **/
 	Texture Foreground; /**< Texture servant de premier plan **/
 	DynArr Textures; /**< Liste de textures utilisables par les objets **/
 	List Objects; /**< Liste d'objets texturés **/
 	Player* P1; /** Joueur 1 **/
	float DistBG, DistFG;
	Flag GoalFlag;
	Bool Finished;

	Texture (*lvlTexLoad)(const char* Path); /** Pointeur de fonction servant à charger en mémoire une texture **/
	void (*lvlTexFree)(Texture Img); /** Pointeur de fonction libèrant une texture **/
	void (*lvlDisplayTex)(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL); /** Pointeur de fonction affichant une texture **/
	void (*lvlDispTexPoly)(Texture T, Polygon* P, List* L); /** Pointeur de fonction affichant une texture sur un polygon **/
	void (*lvlDispFlag)(Flag* F, float X, float Y);
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

/** @brief Accesseur de Filename
**/
const char* lvlGetFilename(const Level* lvl);

/** @brief Accesseur de Name
**/
const char* lvlGetName(const Level* lvl);

/** @brief Accesseur de Desc
**/
const char* lvlGetDesc(const Level* lvl);

/** @brief Accesseur de MD5
**/
const char* lvlGetMD5(const Level* lvl);

/** @brief Accesseur de W
**/
World* lvlGetWorld(const Level* Lvl);

/** @brief Accesseur de Spawn
**/
Vec2 lvlGetSpawn(const Level* lvl);

/** @brief Accesseur de Goal
**/
Vec2 lvlGetGoal(const Level* lvl);

/** @brief Accesseur de P1
**/
Player* lvlGetP1(const Level* lvl);

/** @brief Accesseur de Finished
**/
Bool lvlIsFinished(const Level* lvl);

/** @brief Mutateur de Finished
**/
void lvlSetFinished(Level* Lvl, Bool B);

/** @brief Mutateur de DistBG
**/
void lvlSetDistBG(Level* lvl, float F);

/** @brief Mutateur de DistFG
**/
void lvlSetDistFG(Level* lvl, float F);

/** @brief Mutateur de Name
**/
void lvlSetName(Level* lvl, char* Name);

/** @brief Mutateur de Desc
**/
void lvlSetDesc(Level* lvl, char* Desc);

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
 * @param Paused Le monde est il en pause?
**/
void lvlUpdate(Level* Lvl, Bool Paused);

/** @brief Retourne vrai si l'objectif est atteint par le joueur
 *
**/
Bool lvlIsGoalReached(const Level* L);

/** @brief Affiche le fond
 *
 * @param Lvl Niveau à afficher
 * @param X Coordonnée X du point le plus en haut à gauche affiché
 * @param Y Coordonnée Y du point le plus en haut à gauche affiché
 * @param W Largeur de la portion du niveau affichée
 * @param H Hauteur de la portion du niveau affichée
**/
void lvlDisplayBG(const Level* Lvl, float X, float Y, float W, float H);

/** @brief Affiche la première couche (fond) de décors
**/
void lvlDisplayL1(const Level* Lvl);

/** @brief Affiche la deuxième couche (front) de décors
**/
void lvlDisplayL2(const Level* Lvl);

/** @brief Affiche le Foreground
 *
 * @param Lvl Niveau à afficher
 * @param X Coordonnée X du point le plus en haut à gauche affiché
 * @param Y Coordonnée Y du point le plus en haut à gauche affiché
 * @param W Largeur de la portion du niveau affichée
 * @param H Hauteur de la portion du niveau affichée
**/
void lvlDisplayFG(const Level* Lvl, float X, float Y, float W, float H);

/** @brief Affich un objet texturé
**/
void lvlDisplayObj(const Level* Lvl, Object* Obj);

/** @brief Affiche les objets texturés
**/
void lvlDispAllObj(Level* Lvl);

/** @brief Affiche la drapeau marquant la fin du niveau
**/
void lvlDispGoalFlag(Level* Lvl);

/** @brief Ajoute un objet texturé
**/
void lvlAddObject(Level* Lvl, Object * Obj);

/** @brief Supprime un objet texturé
**/
void lvlDelObject(Level* Lvl, Object * Obj);

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

