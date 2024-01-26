#include "StdAfx.h"
#include "hGraphics.h"
#include "hGlobal.h"

/*
#include <QtWidgets>
#include <QtWinExtras>
#include <QBitmap>

#include "opencv2/opencv.hpp"
*/
//Neu
//#include <Windows.h>
//#include "ERIS.h"
#include <iostream>
void oGraphicControl::Init(CWnd *Wnd, CWnd *ParenWnd)
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	myControl = Wnd;
	pMyGraphic = new Graphics(*myControl);
	ClearGraphic();
	
}

void oGraphicControl::DeInit(void)
{
	ClearGraphic();
	myControl = NULL;
	delete(pMyGraphic);
	pMyGraphic = NULL;
	GdiplusShutdown(gdiplusToken);
}

void oGraphicControl::Reset(void)
{
	if (pMyGraphic == NULL) { return; }
	delete pMyGraphic;
	GdiplusShutdown(gdiplusToken);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	pMyGraphic = new Graphics(*myControl);
}
//Original
/*
void oGraphicControl::DrawingBitmap(Bitmap* MyBitmap)
{
	if (MyBitmap == NULL) { return; }

	ClearGraphic();
	pMyGraphic->DrawImage(MyBitmap, 0, 0);
}
*/

void oGraphicControl::DrawingBitmap(Bitmap* MyBitmap)
{
	if (MyBitmap == NULL) {
		std::cout << "Bitmap 0" << std::endl;
		return;
	}
	std::cout << "Try to Clear Graphic" << std::endl;
	ClearGraphic();
	std::cout << "Try to Draw Image" << std::endl;
	pMyGraphic->DrawImage(MyBitmap, 0, 0);
	std::cout << "image draw" << std::endl;
}
/*
void oGraphicControl::DrawingBitmap(Bitmap *MyBitmap)
{
	std::cout << "Start Drawing Bitmap" << std::endl;
	if (MyBitmap==NULL) { 
		std::cout << "Is Empty" << std::endl; 
		return; }

	ClearGraphic();
	std::cout << "Clear Image" << std::endl;
	pMyGraphic->DrawImage(MyBitmap, 0, 0);
	std::cout << "Draw Image" << std::endl;
	
	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	//pMyGraphic->DrawImage(MyBitmap, 0, 0);

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, MyBitmap->GetWidth(), MyBitmap->GetHeight());
	std::cout << "Lock Bits" << std::endl;
	MyBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, MyBitmap->GetPixelFormat(), &bitmapData);
	//void scanData = &bitmapData.Scan0;
	std::cout << "Create Mat" << std::endl;
	cv::Mat im(MyBitmap->GetHeight(), MyBitmap->GetWidth(), MyBitmap->GetPixelFormat(), bitmapData.Scan0, std::abs(bitmapData.Stride));
	//QImage image(bitmapData, bitmapData.Width, bitmapData.Height, QImage::Format_ARGB32);
	MyBitmap->UnlockBits(&bitmapData);
	// Convert the QImage to a QPixmap
	//QPixmap pixmap = QPixmap::fromImage();
	QPixmap p = QPixmap::fromImage(QImage((unsigned char*)im.data,
		im.cols,
		im.rows,
		QImage::Format_RGB888));
	p = p.scaledToWidth(500);
	std::cout << "Create Pixmap" << std::endl;
	
	//uiERIS.setupUi(this);
	//erisClass.
	//Insp::ERIS erisClass;
	//erisClass.uiERIS.label_Bild->setPixmap(p);
	//erisClass.label_Bild->setPixmap(p);
	std::cout << "Pixmap set" << std::endl;
	Gdiplus::GdiplusShutdown(token);
	//return p;
}*/


/*
QPixmap oGraphicControl::DrawingBitmap(Bitmap* MyBitmap)
{
	
	std::cout << "Start Drawing Bitmap" << std::endl;
	if (MyBitmap == NULL) {
		std::cout << "Is Empty" << std::endl;
		return;
	}

	ClearGraphic();
	std::cout << "Clear Image" << std::endl;
	pMyGraphic->DrawImage(MyBitmap, 0, 0);
	std::cout << "Draw Image" << std::endl;

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	//pMyGraphic->DrawImage(MyBitmap, 0, 0);

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, MyBitmap->GetWidth(), MyBitmap->GetHeight());
	std::cout << "Lock Bits" << std::endl;
	MyBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, MyBitmap->GetPixelFormat(), &bitmapData);
	//void scanData = &bitmapData.Scan0;
	std::cout << "Create Mat" << std::endl;
	cv::Mat im(MyBitmap->GetHeight(), MyBitmap->GetWidth(), MyBitmap->GetPixelFormat(), bitmapData.Scan0, std::abs(bitmapData.Stride));
	//QImage image(bitmapData, bitmapData.Width, bitmapData.Height, QImage::Format_ARGB32);
	MyBitmap->UnlockBits(&bitmapData);
	// Convert the QImage to a QPixmap
	//QPixmap pixmap = QPixmap::fromImage();
	QPixmap p = QPixmap::fromImage(QImage((unsigned char*)im.data,
		im.cols,
		im.rows,
		QImage::Format_RGB888));
	p = p.scaledToWidth(500);
	std::cout << "Create Pixmap" << std::endl;

	//uiERIS.setupUi(this);
	//erisClass.
	//Insp::ERIS erisClass;
	//erisClass.uiERIS.label_Bild->setPixmap(p);
	//erisClass.label_Bild->setPixmap(p);
	std::cout << "Pixmap set" << std::endl;
	Gdiplus::GdiplusShutdown(token);
	return p;
	*/


	/*
	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	// Create a Graphics object and draw something on it
	HDC hdc = GetDC(nullptr);
	Gdiplus::Graphics* graphics = new Gdiplus::Graphics(hdc);
	graphics->Clear(Gdiplus::Color::White);
	graphics->DrawEllipse(new Gdiplus::Pen(Gdiplus::Color::Black, 3.0f), 10, 10, 100, 100);

	// Create a Bitmap object and draw the contents of the Graphics object onto it
	Gdiplus::Bitmap bitmap(120, 120, PixelFormat32bppARGB);
	Gdiplus::Graphics bitmapGraphics(&bitmap);
	bitmapGraphics.DrawImage(graphics->GetVisibleClipBounds().X, graphics->GetVisibleClipBounds().Y, graphics);

	// Use the Bitmap object for further processing
	// ...

	// Clean up
	delete graphics;
	ReleaseDC(nullptr, hdc);

	// Shut down GDI+
	Gdiplus::GdiplusShutdown(token);
	*/

	/*
	HICON hIcon;
	Status st = MyBitmap.GetHICON(&hIcon);
	QPixmap pixM = QtWin::fromHICON(hIcon);
	
}*/

void oGraphicControl::ClearGraphic(void)
{
	if (pMyGraphic == NULL) { return; }
	pMyGraphic->Clear(Color(255, 255, 255));
}
