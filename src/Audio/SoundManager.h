#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <Core/List.h>
#include <map>
#include <string>
#include <assert.h>
#include <Core/Vec2.h>
#include <new>
#include "SoundImpl.h"

/**
 * @defgroup SoundManager
 *
 * Sert à gèrer les sons en mémoires et les instances de ceux-ci
 * Préfixe: sndm
 * @todo Ajouter des const pour les modes. Certains on ne peux pas car il les fonctions sur les map nécessitent ne pas être const
 * @{
 */

typedef struct {
	List Sounds;
	std::map<std::string, SoundBuffer*> SoundBuffers;
	std::map<std::string, Music*> Musics;
	SoundListener Listener;

	//Pour le fading entre deux musiques
	float FadeSpeed;
	std::map<std::string, Music*>::iterator NextMusic;
	bool IsFading, Loop, Paused;
} SoundManager;

/**
 * @brief Alloue et initialise un SoundManager
 */

void sndmInit();

/**
 * @brief Déalloue un SoundManager
 */
void sndmFree();

/**
 * @brief Charge un son en mémoire et l'ajoute avec une clé

 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 * @return faux s'il y a eu une erreur
 */
bool sndmLoadSoundFile(const char *Key, const char *File);

/**
 * @brief Charge une musique en mémoire et l'ajoute avec une clé

 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 * @return faux s'il y a eu une erreur
 */
bool sndmLoadMusicFile(const char *Key, const char *File);

/**
 * @brief Joue un son à une position donnée

 * @param Key Clé pour accéder au son
 * @param Position position où se trouve le son
 */
void sndmPlay(const char *Key, const Vec2 &Position, float MinDist=200.f, float Attenuation=1.f);

/**
 * @brief Joue un son sans position (non relatif au Listener)

 * @param Key Clé pour accéder au son
 */
void sndmPlay(const char *Key);

/**
 * @brief Joue une musique

 * @param Key Clé pour accéder au son
 * @param Loop looper la chanson ou pas
 */
void sndmPlayMusic(const char *Key, bool Loop=1);

/**
 * @brief Change le volume d'une musique

 * @param Key Clé pour accéder au son
 * @param Volume nouvel volume [0..100]
 */
void sndmMusicSetVolume(const char *Key, float Volume);

/**
 * @brief Change le pitch d'une musique

 * @param Key Clé pour accéder au son
 * @param pitch nouvel pitch normal = 1.f
 */
void sndmMusicSetPitch(const char *Key, float Pitch);

/**
 * @brief Donne le volume d'une musique

 * @param Key Clé pour accéder au son
 * @return Volume [0..100]
 */
float sndmMusicGetVolume(const char *Key);

/**
 * @brief Donne le pitch d'une musique

 * @param Key Clé pour accéder au son
 * @return pitch normal = 1.f
 */
float sndmMusicGetPitch(const char *Key);

/**
 * @brief Fait un effet de Fade entre deux musiques

 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFade(const char *NextKey, float FadeSpeed, bool Loop=1);

/**
 * @brief Fait un effet de Fade et stoppe la musique à la fin

 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFadeToStop(float FadeSpeed);

/**
 * @brief Pause tous les sons

 */
void sndmPauseAllSounds();

/**
 * @brief Résume tous les sons

 */
void sndmResumeAllSounds();

/**
 * @brief Pause la musique

 */
void sndmPauseMusic();

/**
 * @brief Résume les musiques en pause

 */
void sndmResumeMusic();


/**
 * @brief Fait une mise à jours sur les sons (élimine de la mémoire les sons qui ne sont plus utilisés

 */
void sndmUpdate();

/**
 * @brief Stope tous les sons et les libère

 */
void sndmStopAll();

/**
 * @brief Stope toutes les musiques

 */
void sndmStopAllMusic();

/**
 * @brief Stope une musique

 * @param Key Clé de la musique
 */
void sndmStopMusic(const char *Key);

/**
 * @brief Change la position du Listener

 * @param Position nouvelle position
 */
void sndmSetListenerPosition(const Vec2& Position);

/**
 * @brief Obtient la position du Listener

 */
Vec2 sndmGetListenerPosition();


///@}

#endif
