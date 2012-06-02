#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <math.h>
#include <Core/DynArr.h>
#include <Core/Vec2.h>
#include <Core/Tools.h>
#include <Physics/Vertex.h>

/** @defgroup Animation Animation
 *
 * Permet d'animqer un squelette et donc le joueur.
 * Préfixe des méthodes : ani
 * @{
 **/

typedef unsigned char AnimType;
typedef unsigned short AnimTriggers; /**< Il faut utiliser les flags pour définir quels sont les parties du corps qui vont décider que l'animation doit passer à l'étape suivante **/

#define ANIM_ANGLES 0x00
#define ANIM_POSITIONS 0x01


/*#define ANIM_FREE 0x7fc00000 < Définie quand on ne tient pas en compte cette valeur. C'est un NaN. EDIT: Finalement ceci ne sert à rien car il considère que c'est un entier. De toutes façons NAN est définie dans math.h avec 
#if defined(__GNUC__)
#define NAN         __builtin_nanf("0x7fc00000") 
#else
#define NAN		__nan( )
#endif

**/

/** Flags pour décider qui va permettre l'animation d'avancer */

#define ANIM_NECK 0x0001
#define ANIM_LEFT_ARM1 0x0002
#define ANIM_LEFT_ARM2 0x0004
#define ANIM_RIGHT_ARM1 0x0008
#define ANIM_RIGHT_ARM2 0x0010
#define ANIM_LEFT_LEG1 0x0020
#define ANIM_LEFT_LEG2 0x0040
#define ANIM_RIGHT_LEG1 0x0080
#define ANIM_RIGHT_LEG2 0x0100
#define ANIM_HEAD 0x0200
#define ANIM_ALL_TRIGGERS 0xffff

/**
 * @brief permet d'accèder aux angles ou positions des structures AnimPositions et AnimAngles
 */

enum BodyParts {
	bpNeck = 0, ///<Donne accès à la tête
	bpLeftArm1 = 1, ///<Donne accès au bras gauche 1
	bpLeftArm2 = 2, ///<Donne accès au bras gauche 2
	bpRightArm1 = 3, ///<Donne accès au bras droit 1
	bpRightArm2 = 4,///<Donne accès au bras droit 2
	bpLeftLeg1 = 5,///<Donne accès à la jambe gauche 1
	bpLeftLeg2 = 6,///<Donne accès à la jambe gauche 2
	bpRightLeg1 = 7,///<Donne accès à la jambe droite 1
	bpRightLeg2 = 8,///<Donne accès à la jambe droite 2
	bpHeadLeft = 9, ///<c'est Head pour un polygones et headleft pour les vertex
	bpHeadRight = 10, ///< que pour les vertex
	bpBase = 11 ///<que pour les vertex
	};

/**
 * @brief State d'une animation contenant des positions
 *
 * Cette structure ne sert qu'à regrouper des données par conséquent aucune fonction pour modifier ou accèder à ses attributs existe. Il faut y accèder directement. Ceci est utilisé pour les différents states à sauvegarder dans une animation et aussi pour avoir un state et faire la mise à jour à partir d'une animation.
 */
typedef struct {
	Vec2 Positions[10]; ///<Tableau des différents positions
	unsigned int CurrentState; //<State courant
	Bool Ended; ///< Dit si l'animation s'est finie
	float Spd[20]; ///< Pour wobble
} AnimPositions;

/**
 * @brief State d'une animation contenant des angles
 *
 * Cette structure ne sert qu'à regrouper des données par conséquent aucune fonction pour modifier ou accèder à ses attributs existe. Il faut y accèder directement. Ceci est utilisé pour les différents states à sauvegarder dans une animation et aussi pour avoir un state et faire la mise à jour à partir d'une animation.
 */
typedef struct {
	float Angles[10]; ///<Tableau des différents angles
	unsigned int CurrentState; //<State courant
	Bool Ended; ///< Dit si l'animation s'est finie
	float Spd[10]; ///< Pour wobble
} AnimAngles;

/**
 * @brief Initialise à 0 un state d'animation d'angles
 *
 * @param[in,out] ang State auquel s'applique la fonction
 */
void animAnglesStatesInit(AnimAngles* ang);

/**
 * @brief Initialise à 0 un state d'animation de positions
 *
 * @param[in,out] pos State auquel s'applique la fonction
 */
void animPositionsStatesInit(AnimPositions* pos);

struct SPlayer;

/**
 * @brief Structure permettant de gérer une animation
 *
 * Cette structure permet de créer des animations à partir de states ou de lire des states directement depuis un fichier. Le format du fichier est le suivant:
 @code
 (nombre de states) (type d'animation) (Triggers) (repeat)
 (Libre?) (angles en degrès) #Commentaire
 ...
 @endcode
 Tout cela en enlevant les parenthèses. Les triggers sont définies dans Animation.h. 0 est une animation par angles et 1 un animation par positions. UTILISER PRÉFERABLEMENT DES ANIMATIONS PAR ANGLES car elles sont plus réalistes, de plus c'est la seule qui est affichée par OGL.
 */
typedef struct {
	AnimType Type; ///<Type de l'animation soit ANIM_ANGLES soit ANIM_POSITIONS
	AnimTriggers Triggers; ///< triggers qui permettent l'animation de change de state
	unsigned char TriggerCount; ///< nombre de triggers, permet de faire e changement de state
	DynArr* States; ///< States de l'animation
	Bool Repeat; ///< décide si l'animation se répète à la fin ou pas
	float Friction; ///<Friction de la fonction wobble
	float Force; ///<Force de la fonction wobble
	float Diff; /**< Différence absolue qui permet de savoir quand on passe à l'animation suivante **/
} Animation;

/** @brief Initialise une animation
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Type Type de l'animation
 * @param[in] Triggers triggers de l'animation
 * @param[in] Repeat dit si l'animation se répète
 */
void aniInit(Animation* A, AnimType Type, AnimTriggers Triggers, Bool Repeat);

/** @brief Libère une animation
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 */
void aniFree(Animation* A);

/** @brief Alloue et initialise une animation
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Type Type de l'animation
 * @param[in] Triggers triggers de l'animation
 * @param[in] Repeat dit si l'animation se répète
 * @return pointeur vers l'animation allouée
 */
Animation* newAnimation(AnimType Type, AnimTriggers Triggers, Bool Repeat);

/** @brief Libère et déalloue une animation
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 */
void delAnimation(Animation* A);

/** @brief Charge une animation (initialisée) depuis un fichier
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Type Type de l'animation
 * @param[in] Triggers triggers de l'animation
 * @param[in] Repeat dit si l'animation se répète
 * @return Vrai si erreur
 */
Bool aniLoadFromFile(Animation* A, const char* File);

/** @brief Reset la valeur de End d'une animation
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 */
void aniResetEnded(Animation* A);

/** @brief Accesseur de End
 *
 * @param[in] A Animation à laquelle s'applique la fonction
 * @return Valeur de End
 */
Bool aniIsEnded(const Animation* A);

/** @brief Accésseur du nombre de states
 *
 * @param[in] A Animation à laquelle s'applique la fonction
 * @return nombre de states
 */
unsigned int aniGetStatesCount(const Animation* A);

/** @brief Accesseur de CurentState
 *
 * @param[in] A Animation à laquelle s'applique la fonction
 * @return Valeur de CurrentState
 */
unsigned int aniGetCurrentState(const Animation* A);

/** @brief Mutateur de CurrentState
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Valeur du nouveau CurrentState
 */
void aniSetCurrentState(Animation* A, unsigned int State);

/** @brief Ajoute un position state
 *
 * cette fonction crée une copie
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Positions State de positions
 */
void aniAddPositionState(Animation* A, const AnimPositions* Positions);

/** @brief Ajoute un angles state
 *
 * cette fonction crée une copie
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Agles State d'angle
 */
void aniAddAngleState(Animation* A, const AnimAngles* Angles);

/** @brief Compte les tiggers pour pouvoir faire le changement de states. Ceci est fait automatiquement lors du chargement d'une animation à partir d'un fichier mais sinon il faut le faire manuellement en appelant cette fonction
 *
 * cette fonction crée une copie
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Agles State d'angle
 */
void aniCountTriggers(Animation* A);

/** @brief Fait une mise à jour d'un Joueur à partir d'une animation.
 *
 * Cette fonction est celle qui doit être appelée à chauqe boucle pour mettre à jour l'animation du joueur
 * cette fonction crée une copie
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in,out] Player auquel s'applique l'animation
 */
void aniUpdate(Animation* A, SPlayer* P);

/** @brief Fait une mise à jour d'un Joueur à partir du currentstate de l'animation
 *
 * Cette fonction est celle qui doit être appelée après avoir créer les animations et le joueur pour mettre les states à un état stable et que les animations soient fluides
 * cette fonction crée une copie
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in,out] Player auquel s'applique l'animation
 */
void aniUpdateForCurrentState(Animation* A, SPlayer* P);

/** @brief Mutateur de Force
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Valeur du nouveau Force
 */
void aniSetForce(Animation* A, float Force);

/** @brief Accesseur de Force
 *
 * @param[in] A Animation à laquelle s'applique la fonction
 * @return Valeur de Force
 */
float aniGetForce(const Animation* A);

/** @brief Mutateur de Friction
 *
 * @param[in,out] A Animation à laquelle s'applique la fonction
 * @param[in] Valeur de la nouvelle Friction
 */
void aniSetFriction(Animation* A, float Friction);

/** @brief Accesseur de Friction
 *
 * @param[in] A Animation à laquelle s'applique la fonction
 * @return Valeur de Friction
 */
float aniGetFriction(const Animation* A);

//normalement cette fonction s'applique sur une animation qui a un seule état car si on modifie l'animation elle même c'ets comme si on animer le modèle...
void aniSetBodyPartAngle(Animation* A, AnimTriggers BodyPart, float Angle, unsigned int State = 0);

void aniSetBodyPartPosition(Animation* A, AnimTriggers BodyPart, Vec2 Pos, unsigned int State = 0);

///@}
 
#endif