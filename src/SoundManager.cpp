#include "SoundManager.h"
#include <new>
#include <stdio.h>



void sndmInit(SoundManager* SM)
{
	lstInit(&(SM->Sounds));
	SM->Listener.SetPosition(0.f, 0.f, 5.f);
}

void sndmFree(SoundManager* SM)
{
	sndmStopAll(SM);
	lstFree(&(SM->Sounds));
	
	for (std::map<std::string, sf::SoundBuffer*>::iterator it=SM->SoundBuffers.begin(); it!=SM->SoundBuffers.end(); it++)
		delete it->second;
	SM->SoundBuffers.clear();
}

void sndmLoadFile(SoundManager* SM, const std::string &Key, const std::string &File)
{
	//On vérifie que la clé n'existe pas déjà
	assert(SM->SoundBuffers.count(Key)==0);
	
	SM->SoundBuffers[Key] = new sf::SoundBuffer;
	SM->SoundBuffers[Key]->LoadFromFile(File);
}

void sndmPlay(SoundManager* SM, const std::string &Key, const Vec2 &Position)
{
	std::map<std::string, sf::SoundBuffer*>::iterator it = SM->SoundBuffers.find(Key);
	assert(it != SM->SoundBuffers.end());
	
	sf::Sound *snd=new sf::Sound;
	
	lstAdd(&(SM->Sounds), snd);
	snd->SetBuffer(*(it->second));
	snd->SetPosition(Position.x, Position.y, 0.f);
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

void sndmUpdate(SoundManager* SM)
{
	if (lstEmpty(&SM->Sounds))
		return;
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
			printf("Un son eliminé");
		}
		else
			snd=nodeGetNext(snd);
	}
	
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


void sndmSetListenerPosition(SoundManager* SM, const Vec2& Position)
{
	SM->Listener.SetPosition(Position.x, Position.y, 5.f);
}

Vec2 sndmGetListenerPosition(const SoundManager* SM)
{
	return vec2(SM->Listener.GetPosition().x, SM->Listener.GetPosition().y);
}

