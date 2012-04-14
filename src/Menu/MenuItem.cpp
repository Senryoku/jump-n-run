#include "MenuItem.h"
#include <string>


void mniInit(MenuItem* I, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	I->Text = (char*) malloc(sizeof(char)*(strlen(Text) + 1));
	strcpy(I->Text, Text);
	I->Zoom = 1.f;
	I->Function = Function;
	I->Data = Data;
	I->Type = Type;
	
	if (I->Type == ITEM_INPUT)
		I->Data = new std::string;
}

void mniFree(MenuItem* I)
{
	free(I->Text);
	if (I->Type == ITEM_INPUT)
		delete (std::string*)I->Data;
}

void mniSetText(MenuItem* I, const char* Text)
{
	free(I->Text);
	I->Text = (char*) malloc(sizeof(char)*(strlen(Text) + 1));
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

void mniUse(MenuItem* I, Bool EnterPressed, ItemDirection IDir, unsigned char KeyCode, Bool Del)
{
	void* Data;
	switch (mniGetType(I))
	{
		case ITEM_BUTTON:
			if (EnterPressed)
				mniRunFunction(I);
			break;
		case ITEM_CHECKBOX:
			Data = mniGetData(I);
			if (Data!=NULL)
			{
				*((Bool*) Data) = !(*((Bool*) Data) );
				/*if (*((Bool*)(Data))==FALSE)
					memset(Data, TRUE, sizeof(Bool));
				else
					memset(Data, FALSE, sizeof(Bool));
				 */
			}
			break;
		case ITEM_INPUT:
		{
			std::string* str = (std::string*)I->Data;
			if (Del && str->size()>0)
				str->resize(str->size() - 1);
			else
				str->push_back(KeyCode);
			printf("str: %s\n", str->c_str());
			break;
		}
		case ITEM_VALUE:
			Data = mniGetData(I);
			if (Data!=NULL)
			{
				if (IDir == MOVE_RIGHT)
					*((float*) Data) += I->Incr; 
				else
					*((float*) Data) -= I->Incr;
				
				if (*((float*) Data) > I->MaxValue)
					*((float*) Data) = I->MaxValue;
				
				if (*((float*) Data) < I->MinValue)
					*((float*) Data) = I->MinValue;
			}
			
			break;
		default:
			break;
	}
}


void mniSetIncr(MenuItem* I, float Incr)
{
	I->Incr = Incr;
}

float mniGetIncr(const MenuItem* I)
{
	return I->Incr;
}

void mniSetMinMaxValues(MenuItem* I, float Min, float Max)
{
	I->MinValue = Min;
	I->MaxValue = Max;
}