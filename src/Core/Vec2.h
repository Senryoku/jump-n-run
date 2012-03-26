#ifndef _VEC2_H_
#define _VEC2_H_

#include <math.h>
#include <stdio.h>

#include <Core/Tools.h>

/** @defgroup Vec2
 *
 * Opérations sur des vecteurs de R²
 * Préfixe des fonctions : vec2
 * @{
**/

/** @brief Vecteur de R²
**/
typedef struct
{
    float x; /**< Coordonnée x **/
    float y; /**< Coordonnée y **/
} Vec2;

/** @brief "Constructeur", renvoi le vecteur de coordonées passées en paramètre
 *
 * @param x Coordonnée x
 * @param y Coordonnée y
 * @return
**/
Vec2 vec2(float x, float y);

/** @brief Addition de vecteur
**/
Vec2 vec2Add(Vec2 V1, Vec2 V2);
/** @brief Soustration de vecteur
**/
Vec2 vec2Sub(Vec2 V1, Vec2 V2);

/** @brief Multiplication par un scalaire
**/
Vec2 vec2Prod(Vec2 V, float f);
/** @brief Division par un scalaire
**/
Vec2 vec2Div(Vec2 V, float f);

/** @brief Produit scalaire
**/
float vec2Dot(Vec2 V1, Vec2 V2);

/** @brief Calcul de la Norme
**/
float vec2Length(Vec2 V);

/** @brief Norme au carré
**/
float vec2SqLength(Vec2 V);

/** @brief Retourne un vecteur normal (orthogonal)
 *
 * Rotation de PI/2 (direct)
**/
Vec2 vec2Ortho(Vec2 V);

/** @brief Retourne le vecteur normalisé (Norme = 1)
**/
Vec2 vec2Normalized(Vec2 V);

/** @brief Test d'égalité des Vec2
**/
Bool vec2Equal(Vec2 V1, Vec2 V2);

/** @brief Copie V2 dans V1
 *
 * V1 pointeur, V2 non !
 * @param V1 Pointeur vers le vecteur à modifier
 * @param V2 Source
**/
void vec2Cp(Vec2* V1, Vec2 V2);

/** @brief Fait une rotation sur le point V
 *
 * @param V vecteur à modifier
 * @param R angle en radians
 **/
Vec2 vec2Rotate(Vec2 V, Vec2 Origin, float R);

/** @brief Test de régression de Vec2
**/
void vec2RegressionTest(void);

/** @}
**/

#endif
