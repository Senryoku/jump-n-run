#ifndef _REPLAY_H
#define _REPLAY_H

#include <stdio.h>
#include <stdlib.h>

//Définition des touches possible pour le joueur
#define ACTION_MOVELEFT 0x01
#define ACTION_MOVERIGHT 0x02
#define ACTION_JUMP 0x04
#define ACTION_ROTLEFT 0x08
#define ACTION_ROTRIGHT 0x10
#define ACTION_GETUP 0x20
#define END_OF_FILE 0xff

#define ACTION_SIZE 1 //taille en octets d'un enregistrement des actions

typedef unsigned char rpAction;

typedef struct s_Replay {
	FILE* f;
} Replay;


Replay* newReplay();

void delReplay(Replay* R);

void rpInitRecording(Replay* R, const char* File, const char LevelFile[255]);

void rpInitReading(Replay* R, const char* File, char* LevelFile);

void rpAddAction(Replay* R, rpAction Action);

rpAction rpReadAction(Replay* R);




#endif