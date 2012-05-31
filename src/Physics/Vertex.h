#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <stdlib.h>
#include <stdio.h>

#include <Core/Tools.h>
#include <Core/Vec2.h>

/** @defgroup Vertex Vertex
 *
 * Vertex et opérations associées
 * Préfixe des fonctions : vx
 * @{
**/

typedef struct
{
	Vec2 Position; /**< Position actuelle **/
	Vec2 OldPos; /**< Position précédente (Frame) **/
	Vec2 Acceleration; /**< Valeur de l'accélération pour la frame courante **/
	float Mass; /**< Mass (Défaut = 1) **/
	Bool Fixed; /**< Défini si le Vertex est Fixed, si vrai, seul un appel direct à vxSetPosition peut modifier sa position **/
} Vertex;

/** @brief Constructeur
 *
 * @return Pointeur vers une nouvelle instance de Vertex
**/
Vertex* newVertex();

/** @brief Construteur par Copie
 *
 * @param[in] V Vertex à copier
 * @return Pointeur vers une copie de V
**/
Vertex* cpyVertex(const Vertex* V);

/** @brief Destructeur
 *
 * @param[in,out] V Vertex à détruire
**/
void delVertex(Vertex* V);

/** @brief Initialise le Vertex V
 *
 * @param[in,out] V Pointeur vers le vecteur considéré
**/
void vxInit(Vertex* V);

/** @brief Accesseur de la position
 *
 * @param[in,out] V Pointeur vers le vecteur considéré
 * @return Position de V
**/
Vec2 vxGetPosition(const Vertex* V);

/** @brief Accesseur de l'ancienne position
 *
 * @param[in] V Pointeur vers le vecteur considéré
 * @return Ancienne position de V
**/
Vec2 vxGetOldPos(const Vertex* V);

/** @brief Accesseur de l'accélération
 *
 * @param[in] V Pointeur vers le vecteur considéré
 * @return Accélération de V
**/
Vec2 vxGetAcceleration(const Vertex* V);

/** @brief Accesseur de la masse
 *
 * @param[in] V Pointeur vers le vecteur considéré
 * @return Masse de V
**/
float vxGetMass(const Vertex* V);

/** @brief Accesseur de Fixed
 *
 * @param[in] V Pointeur vers le vecteur considéré
 * @return true si le point est fixe
**/
Bool vxIsFixed(const Vertex* V);

/** @brief Mutateur de la position
 *
 * Modifie également l'ancienne position !
 * @param[in,out] V Pointeur vers le vertex à modifier
 * @param[in] newPos Nouvelle position
**/
void vxSetPosition(Vertex* V, Vec2 newPos);

/** @brief Mutateur de la position en x
 *
 * Modifie également l'ancienne position !
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] x Nouvelle coordonnée x
 **/
void vxSetX(Vertex* V, float x);

/** @brief Mutateur de la position en y
 *
 * Modifie également l'ancienne position !
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] y Nouvelle coordonnée y
 **/
void vxSetY(Vertex* V, float y);

/** @brief Translate le Vertex (<=> Téléportation)
 *
 * Conserve la vitesse
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] addPos Vecteur de Translation
 **/
void vxTranslate(Vertex* V, Vec2 addPos);

/** @brief Mutateur de la position qui conserve la vitesse
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] newPos Nouvelle position
 **/
void vxChangePosition(Vertex* V, Vec2 newPos);

/** @brief Ajoute un vecteur à la position actuelle de V
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] addPos Position à ajouter
**/
void vxCorrectPosition(Vertex* V, Vec2 addPos);

/** @brief Ajoute un vecteur à la position actuelle de V même s'il est fixe
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] addPos Position à ajouter
 **/
void vxMove(Vertex* V, Vec2 addPos);

/** @brief Ajoute un vecteur à l'ancienne position de V
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] addSpeed Vitesse à ajouter
**/
void vxCorrectSpeed(Vertex* V, Vec2 addSpeed);

/** @brief Mutateur de la masse
 *
 * Attend une valeur positive (une valeur négative ou nulle est ignorée)
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] newMass Nouvelle Masse (>0)
**/
void vxSetMass(Vertex* V, float newMass);

/** @brief Mutateur de Fixed
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] B Nouvelle valeur de Fixed
**/
void vxSetFixed(Vertex* V, Bool B);

/** @brief Mutateur de l'accélération
 *
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] newAcc Nouvelle accélération
**/
void vxSetAcceleration(Vertex* V, Vec2 newAcc);

/** @brief Applique une force au Vertex
 * @param[in,out] V Pointeur vers le vecteur à modifier
 * @param[in] addForce Froce à ajouter
 * @param[in] Mass Booléen, détermine si la masse du vertex doit être prose en compte
**/
void vxApplyForce(Vertex* V, Vec2 addForce, Bool Mass);

/** @brief Met à jour la position du Vertex en fonction de sa vitesse
 * et de son accélération.
 *
 * @param[in,out] V Vertex à déplacer
 * @param[in] prevdt Intervalle de temps considéré lors du dernier appel
 * @param[in] dt Intervalle de temps à considérer
**/
void vxResolve(Vertex* V, float prevdt, float dt);

/** @brief Test de régression de Vertex
**/
void vxRegressionTest(void);

/** @}
**/

#endif
