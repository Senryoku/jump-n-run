#include "MenuItem.h"
#include "Menu.h"


void mniInit(MenuItem* I, const char* Text, ItemType Type, void (*Function)(void), void* Data)
{
	I->Text = (char*) malloc(sizeof(char)*(strlen(Text) + 1));
	strcpy(I->Text, Text);
	I->Zoom = 1.f;
	I->Function = Function;
	I->Data = Data;
	I->Type = Type;
	I->FunctionArg = NULL;
	I->Prec = 2;
	
	if (I->Type == ITEM_MENU_SWITCHER)
	{
		I->Data = malloc(sizeof(unsigned short));
		*(unsigned short*)I->Data = *(unsigned short*)Data;
	}
	
	if (I->Type == ITEM_INPUT_MULTILINE || I->Type == ITEM_INPUT)
		I->Data = new std::string;
	I->MaxValue = INFINITY;
	I->MinValue = -INFINITY;
	
	if (I->Type == ITEM_INPUT_VALUE)
	{
		char input[100];
		sprintf(input, "%.2f", *(float*)I->Data);
		I->Str = new std::string;
		*I->Str = std::string(input);
	}
	
}

void mniInitWithArg(MenuItem* I, const char* Text, void (*Function)(void*), void* Data)
{
	mniInit(I, Text, ITEM_BUTTON, NULL, Data);
	I->FunctionArg = Function;
	
}

void mniFree(MenuItem* I)
{
	free(I->Text);
	if (I->Type == ITEM_INPUT_MULTILINE || I->Type == ITEM_INPUT)
		delete (std::string*)I->Data;
	if (I->Type == ITEM_MENU_SWITCHER)
		free(I->Data);
	if (I->Type == ITEM_INPUT_VALUE)
		delete I->Str;
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

void mniSetFloatPrecision(MenuItem* I, unsigned char Prec)
{
	I->Prec = Prec;
	if (I->Type != ITEM_INPUT_VALUE)
		return;
	
	char input[100], tmp[5];
	sprintf(tmp, "a.%uf", I->Prec);
	tmp[0]='%';
	sprintf(input, tmp, *(float*)I->Data);
	*I->Str = std::string(input);
}


unsigned char mniGetFloatPrecision(const MenuItem* I)
{
	return I->Prec;
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

const std::string& mniGetInput(const MenuItem* I)
{
	assert(I->Type == ITEM_INPUT_VALUE || I->Type == ITEM_INPUT ||I->Type == ITEM_INPUT_MULTILINE);
	
	if (I->Type == ITEM_INPUT_VALUE)
		return *I->Str;
	else
		return *static_cast<std::string*>(I->Data);
}

void mniSetInput(MenuItem* I, const char* Text)
{
	assert(I->Type == ITEM_INPUT_VALUE || I->Type == ITEM_INPUT ||I->Type == ITEM_INPUT_MULTILINE);
	
	if (I->Type == ITEM_INPUT_VALUE)
		*I->Str = Text;
	else
		*static_cast<std::string*>(I->Data) = Text;
	
}

const char* mniGetInputChr(const MenuItem* I)
{
	assert(I->Type == ITEM_INPUT_VALUE || I->Type == ITEM_INPUT ||I->Type == ITEM_INPUT_MULTILINE);
	
	if (I->Type == ITEM_INPUT_VALUE)
		return I->Str->c_str();
	else
		return static_cast<std::string*>(I->Data)->c_str();
}

void mniRunFunction(MenuItem* I)
{
	if (I->Function != NULL)
		(*I->Function)();
	else if (I->FunctionArg != NULL)
		(*I->FunctionArg)(I->Data);
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

void mniUse(SMenu* M, MenuItem* I, Bool EnterPressed, ItemDirection IDir, unsigned char KeyCode, Bool Del)
{
	if (I == NULL)
		return;
	
	void* Data;
	
	switch (mniGetType(I))
	{
		case ITEM_BUTTON:
			if (EnterPressed)
				mniRunFunction(I);
			break;
		case ITEM_CHECKBOX:
			Data = mniGetData(I);
			if (Data!=NULL && (EnterPressed || IDir != MOVE_NONE))
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
			else if (KeyCode>=32 && KeyCode<=126) /* printable chars */
				str->push_back(KeyCode);
			//printf("str: %s\n", str->c_str());
			break;
		}
		case ITEM_INPUT_MULTILINE:
		{
			std::string* str = (std::string*)I->Data;
			if (Del && str->size()>0)
				str->resize(str->size() - 1);
			else if (KeyCode>=32 && KeyCode<=126) /* printable chars */
				str->push_back(KeyCode);
			else if (EnterPressed)
				str->push_back('\n');
			//printf("str: %s\n", str->c_str());
			break;
		}
		case ITEM_INPUT_VALUE:
		{
			
			if (Del && I->Str->size()>0)
				I->Str->resize(I->Str->size() - 1);
			else
			{
				if (KeyCode == ',')
					KeyCode = '.';
				
				if ((KeyCode >= '0' && KeyCode <= '9') || (KeyCode == '.' && I->Str->find('.') == std::string::npos))
						I->Str->push_back(KeyCode);
			}
			
			sscanf(I->Str->c_str(), "%f", (float*)I->Data);
				
			break;
		}
		case ITEM_VALUE:
			Data = mniGetData(I);
			if (Data!=NULL)
			{
				if (IDir == MOVE_RIGHT)
					*((float*) Data) += I->Incr; 
				else if (IDir == MOVE_LEFT)
					*((float*) Data) -= I->Incr;
				
				if (*((float*) Data) > I->MaxValue)
					*((float*) Data) = I->MaxValue;
				
				if (*((float*) Data) < I->MinValue)
					*((float*) Data) = I->MinValue;
			}
			break;
		case ITEM_MENU_SWITCHER:
			if (EnterPressed || IDir != MOVE_NONE)
				mnGoToMenu(M, *(MenuID*)I->Data);
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
