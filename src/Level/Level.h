#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <Physics/Physics.h>
#include <Objects/Player.h>
#include <string.h>
#include <Core/md5.h>
#include <Objects/Object.h>
#include <Objects/Flag.h>

/** @defgroup Level Level
 *
 * Décrit un niveau de jeu
 * Préfixe des méthodes : lvl
 *
 * Voir FormatLvl.txt pour des informations sur le format de fichier lvl
 **/

struct s_SharedResources;

/** @brief Structure définissant un niveau de jeu
 *
 * Les fonctions de callback dépendent de l'API graphique
**/
typedef struct s_Level
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
	Texture VoidTex; ///<Texture vide utilisée par défaut
 	List Objects; /**< Liste d'objets texturés **/
 	Player* P1; /**< Joueur 1 **/
	float DistBG; /**< Distance entre les Layer et le Background **/
	float DistFG; /**< Distance entre les Layer et le Foreground **/
	Flag GoalFlag; /**< Drapeau indiquant la fin du niveau **/
	Bool Finished; /**< Vaut vrai si le joueur a atteint le but du niveau **/

	Texture (*lvlTexLoad)(const char* Path); /**< Pointeur de fonction servant à charger en mémoire une texture **/
	void (*lvlTexFree)(Texture Img); /**< Pointeur de fonction libèrant une texture **/
	void (*lvlDisplayTex)(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL); /**< Pointeur de fonction affichant une texture **/
	void (*lvlDispTexPoly)(Texture T, Polygon* P, List* L); /**< Pointeur de fonction affichant une texture sur un polygon **/
	void (*lvlDispFlag)(Flag* F, float X, float Y);/**< Pointeur de fonction affichant le drapeau **/

	void (*lvlDispPlayer)(Player* P, s_SharedResources* SR);/**< Pointeur de fonction affichant le joueur **/
	void (*lvlDispGrass)(Polygon* P, s_SharedResources* SR);/**< Pointeur de fonction affichant l'herbe **/
	void (*lvlDispRope)(const Elastic* E, s_SharedResources* SR); /**< Pointeur de fonction affichant un elastic **/
	void (*lvlDispChain)(const Rigid* R, s_SharedResources* SR);/**< Pointeur de fonction affichant un rigid **/
	void (*lvlDispSpawn)(Vec2 V, s_SharedResources* SR); /**< Pointeur de fonction affichant le spawn **/
	void (*lvlDispGrab)(Vec2, Vec2, float, s_SharedResources*);/**< Pointeur de fonction affichant le grab du joueur **/
} Level;

/** @brief Constructeur
 * @param[in] Width largeur du niveau
 * @param[in] Height hauteur du niveau
 * @return instance initialisée d'un niveau
**/
Level* newLevel(float Width, float Height);

/** @brief Initialise Lvl
 * @param[in,out] Lvl Level auquel s'applique la fonction
 * @param[in] Width largeur du niveau
 * @param[in] Height hauteur du niveau
**/
void lvlInit(Level* Lvl, float Width, float Height);

/** @brief Libère les ressources utilisées par Lvl
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlFree(Level* Lvl);

/** @brief Destructeur
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void delLevel(Level* lvl);

/** @brief Accesseur de Filename
 * @param[in] Lvl Level auquel s'applique la fonction
**/
const char* lvlGetFilename(const Level* lvl);

/** @brief Accesseur de Name
 * @param[in] Lvl Level auquel s'applique la fonction
**/
const char* lvlGetName(const Level* lvl);

/** @brief Accesseur de Desc
 * @param[in] Lvl Level auquel s'applique la fonction
**/
const char* lvlGetDesc(const Level* lvl);

/** @brief Accesseur de MD5
 * @param[in] Lvl Level auquel s'applique la fonction
**/
const char* lvlGetMD5(const Level* lvl);

/** @brief Accesseur de W
 * @param[in] Lvl Level auquel s'applique la fonction
**/
World* lvlGetWorld(const Level* Lvl);

/** @brief Accesseur de Spawn
 * @param[in] Lvl Level auquel s'applique la fonction
**/
Vec2 lvlGetSpawn(const Level* lvl);

/** @brief Accesseur de Goal
 * @param[in] Lvl Level auquel s'applique la fonction
**/
Vec2 lvlGetGoal(const Level* lvl);

/** @brief Retourne un itérateur sur les Objets de Lvl
 * @param[in] Lvl Level auquel s'applique la fonction
**/
Node* lvlGetObjIt(Level* lvl);

/** @brief Accesseur de P1
 * @param[in] Lvl Level auquel s'applique la fonction
**/
Player* lvlGetP1(const Level* lvl);

/** @brief Accesseur de DistBG
 * @param[in] Lvl Level auquel s'applique la fonction
**/
float lvlGetDistBG(const Level* Lvl);

/** @brief Accesseur de DistFG
 * @param[in] Lvl Level auquel s'applique la fonction
**/
float lvlGetDistFG(const Level* Lvl);

/** @brief Accesseur de Finished
 * @param[in] Lvl Level auquel s'applique la fonction
**/
Bool lvlIsFinished(const Level* lvl);

/** @brief Mutateur de Finished
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetFinished(Level* Lvl, Bool B);

/** @brief Mutateur de DistBG
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetDistBG(Level* lvl, float F);

/** @brief Mutateur de DistFG
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetDistFG(Level* lvl, float F);

/** @brief Mutateur de Name
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetName(Level* lvl, const char* Name);

/** @brief Mutateur de Desc
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetDesc(Level* lvl, const char* Desc);

/** @brief Mutateur de Spawn
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetSpawn(Level* lvl, Vec2 S);

/** @brief Mutateur de Goal
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlSetGoal(Level* lvl, Vec2 G);

/** @brief Charge un niveau à partir d'un fichier
 * @param[in,out] Lvl Level auquel s'applique la fonction
 *
**/
Bool lvlLoad(Level* Lvl, const char* File);

/** @brief Initialise un niveau APRES son chargement complet
 *
 * Le niveau est alors prêt à être joué
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlLoadedInit(Level* Lvl);

/** @brief Avance d'une frame.
 *
 * Met à jour la simulation physique.
 * Le(s) joueur(s) pouvant avoir des réactions spécifiques,
 * notament aux collisions, ils ne sont pas ajoutés directement à la
 * simulation, il faut donc effectuer certaines vérification manuellement.
 * @param[in,out] Lvl Level à mettre à jour
 * @param[in] Paused Le monde est il en pause?
 * @param[in,out] SR Resources Partagées
**/
void lvlUpdate(Level* Lvl, Bool Paused, s_SharedResources* SR);

/** @brief Retourne vrai si l'objectif est atteint par le joueur
 * @param Lvl Level auquel s'applique la fonction
 *
**/
Bool lvlIsGoalReached(const Level* L);

/** @brief Affiche le joueur
 * @param[in] Lvl Level auquel s'applique la fonction
 * @param SR Resources partagées
 **/
void lvlDisplayPlayer(const Level* Lvl, s_SharedResources* SR);

/** @brief Affiche les elastics avec une image
 * @param[in] Lvl Level auquel s'applique la fonction
 * @param SR Resources partagées
 **/
void lvlDisplayElastics(const Level* Lvl, s_SharedResources* SR);

/** @brief Affiche les Rigids avec une image
 * @param[in] Lvl Level auquel s'applique la fonction
 * @param SR Resources partagées
 **/
void lvlDisplayRigids(const Level* Lvl, s_SharedResources* SR);

/** @brief Affiche le fond
 *
 * @param[in] Lvl Niveau à afficher
 * @param X Coordonnée X du point le plus en haut à gauche affiché
 * @param Y Coordonnée Y du point le plus en haut à gauche affiché
 * @param W Largeur de la portion du niveau affichée
 * @param H Hauteur de la portion du niveau affichée
**/
void lvlDisplayBG(const Level* Lvl, float X, float Y, float W, float H);

/** @brief Affiche la première couche (fond) de décors
 * @param[in] Lvl Level auquel s'applique la fonction
**/
void lvlDisplayL1(const Level* Lvl);

/** @brief Affiche la deuxième couche (front) de décors
 * @param[in] Lvl Level auquel s'applique la fonction
**/
void lvlDisplayL2(const Level* Lvl);

/** @brief Affiche le Foreground
 *
 * @param[in] Lvl Niveau à afficher
 * @param X Coordonnée X du point le plus en haut à gauche affiché
 * @param Y Coordonnée Y du point le plus en haut à gauche affiché
 * @param W Largeur de la portion du niveau affichée
 * @param H Hauteur de la portion du niveau affichée
**/
void lvlDisplayFG(const Level* Lvl, float X, float Y, float W, float H);

/** @brief Affich un objet texturé
 * @param[in] Lvl Level auquel s'applique la fonction
**/
void lvlDisplayObj(const Level* Lvl, Object* Obj);

/** @brief Affiche les objets texturés
 * @param[in] Lvl Level auquel s'applique la fonction
**/
void lvlDispAllObj(Level* Lvl);

/** @brief Affiche l'herbe sur les object qui utilisent la texture par défaut d'index 0
 * @param[in] Lvl Level auquel s'applique la fonction
 * @param[in] SR Resources partagées
 **/
void lvlDisplayGrass(Level* Lvl, s_SharedResources* SR);

/** @brief Affiche la drapeau marquant la fin du niveau
 * @param[in] Lvl Level auquel s'applique la fonction
 **/
void lvlDispGoalFlag(Level* Lvl);

/** @brief Affiche le spawn du niveau
 * @param[in] Lvl Level auquel s'applique la fonction
 * @param[in] SR Resources partagées
 **/
void lvlDispSpawn(Level* Lvl, s_SharedResources* SR);

/** @brief Ajoute un objet texturé
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlAddObject(Level* Lvl, Object * Obj);

/** @brief Supprime un objet texturé
 * @param[in,out] Lvl Level auquel s'applique la fonction
**/
void lvlDelObject(Level* Lvl, Object * Obj);

/** @brief Recherche un objet texturé de forme P
 *
 * @param[in] Lvl Niveau où cherche l'objet
 * @param P Polygon servant de forme à l'objet recherché
 * @return Object* si un objet de la forme P est trouvé, NULL sinon
**/
Object* lvlGetObjFromShape(Level* Lvl, Polygon* P);

void lvlCreateTexListForPolygon(Polygon* P, List* l);

/** @brief Ajoute une texture à la liste de textures
 * @param[in,out] Lvl Level auquel s'applique la fonction
 * @param T Texture
 **/
void lvlAddTexture(Level* Lvl, Texture T);

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

/** @brief Test de Regression
 *
 * Principalement pour tester sous Valgrind
 **/
void lvlRegressionTest();

/** @}
**/

#endif

