#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include "List.h"
#include <map>
#include <string>
#include <assert.h>
#include "Vec2.h"
#include <new>
#include <SFML/Audio.hpp>

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
	std::map<std::string, sf::SoundBuffer*> SoundBuffers;
	std::map<std::string, sf::Music*> Musics;
	sf::Listener Listener;
	
	//Pour le fading entre deux musiques
	float FadeSpeed;
	std::map<std::string, sf::Music*>::iterator NextMusic;
	bool IsFading, Loop;
} SoundManager;

/**
 * @brief Alloue et initialise un SoundManager
 * @param Adresse du SoundManager à allouer
 */

void sndmInit(SoundManager* SM);

/**
 * @brief Déalloue un SoundManager
 * @return pointeur vers le SoundManager alloué et initialisé
 */
void sndmFree(SoundManager* SM);

/**
 * @brief Charge un son en mémoire et l'ajoute avec une clé
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 */
void sndmLoadSoundFile(SoundManager* SM, const std::string &Key, const std::string &File);

/**
 * @brief Charge une musique en mémoire et l'ajoute avec une clé
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param File Adrsse du fichier
 */
void sndmLoadMusicFile(SoundManager* SM, const std::string &Key, const std::string &File);

/**
 * @brief Joue un son à une position donnée
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param Position position où se trouve le son
 */
void sndmPlay(SoundManager* SM, const std::string &Key, const Vec2 &Position, float MinDist=200.f, float Attenuation=1.f);

/**
 * @brief Joue un son sans position (non relatif au Listener)
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 */
void sndmPlay(SoundManager* SM, const std::string &Key);

/**
 * @brief Joue une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param Loop looper la chanson ou pas
 */
void sndmPlayMusic(SoundManager* SM, const std::string &Key, bool Loop=1);

/**
 * @brief Change le volume d'une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param Volume nouvel volume [0..100]
 */
void sndmMusicSetVolume(SoundManager* SM, const std::string &Key, float Volume);

/**
 * @brief Change le pitch d'une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param pitch nouvel pitch normal = 1.f
 */
void sndmMusicSetPitch(SoundManager* SM, const std::string &Key, float Pitch);

/**
 * @brief Donne le volume d'une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @return Volume [0..100]
 */
float sndmMusicGetVolume(SoundManager* SM, const std::string &Key);

/**
 * @brief Donne le pitch d'une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @return pitch normal = 1.f
 */
float sndmMusicGetPitch(SoundManager* SM, const std::string &Key);

/**
 * @brief Fait un effet de Fade entre deux musiques
 * @param SM SoundManager où s'effectue la fonction
 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFade(SoundManager* SM, const std::string &NextKey, float FadeSpeed, bool Loop=1);

/**
 * @brief Fait un effet de Fade et stoppe la musique à la fin
 * @param SM SoundManager où s'effectue la fonction
 * @param NextKey Clé pour accéder au son
 * @param FadeSpeed vitesse du fading 100 = tout le volume d'un coup
 */
void sndmMusicFadeToStop(SoundManager* SM, float FadeSpeed);

/**
 * @brief Pause tous les sons
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmPauseAllSounds(SoundManager* SM);

/**
 * @brief Résume tous les sons
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmResumeAllSounds(SoundManager* SM);

/**
 * @brief Pause la musique
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmPauseMusic(SoundManager* SM);

/**
 * @brief Résume les musiques en pause
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmResumeMusic(SoundManager* SM);


/**
 * @brief Fait une mise à jours sur les sons (élimine de la mémoire les sons qui ne sont plus utilisés
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmUpdate(SoundManager* SM);

/**
 * @brief Stope tous les sons et les libère
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmStopAll(SoundManager* SM);

/**
 * @brief Stope toutes les musiques
 * @param SM SoundManager où s'effectue la fonction
 */
void sndmStopAllMusic(SoundManager* SM);

/**
 * @brief Stope une musique
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé de la musique
 */
void sndmStopMusic(SoundManager* SM, const std::string &Key);

/**
 * @brief Change la position du Listener
 * @param SM SoundManager où s'effectue la fonction
 * @param Position nouvelle position
 */
void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position);

/**
 * @brief Obtient la position du Listener
 * @param SM SoundManager où s'effectue la fonction
 */
Vec2 sndmGetListenerPosition(const SoundManager* SM);


///@}

#endif