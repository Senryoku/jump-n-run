#ifndef _TOOLS_H_
#define _TOOLS_H_

typedef char Bool;

/*
typedef enum e_bool
{
	FALSE = 0x00,
	TRUE = 0x01
} bool;
*/

#define MIN( X, Y ) ( (X) < (Y) ? (X) : (Y) ) /* Deux macros qui déterminent le min et le max */
#define MAX( X, Y ) ( (X) > (Y) ? (X) : (Y) )
#define SGN( X ) ( (X) < 0 ? (-1) : (1) ) /* Signe de X */

/**
 * @brief Fait une transition enre deux variables
 * @param CurrentPosition pointeur sur la valeur qui va être modifié
 * @param TargetPosition position vers laquelle on va faire la transition
 * @param Force force de la transition
 * @param Friction friction (ralentissement) de la transition)
 * @param Spd pointeur vers une variable qui permet de faire la transition (cette variable ne doit pas être modifié outre que dans cette fonction, il faut donc qu'elle soit crée quelquepart car sa valeur est modifié et est essentielle
 */
void Wobble(float *CurrentPosition, float TargetPosition, float Force,float Friction, float *Spd);

#endif
