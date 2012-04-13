#include "Tools.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(__WIN32__)
// Windows
#define SYS_WINDOWS
#include <direct.h>     // _mkdir

#elif defined(linux) || defined(__linux)
// Linux
#define SYS_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
// Mac OS
#define SYS_MACOS

#endif


void Wobble(float *CurrentPosition, float TargetPosition, float Force,float Friction, float *Spd)
{
	*Spd=((*Spd)+((TargetPosition-(*CurrentPosition))/2.f)*Force)*(1.f-Friction);
	*CurrentPosition+=(*Spd);
}

Bool DirectoryExists(const char* Dir)
{
	struct stat buf;
	if (stat(Dir, &buf) == 0)
	{
		return (buf.st_mode & S_IFDIR) != 0;
	}
	return FALSE;
}


Bool CreateDirectory(const char* Dir)
{
	Bool success = FALSE;

#ifdef SYS_WINDOWS
	success = _mkdir(Dir) == 0;
#elif defined(SYS_LINUX) || defined(SYS_MACOS)
	success = mkdir(Dir, 0755) == 0;
#endif

	if (!success)
		printf("Pas possible de créer le dossier %s\n", Dir);

	return success;
}

Config GetConfiguration()
{
	Config Cfg;
	char line[255];
	char id[255];
	float value;

	/* Options par défaut */
	Cfg.WindowWidth = 800.f;
	Cfg.WindowHeight = 600.f;
	Cfg.FPSLimit = 60.f;
	Cfg.AntiAliasing = 1.f;
	Cfg.VerticalSync = 1.f;

	FILE* f;
	f = fopen("Config.cfg", "r");
	if(f != NULL)
	{
		while (fgets(line, 255, f) != NULL)
		{
			sscanf(line, "%s %f\n", id, &value);
			printf("Lu : %s, %f\n", id, value);
			if(strcmp(id, "WindowWidth") == 0) Cfg.WindowWidth = value;
			if(strcmp(id, "WindowHeigth") == 0) Cfg.WindowHeight = value;
			if(strcmp(id, "FPSLimit") == 0) Cfg.FPSLimit = value;
			if(strcmp(id, "AntiAliasing") == 0) Cfg.AntiAliasing = value;
			if(strcmp(id, "VerticalSync") == 0) Cfg.VerticalSync = value;
		}
	} else {
		/* Création d'un fichier de configuration par défaut */
		f = fopen("Config.cfg", "w");

		if (f != NULL)
			fprintf(f, "WindowWidth %f\nWindowHeight %f\nFPSLimit %f\nAntiAlising %f\nVerticalSync %f\n", Cfg.WindowWidth, Cfg.WindowHeight, Cfg.FPSLimit, Cfg.AntiAliasing, Cfg.VerticalSync);
		else
			printf("Erreur d'écriture du fichier de configuration par défaut\n");
	}
	fclose(f);

	return Cfg;
}
