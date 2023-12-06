#pragma once

#include "StdAfx.h"
#include <Windows.h>
#include <ObjIdl.h>
#include "gdiplus.h"	//Needs windows.h
//NEU
//#include "ERIS.h"
// ERROR C-
//#include <QPixmap>

using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")	//if not included causes Linker-Error

#ifndef __GRAPHICS__
#define __GRAPHICS__



class oGraphicControl
{
private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	
	Graphics* pMyGraphic;
	
	//Insp::ERIS erisClass;

public:
	void Init(CWnd *Wnd, CWnd *ParenWnd);
	void DeInit(void);
	//QPixmap DrawingBitmap(Bitmap *MyBitmap);
	void DrawingBitmap(Bitmap* MyBitmap);
	void ClearGraphic(void);
	void Reset(void);

	CWnd				*myControl;
};

#endif // !__GRAPHICS__

