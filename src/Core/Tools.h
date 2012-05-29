#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <assert.h>

typedef char Bool;


typedef enum enum_bool
{
	FALSE = 0x00,
	TRUE = 0x01
} e_bool;

typedef struct
{
	float WindowWidth;
	float WindowHeight;
	float FPSLimit;
	float AntiAliasing;
	float VerticalSync;
} Config;

#define MIN( X, Y ) ( (X) < (Y) ? (X) : (Y) ) /* Deux macros qui déterminent le min et le max */
#define MAX( X, Y ) ( (X) > (Y) ? (X) : (Y) )
#define SGN( X ) ( (X) < 0 ? (-1) : (1) ) /* Signe de X */
#define ABS( X ) ( (X) < 0 ? (-(X)) : (X) ) /* Valeur absolue de X */
#define RAD2DEG( X ) ( (X) * (180.0/M_PI) ) /* Conversion de radians en degrès */
#define DEG2RAD( X ) ( (X) * (M_PI/180.0) ) /* Conversion de degrès en radians */
#define ISNAN( X ) ( (X) != (X) ) /* Permet de savoir si un float est un NaN */

/**
 * @brief Fait une transition enre deux variables
 * @param CurrentPosition pointeur sur la valeur qui va être modifié
 * @param TargetPosition position vers laquelle on va faire la transition
 * @param Force force de la transition
 * @param Friction friction (ralentissement) de la transition)
 * @param Spd pointeur vers une variable qui permet de faire la transition (cette variable ne doit pas être modifié outre que dans cette fonction, il faut donc qu'elle soit crée quelquepart car sa valeur est modifié et est essentielle
 */
void Wobble(float *CurrentPosition, float TargetPosition, float Force,float Friction, float *Spd);

Bool DirectoryExists(const char* Dir);
Bool CreateDirectory(const char* Dir);

Config GetConfiguration();

#endif
