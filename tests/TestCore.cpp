#include <string.h>

#include <Core/Vec2.h>
#include <Core/DynArr.h>
#include <Core/List.h>
#include <Core/Tools.h>
#include <Core/md5.h>

int main(void)
{
	vec2RegressionTest();
	daRegressionTest();
	lstRegressionTest();

	printf("Test MD5\n");
	char md5Test[255];
	strcpy(md5Test, md5FromFile("tests/TestCore.cpp").c_str());
	for(unsigned int i = 0; i < 100; i++)
		assert(strcmp(md5Test, md5FromFile("tests/TestCore.cpp").c_str()) == 0);
	strcpy(md5Test, md5FromFile("src/Core/Vec2.h").c_str());
	for(unsigned int i = 0; i < 100; i++)
		assert(strcmp(md5Test, md5FromFile("src/Core/Vec2.h").c_str()) == 0);
	printf("MD5 OK\n");

	return 0;
}
