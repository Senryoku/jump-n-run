#ifndef _FLAG_H_
#define _FLAG_H_

#include <Rendering/OpenGl.h>

/** @defgroup Flag
 *
 * Un drapeau servant à afficher le but du niveau
 * Préfixe des méthodes : fl
 **/

typedef struct s_Flag
{
	DynArr Vertices; ///< Vertices composants le tissu du drapeau
	float cellH, cellW; ///< Nombre de cellules
	unsigned int H, W; ///< Dimension des cellules
	DynArr Rigids; ///< Rigids composants le tissu du drapeau
	Texture TexFlag; ///< Texture du drapeau
	Texture TexPole; ///< Texture du poteau
} Flag;

/** @brief Initialistion
 *
 **/
void flInit(Flag* F, float cellH, float cellW, unsigned int H, unsigned int W, Texture TexFlag, Texture TexPole);

/** @brief Libération
 *
**/
void flFree(Flag* F);

/** @brief Applique une force aux vertices du drapeau
 *
**/
void flApplyForce(Flag* F, Vec2 Force, Bool B);

/** @brief Applique la physique au drapeau
 *
**/
void flResolve(Flag* F, float prevdt, float dt);



/** @}
**/

#endif
