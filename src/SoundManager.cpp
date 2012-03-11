#include "SoundManager.h"

#include <stdio.h>



void sndmInit(SoundManager* SM)
{
	lstInit(&(SM->Sounds));
	SM->Listener.SetPosition(0.f, 0.f, 5.f);
	SM->FadeSpeed=0.f;
	SM->IsFading=0;
	SM->Loop=1;
	//SM->NextMusic=;
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

bool sndmLoadSoundFile(SoundManager* SM, const std::string &Key, const std::string &File)
{
	//On vérifie que la clé n'existe pas déjà
	assert(SM->SoundBuffers.count(Key)==0);
	
	SM->SoundBuffers[Key] = new sf::SoundBuffer;
	if (!SM->SoundBuffers[Key]->LoadFromFile(File))
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

bool sndmLoadMusicFile(SoundManager* SM, const std::string &Key, const std::string &File)
{
	//On vérifie que la clé n'existe pas déjà
	assert(SM->Musics.count(Key)==0);
	
	SM->Musics[Key] = new sf::Music;
	if (!SM->Musics[Key]->OpenFromFile(File))
	{
		//Le chargement a raté
		delete SM->Musics[Key];
		std::map<std::string, sf::Music*>::iterator it;
		it=SM->Musics.find(Key);
		SM->Musics.erase(it);
		return 0;
	}
	
	return 1;
}

void sndmPlay(SoundManager* SM, const std::string &Key, const Vec2 &Position, float MinDist, float Attenuation)
{
	std::map<std::string, sf::SoundBuffer*>::iterator it = SM->SoundBuffers.find(Key);
	assert(it != SM->SoundBuffers.end());
	
	sf::Sound *snd=new sf::Sound;
	
	lstAdd(&(SM->Sounds), snd);
	snd->SetBuffer(*(it->second));
	snd->SetPosition(Position.x, Position.y, 0.f);
	snd->SetMinDistance(MinDist);
	snd->SetAttenuation(Attenuation);
	snd->Play();
}

void sndmPlay(SoundManager* SM, const std::string &Key)
{
	std::map<std::string, sf::SoundBuffer*>::iterator it = SM->SoundBuffers.find(Key);
	assert(it != SM->SoundBuffers.end());
	
	sf::Sound *snd=new sf::Sound;
	
	lstAdd(&(SM->Sounds), snd);
	snd->SetBuffer(*(it->second));
	snd->SetRelativeToListener(1); //Le son est joué sans tenir compte de la position
	snd->Play();
	
}

void sndmPlayMusic(SoundManager* SM, const std::string &Key, bool Loop)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	it->second->SetLoop(Loop);
	it->second->Play();
}

void sndmMusicSetVolume(SoundManager* SM, const std::string &Key, float Volume)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	it->second->SetVolume(Volume);
}

void sndmMusicSetPitch(SoundManager* SM, const std::string &Key, float Pitch)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	it->second->SetPitch(Pitch);
}

float sndmMusicGetVolume(SoundManager* SM, const std::string &Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	return it->second->GetVolume();
}

float sndmMusicGetPitch(SoundManager* SM, const std::string &Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	return it->second->GetPitch();
}

void sndmMusicFade(SoundManager* SM, const std::string &NextKey, float FadeSpeed, bool Loop)
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
			if (((sf::Sound*)nodeGetData(snd))->GetStatus()==sf::Sound::Stopped)
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
	
	//Fading effect
	if (SM->FadeSpeed>0.f)
	{
		if (SM->IsFading) //fade ver la nouvelle musique
		{
			SM->NextMusic->second->SetVolume(MIN(100.f, SM->NextMusic->second->GetVolume()+SM->FadeSpeed));
			if (SM->NextMusic->second->GetVolume()>=100.f)
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
				if (it->second->GetStatus()==sf::Music::Playing)
				{
					it->second->SetVolume(it->second->GetVolume()-SM->FadeSpeed);
					IsPlaying=1;
					if (it->second->GetVolume()<=0.f)
					{
						sndmStopAllMusic(SM);
						it->second->SetVolume(100.f); //On retablie le volume
						SM->IsFading=1;
						if (SM->NextMusic!=SM->Musics.end())
						{
							SM->NextMusic->second->SetVolume(0.f);
							SM->NextMusic->second->Play();
							SM->NextMusic->second->SetLoop(SM->Loop);
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
				SM->NextMusic->second->SetVolume(0.f);
				SM->NextMusic->second->Play();
				SM->NextMusic->second->SetLoop(SM->Loop);
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
		snd_ptr->Pause();
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
		snd_ptr->Play();
		snd=nodeGetNext(snd);		
	}
}

void sndmPauseMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		if (it->second->GetStatus() == sf::Music::Playing)
			it->second->Pause();
}

void sndmResumeMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		if (it->second->GetStatus() == sf::Music::Paused)
			it->second->Play();
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

void sndmStopMusic(SoundManager* SM, const std::string &Key)
{
	std::map<std::string, sf::Music*>::iterator it = SM->Musics.find(Key);
	assert(it != SM->Musics.end());
	
	it->second->Stop();
}

void sndmStopAllMusic(SoundManager* SM)
{
	for (std::map<std::string, sf::Music*>::iterator it = SM->Musics.begin(); it!=SM->Musics.end(); it++)
		it->second->Stop();
}


void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position)
{
	SM->Listener.SetPosition(Position.x, Position.y, 5.f);
}

Vec2 sndmGetListenerPosition(const SoundManager* SM)
{
	//vec2(float, float) ne marche pas ?!
	Vec2 V;
	V.x=SM->Listener.GetPosition().x;
	V.y=SM->Listener.GetPosition().y;
	return V;
	//return vec2(SM->Listener.GetPosition().x, SM->Listener.GetPosition().y);
}

