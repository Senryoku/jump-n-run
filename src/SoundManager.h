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
 * @todo Ajouter des const pour les modes
 * @{
 */

typedef struct {
	List Sounds;
	std::map<std::string, sf::SoundBuffer*> SoundBuffers;
	sf::Listener Listener;
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
void sndmLoadFile(SoundManager* SM, const std::string &Key, const std::string &File);

/**
 * @brief Joue un son à une position donnée
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 * @param Position position où se trouve le son
 */
void sndmPlay(SoundManager* SM, const std::string &Key, const Vec2 &Position);

/**
 * @brief Joue un son sans position (non relatif au Listener)
 * @param SM SoundManager où s'effectue la fonction
 * @param Key Clé pour accéder au son
 */
void sndmPlay(SoundManager* SM, const std::string &Key);

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
 * @brief Change la position du Listener
 * @param SM SoundManager où s'effectue la fonction
 * @param Position nouvelle position
 */
void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position, float MinDist=200.f, float Attenuation=1.f);

/**
 * @brief Obtient la position du Listener
 * @param SM SoundManager où s'effectue la fonction
 */
Vec2 sndmGetListenerPosition(const SoundManager* SM);


///@}

#endif