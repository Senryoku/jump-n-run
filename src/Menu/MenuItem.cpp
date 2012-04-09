#include "MenuItem.h"


void mniInit(MenuItem* I, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	I->Text = (char*)malloc(sizeof(char)*strlen(Text));
	strcpy(I->Text, Text);
	I->Zoom = 1.f;
	I->Function = Function;
	I->Data = Data;
}

void mniFree(MenuItem* I)
{
	free(I->Text);
}

void mniSetText(MenuItem* I, const char* Text)
{
	free(I->Text);
	I->Text = (char*)malloc(sizeof(char)*strlen(Text));
	strcpy(I->Text, Text);
}

const char* mniGetText(const MenuItem* I)
{
	return I->Text;
}

void mniSetType(MenuItem* I, ItemType Type)
{
	I->Type = Type;
}

ItemType mniGetType(const MenuItem* I)
{
	return I->Type;
}

float* mniGetZoom(MenuItem* I)
{
	return &I->Zoom;
}

void* mniGetData(MenuItem* I)
{
	return I->Data;
}

void mniRunFunction(MenuItem* I)
{
	if (I->Function != NULL)
		(*I->Function)();
}