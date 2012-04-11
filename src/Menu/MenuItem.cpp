#include "MenuItem.h"


void mniInit(MenuItem* I, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	I->Text = (char*)malloc(sizeof(char)*strlen(Text));
	strcpy(I->Text, Text);
	I->Zoom = 1.f;
	I->Function = Function;
	I->Data = Data;
	I->Type = Type;
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

void foo(void);

///@todo vérifier les erreurs données par valgrind
void mniRegressionTest(void)
{
	MenuItem I, I2;
	float data;
	
	mniInit(&I, "Item 1", ITEM_LABEL, NULL, NULL);
	mniInit(&I2, "Item2", ITEM_BUTTON, &foo, &data);
	
	printf("Item 1 text: %s\nItem 2 text: %s\n", mniGetText(&I), mniGetText(&I2));
	mniSetText(&I, "Nouveau texte");
	printf("Item 1 text: %s\nItem 2 text: %s\n", mniGetText(&I), mniGetText(&I2));
	
	mniRunFunction(&I2);
	
	mniFree(&I);
	mniFree(&I2);
}

void foo(void)
{
	printf("foo\n");
}