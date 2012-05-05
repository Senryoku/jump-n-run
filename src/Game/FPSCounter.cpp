#include "FPSCounter.h"

void fpsInit(FPSCounter* F)
{
	F->Frames = 0;
	F->LastFPS = 0;
	F->Changed = FALSE;
	F->FPSString = "0.0";
	F->Clock.restart();
}

void fpsReset(FPSCounter* F)
{
	F->Frames = 0;
	F->Clock.restart();
}


void fpsStep(FPSCounter* F)
{
	F->Frames++;
	F->Changed = FALSE;
	if (F->Clock.getElapsedTime().asSeconds()>=1.f)
	{
		F->LastFPS = F->Frames/F->Clock.getElapsedTime().asSeconds();
		F->Frames = 0;
		F->Clock.restart();
		char Text[10];
		sprintf(Text, "%.1f", F->LastFPS);
		F->FPSString = Text;
		F->Changed = TRUE;
	}
}

const std::string& fpsGetString(const FPSCounter* F)
{
	return F->FPSString;
}

const char* fpsGetChar(const FPSCounter* F)
{
	return F->FPSString.c_str();
}

float fpsGetNumber(const FPSCounter* F)
{
	return F->LastFPS;
}

Bool fpsChanged(const FPSCounter* F)
{
	return F->Changed;
}