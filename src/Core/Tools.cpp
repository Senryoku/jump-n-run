#include "Tools.h"

#include <stdlib.h>
#include <stdio.h>
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