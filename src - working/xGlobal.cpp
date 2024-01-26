#include "StdAfx.h"
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include "hGlobal.h"

using namespace std;

void FormatDoubleToString(const double Value, int Precision, char * unit, char* Output)
{
	char Instr[8] = "%.0f %s";
	Instr[2] =  0x30 + Precision;

	sprintf(Output, Instr, Value, unit);
}

void FormatIntToString(const int Value, char * unit, char *Output)
{
	sprintf(Output, "%i %s", Value, unit);
}


void WriteParameterToEdit(CWnd *Ctrl, char *Input)
{
	if (Ctrl != NULL)
	{
		Ctrl->SetWindowText(Input);
	}
}

void SetCWndPosition(CWnd *Control,int x, int y, int width, int height)
{
	RECT target;
	target.top = y;
	target.bottom = y + height;
	target.left = x;
	target.right = x + width;

	MoveWindow(*Control, x, y, width, height, false);
}

void FreeAndNull(void *Buffer)
{
	free(Buffer);
	Buffer = NULL;
}


