#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <SFML/Network.hpp>
#include <Core/Tools.h>
#include <Core/DynArr.h>

/** @defgroup Score
 *
 * Structure permettant la gestion des scores.
 * Collecte et mise à jour depuis un serveur (communication avec un script php)
 * Préfixe des fonctions : sc
 * @{
**/

typedef struct
{
	char Player[255];
	char LvlName[255];
	char LvlMD5[255];
	unsigned int Time;
} Score;

/** @brief Constructeur
**/
Score* newScore(char Player[255], const char LvlName[255], const char LvlMD5[255], unsigned int Time);

/** @brief Destructeur
 *
 * @param S Score à détruire
**/
void delScore(Score* S);

/** @brief Initialisation
 *
 * @param S Score à initialiser
 * @param Player Pseudo du joueur
 * @param LvlName Nom du Niveau
 * @param lvlMD5 MD5 du niveau
 * @param Time Temps (en centièmes de secondes) réalisé par le joueur
**/
void scInit(Score* S, const char Player[255], const char LvlName[255], const char LvlMD5[255], unsigned int Time);

/** @brief Envoi un score au serveur de score pour être enregistré
 *
 * @param S Score à enregistrer
 * @return 0 si tout s'est bien passé
**/
Bool scSend(Score *S);

/** @brief Retourne un DynArr* rempli avec les scores du niveau demandé
 *
 * Les codes d'erreurs ne sont pas géré, le passage en paramètre d'un tableau à remplir
 * générant des erreurs incompréhensibles (plantage sous windows sur un malloc(sizeof(void*)))
 * @param LvlName Nom du niveau
 * @param LvlMD5 MD5 du niveau
 * @return DynArr* contenant les score récupérés
**/
//unsigned int scCollect(DynArr* DA, const char LvlName[255], const char LvlMD5[255]);
DynArr* scCollect(const char LvlName[255], const char LvlMD5[255]);

/** @brief Libère un tableau de score rempli par scCollect
 *
 * Libère également les Score* portés par le DynArr
 * @param DA DynArr à libérer
**/
void scCollectFree(DynArr* DA);

/** @brief Libération des ressources utilisées par S
 *
 * @param S Score à libérer
**/
void scFree(Score *S);

/** @brief Test de Régression
**/
void scRegressionTest();

/** @}
**/

#endif // _SCORE_H_
