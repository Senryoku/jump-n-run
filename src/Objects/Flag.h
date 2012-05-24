#ifndef _FLAG_H_
#define _FLAG_H_

#include <Rendering/OpenGl.h>

/** @defgroup Flag Flag
 *
 * Un drapeau servant à afficher le but du niveau
 * Préfixe des méthodes : fl
 * @{
**/

typedef struct s_Flag
{
	DynArr Vertices; ///< Vertices composants le tissu du drapeau
	float cellH, cellW; ///< Dimension des cellules
	unsigned int H, W; ///< Nombre de cellules
	DynArr Rigids; ///< Rigids composants le tissu du drapeau
	Texture TexFlag; ///< Texture du drapeau
	Texture TexPole; ///< Texture du poteau
} Flag;

/** @brief Initialisation
 *
 * @param F Flag
 * @param cellH Hauteur des cellules
 * @param cellW Largeur des Cellules
 * @param H Nombre de cellules en hauteur
 * @param W Nombre de cellules en largeur
 * @param TexFlag Texture du drapeau
 * @param TexPole Texture du poteau
 **/
void flInit(Flag* F, float cellH, float cellW, unsigned int H, unsigned int W, Texture TexFlag, Texture TexPole);

/** @brief Libération
 *
 * @param F Flag
**/
void flFree(Flag* F);

/** @brief Applique une force aux vertices du drapeau
 *
 * @param F Flag
 * @param Force Force à appliquer
 * @param B Bool, détermine la prise en compte de la masse
**/
void flApplyForce(Flag* F, Vec2 Force, Bool B);

/** @brief Applique la physique au drapeau
 *
 * @param F Flag
 * @param prevdt Intervalle de temps précédent
 * @param dt Intervalle de temps courant
**/
void flResolve(Flag* F, float prevdt, float dt);

/** @}
**/

#endif
