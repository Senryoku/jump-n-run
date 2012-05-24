#ifndef _FPSCOUNTER_H
#define _FPSCOUNTER_H

#include <SFML/System/Clock.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Core/Tools.h>

/**
 * @defgroup FPSCounter FPSCounter
 * Structure qui gère les fps pour un affichage en debug et test de vitesse
 * préfixe: fps
 * @{
 */

typedef struct {
	sf::Clock Clock; /**<Horloge **/
	unsigned int Frames; /**<Nombre de steps faits depuis la dernière mise à 0 **/
	float LastFPS; /**<Dernière valeur des FPS **/
	std::string FPSString; /**<Valeur des FPS en std::string pour l'afficher à l'écran **/
	Bool Changed; /**<Détermine si la valeur des FPS vient d'être changée **/
} FPSCounter;

/** @brief Initialise une instance de FPSCounter
 *
 * @param F FPSCounter auquel s'applique la fonction
 **/
void fpsInit(FPSCounter* F);

/** @brief Reset un FPSCounter
 *
 * @param F FPSCounter auquel s'applique la fonction
 **/
void fpsReset(FPSCounter* F);

/** @brief Compte un Step pour le FPSCounter
 *
 * Cette fonction doit être appelée avant chanque Display
 * @code
 fpsStep(fps);
 if (fpsChanged(fps))
	printf("FPS: %s\n", fpsGetChar(fps));
 @endcode
 * @param F FPSCounter auquel s'applique la fonction
 **/
void fpsStep(FPSCounter* F);

/** @brief Donne accès au texte contenant les FPS
 *
 * @param F FPSCounter auquel s'applique la fonction
 * @return réference constante au texte des FPS
 **/
const std::string& fpsGetString(const FPSCounter* F);

/** @brief Donne accès au texte contenant les FPS
 *
 * @param F FPSCounter auquel s'applique la fonction
 * @return pointeur constant au texte des FPS
 **/
const char* fpsGetChar(const FPSCounter* F);

/** @brief Donne accès aux FPS
 *
 * @param F FPSCounter auquel s'applique la fonction
  * @return FPS en valeur numérique
 **/
float fpsGetNumber(const FPSCounter* F);

/** @brief Dit si la valeur de LastFPS vient de changer
 *
 * @param F FPSCounter auquel s'applique la fonction
 * @return Vrai si on on vient de changer la valeur des FPS
 **/
Bool fpsChanged(const FPSCounter* F);

/** @}
 **/

#endif
