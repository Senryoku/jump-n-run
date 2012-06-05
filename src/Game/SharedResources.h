#ifndef _SHAREDRESOURCES_H
#define _SHAREDRESOURCES_H

#include <Game/Message.h>
#include <Audio/SoundManager.h>
#include <SFML/Graphics.hpp>

/**
 * @defgroup SharedResources SharedRessources
 * Structure qui réunit tous les managers et qui est là pour être instancé une seule fois. C'est cette structure qui sera passée de module en module pour partager des resources telles que des managers ou des images
 * préfixe: sh
 * SFML crée (des fois) des leaks (32 otets ou plus) pour listener et texture, impossible corriger cela avec des allocations dynamiques ou autres.
 * @todo Mettre ici des pointeurs vers les fonction pour charger et liberér les images pour faire ça plus général
 * @{
 */

#define SPR_CURSOR 0x00
#define SPR_CURSOR_HAND 0x01
#define SPR_CURSOR_NORMAL 0x02
#define SPR_CURSOR_DRAG 0x03


typedef struct s_SharedResources {
	SoundManager* SM; ///< SoundManager du jeu
	MessageManager* MM; ///< MessageManager du jeu
	std::map<std::string, unsigned int> Textures; ///< textures du jeu, accès par clés

	unsigned int (*LoadTexture)(const char*); ///< Fonction de chargement de texture
	void (*FreeTexture)(unsigned int); ///<Fonction pour libérer une Texture

	sf::Font FntMenu;
	sf::Texture txCursor;
	sf::Sprite sprCursor[4];
	sf::Image imgIcon;

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
 * @return MessageManager
 **/
MessageManager* shMessageManager(SharedResources* SR);

/** @brief Change l'iconne d'une fenêtre
 *
 * @param SR SharedResources auquel s'applique la fonction
 * @param win Fenêtre à laquelle s'applique la fonction 
 **/
void shSetWindowIcon(SharedResources* SR, sf::RenderWindow& win);

/** @brief Charge une texture en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 * @param Key Clé pour accéder à la texture
 * @param Path chemin où se trouve l'image
 **/
void shAddTexture(SharedResources* SR, const char* Key, const char* Path);

/** @brief Charge une texture en mémoire
 *
 * @param SR SharedResources auquel s'applique la fonction
 * @param i curseur @see SPR_CURSOR
 * @return référence vers le sf::Sprite
 **/
sf::Sprite& shGetCursorSprite(SharedResources* SR, unsigned char i);

/** @brief Donne accès à la police pour le menu
 *
 * @param SR SharedResources auquel s'applique la fonction
 **/
const sf::Font& shFntMenu(const SharedResources* SR);

/** @brief Charge une texture
 *
 * Puisque cette fonction est abstraite ça permet d'avoir le même code avec des libraries différentes
 * @param SR SharedResources auquel s'applique la fonction
 * @param Path Chemin de la texture à charger
 **/

unsigned int shLoadTexture(const SharedResources* SR, const char* Path);

/** @brief Libère une texture
 *
 * Puisque cette fonction est abstraite ça permet d'avoir le même code avec des libraries différentes
 * @param SR SharedResources auquel s'applique la fonction
 * @param T texture à libérer
 **/
void shFreeTexture(const SharedResources* SR, unsigned int T);



///@}


#endif
