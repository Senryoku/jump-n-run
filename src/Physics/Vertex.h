#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <stdlib.h>
#include <stdio.h>

#include <Core/Tools.h>
#include <Core/Vec2.h>

/** @defgroup Vertex
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
	Bool Fixe; /**< Défini si le Vertex est Fixe, si vrai, seul un appel direct à vxSetPosition peut modifier sa position **/
} Vertex;

/** @brief Constructeur
**/
Vertex* newVertex();

/** @brief Destructeur
**/
void delVertex(Vertex* V);

/** @brief Initialise le Vertex V
 *
 * @param V Pointeur vers le vecteur considéré
**/
void vxInit(Vertex* V);

/** @brief Accesseur de la position
 *
 * @param V Pointeur vers le vecteur considéré
**/
Vec2 vxGetPosition(const Vertex* V);

/** @brief Accesseur de l'ancienne position
 *
 * @param V Pointeur vers le vecteur considéré
**/
Vec2 vxGetOldPos(const Vertex* V);

/** @brief Accesseur de l'accélération
 *
 * @param V Pointeur vers le vecteur considéré
**/
Vec2 vxGetAcceleration(const Vertex* V);

/** @brief Accesseur de la masse
 *
 * @param V Pointeur vers le vecteur considéré
**/
float vxGetMass(const Vertex* V);

/** @brief Accesseur de Fixe
 *
 * @param V Pointeur vers le vecteur considéré
 * @return true si le point est fixe
**/
Bool vxIsFixe(const Vertex* V);

/** @brief Mutateur de la position
 *
 * Modifie également l'ancienne position !
 * @param V Pointeur vers le vecteur à modifier
**/
void vxSetPosition(Vertex* V, Vec2 newPos);

/** @brief Mutateur de la position en x
 *
 * Modifie également l'ancienne position !
 * @param V Pointeur vers le vecteur à modifier
 **/
void vxSetX(Vertex* V, float x);

/** @brief Mutateur de la position en y
 *
 * Modifie également l'ancienne position !
 * @param V Pointeur vers le vecteur à modifier
 **/
void vxSetY(Vertex* V, float y);

/** @brief Mutateur de la position qui conserve la vitesse
 *
 * @param V Pointeur vers le vecteur à modifier
 **/
void vxChangePosition(Vertex* V, Vec2 newPos);

/** @brief Ajoute un vecteur à la position actuelle de V
 *
 * @param V Pointeur vers le vecteur à modifier
**/
void vxCorrectPosition(Vertex* V, Vec2 addPos);

/** @brief Ajoute un vecteur à la position actuelle de V même s'il est fixe
 *
 * @param V Pointeur vers le vecteur à modifier
 **/
void vxMove(Vertex* V, Vec2 addPos);

/** @brief Ajoute un vecteur à l'ancienne position de V
 *
 * @param V Pointeur vers le vecteur à modifier
**/
void vxCorrectSpeed(Vertex* V, Vec2 addSpeed);

/** @brief Mutateur de la masse
 *
 * Attend une valeur positive (une valeur négative ou nulle est ignorée)
 * @param V Pointeur vers le vecteur à modifier
 * @param newMass Nouvelle Masse (>0)
**/
void vxSetMass(Vertex* V, float newMass);

/** @brief Mutateur de Fixe
 *
 * @param V Pointeur vers le vecteur à modifier
 * @param B Nouvelle valeur de Fixe
**/
void vxSetFixe(Vertex* V, Bool B);

/** @brief Mutateur de l'accélération
 *
 * @param V Pointeur vers le vecteur à modifier
 * @param newAcc Nouvelle accélération
**/
void vxSetAcceleration(Vertex* V, Vec2 newAcc);

/** @brief Applique une force au Vertex
 * @param V Pointeur vers le vecteur à modifier
 * @param addForce Froce à ajouter
 * @param Mass Booléen, détermine si la masse du vertex doit être prose en compte
**/
void vxApplyForce(Vertex* V, Vec2 addForce, Bool Mass);

/** @brief Met à jour la position du Vertex en fonction de sa vitesse
 * et de son accélération.
 *
 * @param V Vertex à déplacer
 * @param prevdt Intervalle de temps considéré lors du dernier appel
 * @param dt Intervalle de temps à considérer
**/
void vxResolve(Vertex* V, float prevdt, float dt);

/** @brief Test de régression de Vertex
**/
void vxRegressionTest(void);

/** @}
**/

#endif
