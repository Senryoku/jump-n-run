#ifndef _SHAREDRESOURCES_H
#define _SHAREDRESOURCES_H

#include <Game/Message.h>
#include <Audio/SoundManager.h>

/**
 * @defgroup SharedResources
 * Structure qui réunit tous les managers et qui est là pour être instancé une seule fois. C'est cette structure qui sera passée de module en module pour partager des resources telles que des managers ou des images
 * préfixe: sh
 * @todo Mettre ici des pointeurs vers les fonction pour charger et liberér les images pour faire ça plus général
 * @{
 */

typedef struct s_SharedResources {
	SoundManager* SM; ///< SoundManager du jeu
	MessageManager* MM; ///< MessageManager du jeu
	std::map<std::string, unsigned int> Textures; ///< texture du jeu, accès par clés
	
	unsigned int (*LoadTexture)(const char*); ///< Fonction de chargement de texture
	void (*FreeTexture)(unsigned int); ///<Fonction pour libérer une Texture
	
	sf::Font FntMenu;
	
} SharedResources;


/** @brief Initialise un Sharedresources
 *
 * Ceci crée les instances nécessaires et les initialise
 * @param SR SharedResources auquel s'applique la fonction
 **/
void shInit(SharedResources* SR, unsigned int (*funcLoadTexture)(const char*), void (*funcFreeTexture)(unsigned int));

/** @brief Libère un Sharedresources
 *
 * On libère tout ce qui a été alloué
 * @param SR SharedResources auquel s'applique la fonction
 **/
void shFree(SharedResources* SR);

/** @brief Charge les textures en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
void shLoadTextures(SharedResources* SR);

/** @brief Charge les Fonts en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
void shLoadFonts(SharedResources* SR);

/** @brief Charge les sons et musques en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
void shLoadAudio(SharedResources* SR);

/** @brief Donne accès à une texture en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 * @param Key Clé pour accéder à la texture
 **/
unsigned int shGetTexture(SharedResources* SR, const char* Key);

/** @brief Donne accès au SoundManager
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
SoundManager* shSoundManager(SharedResources* SR);

/** @brief Donne accès au MessageManager
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
MessageManager* shMessageManager(SharedResources* SR);

/** @brief Donne accès à la police pour le menu
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
const sf::Font& shFntMenu(const SharedResources* SR); 


///@}


#endif