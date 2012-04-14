#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <SFML/Network.hpp>
#include <Core/Tools.h>

typedef struct
{
	char Player[255];
	char LvlName[255];
	char LvlMD5[255];
	unsigned int Time;
} Score;

void scInit(Score* S, char Player[255], char LvlName[255], char LvlMD5[255], unsigned int Time);

Bool scSend(Score *S);

void scFree(Score *S);

#endif // _SCORE_H_
