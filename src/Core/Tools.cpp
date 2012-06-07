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

Bool FileExists(const char* File)
{
	FILE* f;
	f = fopen(File, "r");

	if (f != NULL)
	{
		fclose(f);
		return TRUE;
	}

	return FALSE;
}


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

int GetLevels(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        printf("Cannot get acces to dir\n");
        return 1;
    }

	files.clear();

    while ((dirp = readdir(dp)) != NULL) {
		if (strstr(dirp->d_name, ".lvl") != NULL)
			files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
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
	Cfg.PlayMusic = 0.f;
	Cfg.joyButJump = 16;
	Cfg.joyButL = 13;
	Cfg.joyButR = 14;
	Cfg.joyAxisMove = sf::Joystick::X;
	Cfg.joyButUp = 17;
	Cfg.UseJoystick = true;
	Cfg.joyRestart = 18;

	FILE* f;
	f = fopen("Config.cfg", "r");
	if(f != NULL)
	{
		while (fgets(line, 255, f) != NULL)
		{
			sscanf(line, "%s %f\n", id, &value);
			// printf("Lu : %s, %f\n", id, value);
			if(strcmp(id, "WindowWidth") == 0) Cfg.WindowWidth = value;
			if(strcmp(id, "WindowHeight") == 0) Cfg.WindowHeight = value;
			if(strcmp(id, "FPSLimit") == 0) Cfg.FPSLimit = value;
			if(strcmp(id, "AntiAliasing") == 0) Cfg.AntiAliasing = value;
			if(strcmp(id, "VerticalSync") == 0) Cfg.VerticalSync = value;
			if(strcmp(id, "Music") == 0) Cfg.PlayMusic = value;
			if(strcmp(id, "JoyJump") == 0) Cfg.joyButJump = (unsigned int)value;
			if(strcmp(id, "JoyMove") == 0) Cfg.joyAxisMove = static_cast<sf::Joystick::Axis>(value);
			if(strcmp(id, "JoyL") == 0) Cfg.joyButL = (unsigned int)value;
			if(strcmp(id, "JoyR") == 0) Cfg.joyButR = (unsigned int)value;
			if(strcmp(id, "JoyUp") == 0) Cfg.joyButUp = (unsigned int)value;
			if(strcmp(id, "UseJoy") == 0) Cfg.UseJoystick = (bool)value;
			if(strcmp(id, "JoyRestart") == 0) Cfg.joyRestart = (bool)value;
		}
	} else {
		/* Création d'un fichier de configuration par défaut */
		f = fopen("Config.cfg", "w");

		if (f != NULL)
			fprintf(f, "WindowWidth %f\nWindowHeight %f\nFPSLimit %f\nAntiAlising %f\nVerticalSync %f\nMusic %f\n", Cfg.WindowWidth, Cfg.WindowHeight, Cfg.FPSLimit, Cfg.AntiAliasing, Cfg.VerticalSync, Cfg.PlayMusic);
		else
			printf("Error writing to Config.cfg: file cannot be created...\n");
	}
	fclose(f);

	return Cfg;
}

void printError(const char *func, const char *file, int line)
{
	char tmp[300];
	sprintf(tmp, "Error at function %s. In file %s at line %d", func, file, line);
	perror(tmp);
}
