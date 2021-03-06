#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <Core/List.h>
#include <map>
#include <string>
#include <assert.h>
#include <Core/Vec2.h>
#include <new>
#include <SFML/Audio.hpp>
//Utiliser des typedef fait des leaks, donc on est obligés d'utiliser directement SFML
//Il y a un leak de 16b de la part de Listener

/**
 * @defgroup SoundManager SoundManager
 *
 * Sert à gèrer les sons en mémoires et les instances de ceux-ci
 * Préfixe: sndm
 * @{
 */

typedef struct {
	List Sounds;
	std::map<std::string, sf::SoundBuffer*>* SoundBuffers;
	std::map<std::string, sf::Music*>* Musics;
	sf::Listener* Listener;

	//Pour le fading entre deux musiques
	float FadeSpeed;
	int PlayCount; /**<PlayCount de la musique, permet de changer la musique au bout d'un certain nombre de Replays **/
	int MaxPlayCount; /**<Si MaxPlayCount est < à 0 Le playcount n'est pas tenu en compte pour passer les chansons automatiquement **/
	float DefaultFadingSpeed;
	float LastTimeOffset;
	float CurrentTimeOffset;
	std::map<std::string, sf::Music*>::iterator NextMusic;
	std::map<std::string, sf::Music*>::iterator CurrentMusic;
	bool IsFading, Loop, Paused;
} SoundManager;

/**
 * @brief Alloue et initialise un SoundManager
 * @param SM SoundManager auquel s'applique la fonction
 */

void sndmInit(SoundManager* SM);

/**
 * @brief Déalloue un SoundManager
 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmFree(SoundManager* SM);

/**
 * @brief Charge un son en mémoire et l'ajoute avec une clé
 *
 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 * @return faux s'il y a eu une erreur
 */
bool sndmLoadSoundFile(SoundManager* SM, const char *Key, const char *File);

/**
 * @brief Charge une musique en mémoire et l'ajoute avec une clé

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 * @return faux s'il y a eu une erreur
 */
bool sndmLoadMusicFile(SoundManager* SM, const char *Key, const char *File);

/**
 * @brief Obtient le PlayCount de la musique en cours

 * @param SM SoundManager auquel s'applique la fonction
 * @return PlayCount
 */
int sndmGetPlayCount(const SoundManager* SM);

/**
 * @brief Donne le MaxPlayCount pour changer de musique
 *
 * Un MaxPlayCount négatif veut dire Loop infini
 * @param SM SoundManager auquel s'applique la fonction
 * @param MaxPlayCount
 */
void sndmSetMaxPlayCount(SoundManager* SM, int MaxPlayCount);

/**
 * @brief Dit si on est en train de faire un Fade sur une chanson

 * @param SM SoundManager auquel s'applique la fonction
 * @return Vrai si c'ets le cas
 */
Bool sndmIsInFading(const SoundManager* SM);

/**
 * @brief Joue un son à une position donnée

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param Position position où se trouve le son
 * @param MinDist Distance à laquelle le son peu être écouté sans attenuation
 * @param Attenuation Attenuation du son
 */
void sndmPlay(SoundManager* SM, const char *Key, const Vec2 &Position, float MinDist=200.f, float Attenuation=1.f);

/**
 * @brief Joue un son sans position (non relatif au Listener)

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 */
void sndmPlay(SoundManager* SM, const char *Key);

/**
 * @brief Joue une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param Loop looper la chanson ou pas
 */
void sndmPlayMusic(SoundManager* SM, const char *Key, bool Loop=1);

/**
 * @brief Joue la première musique
 
 * @param SM SoundManager auquel s'applique la fonction
 * @param Loop looper la chanson ou pas
 */
void sndmPlayMusic(SoundManager* SM, bool Loop=1);

/**
 * @brief Change le volume d'une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param Volume nouvel volume [0..100]
 */
void sndmMusicSetVolume(SoundManager* SM, const char *Key, float Volume);

/**
 * @brief Change le pitch d'une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @param pitch nouvel pitch normal = 1.f
 */
void sndmMusicSetPitch(SoundManager* SM, const char *Key, float Pitch);

/**
 * @brief Donne le volume d'une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @return Volume [0..100]
 */
float sndmMusicGetVolume(SoundManager* SM, const char *Key);

/**
 * @brief Donne le pitch d'une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé pour accéder au son
 * @return pitch normal = 1.f
 */
float sndmMusicGetPitch(SoundManager* SM, const char *Key);

/**
 * @brief Fait un effet de Fade entre deux musiques

 * @param SM SoundManager auquel s'applique la fonction
 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFade(SoundManager* SM, const char *NextKey, float FadeSpeed, bool Loop=1);

/**
 * @brief Fait un effet de Fade entre deux musiques
 
 * @param SM SoundManager auquel s'applique la fonction
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFadeToNext(SoundManager* SM, float FadeSpeed, bool Loop=1);

/**
 * @brief Fait un effet de Fade et stoppe la musique à la fin

 * @param SM SoundManager auquel s'applique la fonction
 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFadeToStop(SoundManager* SM, float FadeSpeed);

/**
 * @brief Pause tous les sons

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmPauseAllSounds(SoundManager* SM);

/**
 * @brief Résume tous les sons

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmResumeAllSounds(SoundManager* SM);

/**
 * @brief Pause la musique
 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmPauseMusic(SoundManager* SM);

/**
 * @brief Résume les musiques en pause

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmResumeMusic(SoundManager* SM);


/**
 * @brief Fait une mise à jours sur les sons (élimine de la mémoire les sons qui ne sont plus utilisés

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmUpdate(SoundManager* SM);

/**
 * @brief Stope tous les sons et les libère

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmStopAll(SoundManager* SM);

/**
 * @brief Stope toutes les musiques

 * @param SM SoundManager auquel s'applique la fonction
 */
void sndmStopAllMusic(SoundManager* SM);

/**
 * @brief Stope une musique

 * @param SM SoundManager auquel s'applique la fonction
 * @param Key Clé de la musique
 */
void sndmStopMusic(SoundManager* SM, const char *Key);

/**
 * @brief Change la position du Listener

 * @param SM SoundManager auquel s'applique la fonction
 * @param Position nouvelle position
 */
void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position);

/**
 * @brief Obtient la position du Listener

 * @param SM SoundManager auquel s'applique la fonction
 */
Vec2 sndmGetListenerPosition(const SoundManager* SM);


/**
 * @brief Test de regression

 */
void sndmRegressionTest();


///@}

#endif
