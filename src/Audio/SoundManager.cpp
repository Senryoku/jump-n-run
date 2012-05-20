#include "SoundManager.h"

#include <stdio.h>


void sndmInit(SoundManager* SM)
{
	lstInit(&SM->Sounds);
	SM->Listener.setPosition(0.f, 0.f, 5.f);
	SM->FadeSpeed=0.f;
	SM->IsFading=0;
	SM->Loop=1;
	SM->Paused=0;
	SM->PlayCount = 0;
	SM->MaxPlayCount = 1;
	SM->LastTimeOffset = 0.f;
	SM->CurrentTimeOffset = 0.f;
	SM->DefaultFadingSpeed = 5.f;
	SM->CurrentMusic = SM->Musics.end();
	//SM->NextMusic=;
	//Cela évite un crash bizarre...
	SM->SoundBuffers.clear();
	SM->Musics.clear();
}

void sndmFree(SoundManager* SM)
{
	sndmStopAll(SM);
	lstFree(&(SM->Sounds));
	for (std::map<std::string, sf::SoundBuffer*>::iterator it=SM->SoundBuffers.begin(); it!=SM->SoundBuffers.end(); it++)
		delete it->second;
	SM->SoundBuffers.clear();

	for (std::map<std::string, sf::Music*>::iterator it=SM->Musics.begin(); it!=SM->Musics.end(); it++)
		delete it->second;
	SM->Musics.clear();
}

bool sndmLoadSoundFile(SoundManager* SM, const char *Key, const char *File)
{
	//On vérifie que la clé n'existe pas déjà
	assert(SM->SoundBuffers.count(Key)==0);

	SM->SoundBuffers[Key] = new sf::SoundBuffer;
	if (!SM->SoundBuffers[Key]->loadFromFile(File))
	{
		//Le chargement a raté
		delete SM->SoundBuffers[Key];
		std::map<std::string, sf::SoundBuffer*>::iterator it;
		it=SM->SoundBuffers.find(Key);
		SM->SoundBuffers.erase(it);
		return 0;
	}
	return 1;
}

bool sndmLoadMusicFile(SoundManager* SM, const char *Key, const char *File)
{
	//On vérifie que la clé n'existe pas déjà
	assert(SM->Musics.count(Key)==0);

	SM->Musics[Key] = new sf::Music;
	if (!SM->Musics[Key]->openFromFile(File))
	{
		//Le chargement a raté
		delete SM->Musics[Key];
		std::map<std::string, sf::Music*>::iterator it;
		it=SM->Musics.find(Key);
		SM->Musics.erase(it);
		printf("Error Loading Music File %s\n", File);
		return 0;
	}

	return 1;
}

void sndmPlay(SoundManager* SM, const char *Key, const Vec2 &Position, float MinDist, float Attenuation)
{
	std::map<std::string, sf::SoundBuffer*>::iterator it = SM->SoundBuffers.find(Key);
	assert(it != SM->SoundBuffers.end());

	sf::Sound *snd=new sf::Sound;

	lstAdd(&(SM->Sounds), snd);
	snd->setBuffer(*(it->second));
	snd->setPosition(Position.x, Position.y, 0.f);
	snd->setMinDistance(MinDist);
	snd->setAttenuation(Attenuation);
	snd->play();
}


void sndmPlay(SoundManager* SM, const char *Key)
{
	std::map<std::string, sf::SoundBuffer*>::iterator it = SM->SoundBuffers.find(Key);
	assert(it != SM->SoundBuffers.end());

	sf::Sound *snd=new sf::Sound;

	lstAdd(&(SM->Sounds), snd);
	snd->setBuffer(*(it->second));
	snd->setRelativeToListener(1); //Le son est joué sans tenir compte de la position
	snd->play();

}

void sndmPlayMusic(SoundManager* SM, const char *Key, bool Loop)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	it->second->setLoop(Loop);
	it->second->play();
	SM->CurrentMusic = it;
	SM->PlayCount = 0;
}

unsigned int sndmGetPlayCount(const SoundManager* SM)
{
	return SM->PlayCount;
}

Bool sndmIsInFading(const SoundManager* SM)
{
	return SM->FadeSpeed>0.f;
}

void sndmMusicSetVolume(SoundManager* SM, const char *Key, float Volume)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	it->second->setVolume(Volume);
}

void sndmMusicSetPitch(SoundManager* SM, const char *Key, float Pitch)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	it->second->setPitch(Pitch);
}

float sndmMusicGetVolume(SoundManager* SM, const char *Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	return it->second->getVolume();
}

float sndmMusicGetPitch(SoundManager* SM, const char *Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	return it->second->getPitch();
}

void sndmMusicFade(SoundManager* SM, const char *NextKey, float FadeSpeed, bool Loop)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(NextKey);
	assert(it != SM->Musics.end());

	if (SM->IsFading) return; //On est en milieu d'un fade on ne va pas en faire un

	SM->FadeSpeed=FadeSpeed;
	//SM->IsFading=0;
	SM->NextMusic=it;
	SM->Loop=Loop;
}

void sndmMusicFadeToStop(SoundManager* SM, float FadeSpeed)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.end();
	SM->FadeSpeed=FadeSpeed;
	SM->IsFading=0;
	SM->NextMusic=it;
}

void sndmUpdate(SoundManager* SM)
{
	if (!lstEmpty(&SM->Sounds))
	{
		Node* snd, *last_snd;
		snd=lstFirst(&SM->Sounds);

		while (snd!=NULL)
		{
			if (((sf::Sound*)nodeGetData(snd))->getStatus()==sf::Sound::Stopped)
			{
				delete ((sf::Sound*)nodeGetData(snd));
				last_snd=snd;
				snd=nodeGetNext(snd);
				lstRem(&SM->Sounds, last_snd);
			}
			else
				snd=nodeGetNext(snd);
		}
	}

	if (SM->CurrentMusic != SM->Musics.end())
	{
		// PlayCount
		SM->LastTimeOffset = SM->CurrentTimeOffset;
		SM->CurrentTimeOffset = SM->CurrentMusic->second->getPlayingOffset().asSeconds();
		if (SM->CurrentTimeOffset < SM->LastTimeOffset)
			SM->PlayCount++;
		if (SM->MaxPlayCount > 0 && SM->PlayCount >= SM->MaxPlayCount && !sndmIsInFading(SM))
		{
			SM->NextMusic = ++SM->CurrentMusic;
			if (SM->NextMusic == SM->Musics.end())
				SM->NextMusic = SM->Musics.begin();
			SM->FadeSpeed = SM->DefaultFadingSpeed;
		}

	}


	//Fading effect
	if (!SM->Paused && SM->FadeSpeed>0.f)
	{

		if (SM->IsFading) //fade vers la nouvelle musique
		{
			SM->NextMusic->second->setVolume(MIN(100.f, SM->NextMusic->second->getVolume()+SM->FadeSpeed));
			if (SM->NextMusic->second->getVolume()>=100.f)
			{
				SM->FadeSpeed=-1.f;
				SM->IsFading=0.f;
			}
		}
		else
		{
			//int TotalFaded(0);
			bool IsPlaying(0);
			for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
			{
				if (it->second->getStatus()==sf::Music::Playing)
				{
					it->second->setVolume(it->second->getVolume()-SM->FadeSpeed);
					IsPlaying=1;
					if (it->second->getVolume()<=0.f)
					{
						sndmStopAllMusic(SM);
						it->second->setVolume(100.f); //On retablie le volume
						SM->IsFading=1;
						if (SM->NextMusic!=SM->Musics.end())
						{
							SM->NextMusic->second->setVolume(0.f);
							SM->NextMusic->second->play();
							SM->NextMusic->second->setLoop(SM->Loop);
							SM->CurrentMusic = SM->NextMusic;
							SM->PlayCount = 0;
						}
						else
						{
							SM->IsFading=0;
							SM->FadeSpeed=0.f;
						}

					}
				}
			}

			if (!IsPlaying) //Tout est stoppé!
			{
				SM->IsFading=1;
				SM->NextMusic->second->setVolume(0.f);
				SM->NextMusic->second->play();
				SM->NextMusic->second->setLoop(SM->Loop);
				SM->CurrentMusic = SM->NextMusic;
				SM->PlayCount = 0;
			}

		}

	}

}

void sndmPauseAllSounds(SoundManager* SM)
{
	if (lstEmpty(&SM->Sounds))
		return;
	Node* snd;
	snd=lstFirst(&SM->Sounds);

	while (snd!=NULL)
	{
		sf::Sound* snd_ptr=((sf::Sound*)nodeGetData(snd));
		snd_ptr->pause();
		snd=nodeGetNext(snd);
	}
}

void sndmResumeAllSounds(SoundManager* SM)
{
	if (lstEmpty(&SM->Sounds))
		return;
	Node* snd;
	snd=lstFirst(&SM->Sounds);

	while (snd!=NULL)
	{
		sf::Sound* snd_ptr=((sf::Sound*)nodeGetData(snd));
		snd_ptr->play();
		snd=nodeGetNext(snd);
	}
}

void sndmPauseMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		if (it->second->getStatus() == sf::Music::Playing)
			it->second->pause();
	SM->Paused=1;
}

void sndmResumeMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		if (it->second->getStatus() == sf::Music::Paused)
			it->second->play();
	SM->Paused=0;
}


void sndmStopAll(SoundManager* SM)
{
	if (lstEmpty(&SM->Sounds))
		return;
	Node* snd, *last_snd;
	snd=lstFirst(&SM->Sounds);

	while (snd!=NULL)
	{
		delete ((sf::Sound*)nodeGetData(snd));
		last_snd=snd;
		snd=nodeGetNext(snd);
		lstRem(&SM->Sounds, last_snd);

	}

}

void sndmStopMusic(SoundManager* SM, const char *Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());

	it->second->stop();
}

void sndmStopAllMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		it->second->stop();
}


void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position)
{
	SM->Listener.setPosition(Position.x, Position.y, 5.f);
}

Vec2 sndmGetListenerPosition(const SoundManager* SM)
{
	//vec2(float, float) ne marche pas ?!
	Vec2 V;
	V.x=SM->Listener.getPosition().x;
	V.y=SM->Listener.getPosition().y;
	return V;
	//return vec2(SM->Listener.GetPosition().x, SM->Listener.GetPosition().y);
}

