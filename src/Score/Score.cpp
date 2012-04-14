#include "Score.h"

void scInit(Score* S, char Player[255], char LvlName[255], char LvlMD5[255], unsigned int Time)
{
	strcpy(S->Player, Player);
	strcpy(S->LvlName, LvlName);
	strcpy(S->LvlMD5, LvlMD5);
	S->Time = Time;
}

Bool scSend(Score* S)
{
	sf::Http http;
	http.setHost("http://maretverdant.free.fr");

	/* Conversion unsigned iny -> char* */
	char Time[255];
	sprintf(Time, "%d", S->Time);

	char Body[5000];
	Body[0] = '\0';
	strcat(Body, "Player=");
	strcat(Body, S->Player);
	strcat(Body, "&LvlName=");
	strcat(Body, S->LvlName);
	strcat(Body, "&LvlMD5=");
	strcat(Body, S->LvlMD5);
	strcat(Body, "&Time=");
	strcat(Body, Time);

	sf::Http::Request request("JumpNRun/submit.php");
    request.setMethod(sf::Http::Request::Post);
    request.setBody(Body);
	sf::Http::Response response = http.sendRequest(request);

	sf::Http::Response::Status status = response.getStatus();
	if (status == sf::Http::Response::Ok)
	{
	    //std::cout << response.getBody() << std::endl; // Debug
		return 0;
	} else {
		return 1;
	}
}

void scFree(Score *S)
{
	strcpy(S->Player, "");
	strcpy(S->LvlName, "");
	strcpy(S->LvlMD5, "");
	S->Time = 0;
}
