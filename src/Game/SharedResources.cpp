#include "SharedResources.h"


void shInit(SharedResources* SR, unsigned int (*funcLoadTexture)(const char*), void (*funcFreeTexture)(unsigned int))
{
	SR->MM = (MessageManager*)malloc(sizeof(MessageManager));
	SR->SM = (SoundManager*)malloc(sizeof(SoundManager));
	
	sndmInit(SR->SM);
	msgInit(SR->MM, SR);
	
	SR->LoadTexture = funcLoadTexture;
	SR->FreeTexture = funcFreeTexture;
}


void shFree(SharedResources* SR)
{
	sndmFree(SR->SM);
	msgFree(SR->MM);
	
	free(SR->MM);
	free(SR->SM);
	
	for (std::map<std::string, unsigned int>::iterator it = SR->Textures.begin(); it != SR->Textures.end(); it++)
		(*SR->FreeTexture)(it->second);
}


void shLoadTextures(SharedResources* SR)
{
	//Ici on met toutes les textures à charger elles seront libérées dans shFree()
	shAddTexture(SR, "mn_side", "data/gui/s_boxside.png");
	shAddTexture(SR, "mn_corner", "data/gui/s_boxcorner.png");
	shAddTexture(SR, "mn_shadow", "data/gui/s_box_shadow.png");
	shAddTexture(SR, "mn_gloss", "data/gui/s_box_gloss.png");
	shAddTexture(SR, "mn_anim", "data/gui/s_box_anim_strip20.png");
	shAddTexture(SR, "pl_leg", "data/s_leg.png");
	shAddTexture(SR, "gr_grass", "data/s_ground_grass.png");
	
	SR->txCursor.loadFromFile("data/gui/s_cursors.png");
	
	for (int i=0; i<4; i++)
	{
		SR->sprCursor[i].setTexture(SR->txCursor);
		SR->sprCursor[i].setTextureRect(sf::IntRect(i*16, 0, 16, 24));
		SR->sprCursor[i].setOrigin(3.f, 5.f);
	}
}

sf::Sprite& shGetCursorSprite(SharedResources* SR, unsigned char i)
{
	return SR->sprCursor[i];
}

void shAddTexture(SharedResources* SR, const char* Key, const char* Path)
{
	assert(SR->Textures.count(Key)==0);
	SR->Textures[Key] = (*SR->LoadTexture)(Path);
}

unsigned int shGetTexture(SharedResources* SR, const char* Key)
{
	std::map<std::string, unsigned int>::iterator it = SR->Textures.find(Key);
	assert(it != SR->Textures.end());
	
	return it->second;
}

SoundManager* shSoundManager(SharedResources* SR)
{
	return SR->SM;
}

MessageManager* shMessageManager(SharedResources* SR)
{
	return SR->MM;
}

void shLoadFonts(SharedResources* SR)
{
	SR->FntMenu.loadFromFile("data/gui/fnt_menu.ttf");
}

void shLoadAudio(SharedResources* SR)
{
	sndmLoadSoundFile(SR->SM, "snd_button", "data/sfx/snd_button.ogg");
	sndmLoadSoundFile(SR->SM, "snd_menu", "data/sfx/snd_menu.ogg");
	sndmLoadSoundFile(SR->SM, "snd_select", "data/sfx/snd_select.ogg");
	sndmLoadSoundFile(SR->SM, "snd_msg", "data/sfx/snd_msg.ogg");
	sndmLoadSoundFile(SR->SM, "snd_step1", "data/sfx/snd_step1.ogg");
	sndmLoadSoundFile(SR->SM, "snd_step2", "data/sfx/snd_step2.ogg");
	
	char file[255], key[20];
	
	for (int i=1; i<=11; i++)
	{
		sprintf(file, "data/music/music%u.ogg",i);
		sprintf(key, "music%u",i);
		sndmLoadMusicFile(SR->SM, key, file);
	}
	
}

const sf::Font& shFntMenu(const SharedResources* SR)
{
	return SR->FntMenu;
}

unsigned int shLoadTexture(const SharedResources* SR, const char* Path)
{
	return (*SR->LoadTexture)(Path);
}

void shFreeTexture(const SharedResources* SR, unsigned int T)
{
	(*SR->FreeTexture)(T);
}
