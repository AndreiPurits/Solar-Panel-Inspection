// Kompiler einstellungen
//Eigenschaften - Erweitert MFC als DLL
//Konfigurationseingenschaften->Erwetiert DLL als Lib
//Präprozesspr WIN64?

#include <stdio.h>
#include "stdafx.h"

#include "hGlobal.h"	//has to included before demo.h
#include "hGraphics.h"//Raus?
#include "ERIS.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

oLibHandler Lib;
oGraphicControl MyGraphic;

CEdit* EditLink[PARAM_COUNT];
bool editchangelock = false;
int coordX;
int coordY;






//TODO Höhenbezeichnungen und Berechnungen Vereinheitlichen
using namespace std;
namespace Insp {


	classERIS::classERIS(QWidget* parent) : QMainWindow(parent)
	{
		uiERIS.setupUi(this);
		//TODO Bild skalieren
		//uiERIS.label_Bild->setScaledContents(true);
		//uiERIS.label_Bild->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		QObject::connect(uiERIS.actionImport, SIGNAL(triggered()), this, SLOT(importData()));
		QObject::connect(uiERIS.pushButton_plusFrame, SIGNAL(clicked()), this, SLOT(nextFrame()));
		QObject::connect(uiERIS.pushButton_minusFrame, SIGNAL(clicked()), this, SLOT(previousFrame()));
		QObject::connect(uiERIS.pushButton_frame, SIGNAL(clicked()), this, SLOT(setFrameNumber()));
	}
	
	classERIS::~classERIS() {	}
	void classERIS::setFrameNumber() {
		int frameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		actFN = validFrameNumber(frameNumber);
		//Um immer die aktuell dargestellte Framenummer zu sehen
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN));
		changeFrameNumber();
	}

	void classERIS::nextFrame() {
		int oldFrameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		//TODO Abbruch bei überschreitung des maximalen Frames
		//EInkürzen der changeFrameNumber funktion?
		//Zahl setzen belegen 
		int frameNumber = oldFrameNumber + 1;
		actFN = validFrameNumber(frameNumber);
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN));
		changeFrameNumber();
	}
	void classERIS::previousFrame() {
		int oldFrameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		int frameNumber = oldFrameNumber - 1;
		actFN = validFrameNumber(frameNumber);
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN));
		changeFrameNumber();
	}

	int classERIS::validFrameNumber(int frameNumber) {
		if (frameNumber >= 0 && frameNumber < maxFrameNumber) {
			return frameNumber;
		}
		else {
			return actFN;
		}
	}

	void classERIS::ActualizeFrameParameters(int FrameIndex)
	{
		int ii;
		char Value[64];
		std::cout << "SelectFrame " << std::endl;
		std::cout << "startLoop" << std::endl;
			for (ii = 0; ii < PARAM_COUNT; ii++)
			{
				std::cout << ii << std::endl;
				Lib.GetParameterString(ii, Value);
				std::cout << "Get Param Sring " << Value << std::endl;

			}
	}



	void classERIS::importData() {
		{
			CString filename = "";
			char sFrameCount[11];

			//TODO zurücksetzen auf c
			QString directory = "C:/Program Files/InfraTec/IRBGRAB_SDK/v4/Testdaten"; // Replace with the path to your directory  

			QString filter = "*.irb"; // Replace with the file extension you want to select
			QString selected_file = QFileDialog::getOpenFileName(nullptr, "Select File", directory, filter);
			filename = selected_file.toStdString().c_str();

#ifdef WIN64
			const char* DLLName = "C:\\Programmieren\\ERIS-Auswertesoftware\\build\\Release\\irbacs_win64.dll";
#else
#ifdef WIN32
			const char* DLLName = "C:\\Program Files\\InfraTec\\IRBACS_SDK\\v2\\irbacs_win64.dll";
#else
#error No platform specifies!;
#endif
#endif


			if (!Lib.IsLibLoaded())
			{
				if (Lib.LoadLib(DLLName))
				{
					std::cout << "Load DLL" << std::endl;
					if (Lib.InitFunctions())
					{
						std::cout << "init functions fails" << std::endl;
					}
					else
					{
						int lenA = lstrlenA((LPCTSTR)filename);
						BSTR unicodestr;

						int lenW = ::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, 0, 0);
						if (lenW > 0)
						{
							// Check whether conversion was successful
							unicodestr = ::SysAllocStringLen(0, lenW);
							::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, unicodestr, lenW);
						}

						char* ansistr;
						lenW = ::SysStringLen(unicodestr);
						lenA = ::WideCharToMultiByte(CP_UTF8, 0, unicodestr, lenW, 0, 0, NULL, NULL);
						if (lenA > 0)
						{
							ansistr = new char[lenA + 1]; // allocate a final null terminator as well
							::WideCharToMultiByte(CP_UTF8, 0, unicodestr, lenW, ansistr, lenA, NULL, NULL);
							ansistr[lenA] = 0; // Set the null terminator yourself
						}
						// ###################################

						if (Lib.LoadIRB(ansistr, &MyGraphic))
						{
							std::cout << "Load IRB" << std::endl;
							//sFrameCount = IntToString(Lib.GetFrameCount());
							std::cout <<"Filename: "<< filename <<"\t DLL-Name: \t"<< DLLName << std::endl;


							FormatIntToString(Lib.GetFrameCount(), "", sFrameCount);
							//Maximale Framenumber definiert.
							maxFrameNumber = Lib.GetFrameCount();
							//std::cout << "Max Frame Number \t" << sFrameCount << std::endl;
							std::cout << "Max Frame Number \t" << maxFrameNumber << std::endl;
							std::cout << "Load IRB" << std::endl;
							QString textLabelFrameNummer1 = "Von \t";
							QString textLabel = textLabelFrameNummer1 + sFrameCount;
							uiERIS.label_maxFrame->setText(textLabel);
							//edtFrameCount.SetWindowText(sFrameCount);
							std::cout << "setFrameParams \t"<< sFrameCount << std::endl;
							//TODO
							//ActualizeFrameParameters(0);
							uiERIS.lineEdit_Framenummer->setText(QString::number(0));
							//Actualize Actualisierung der Matrize
							//ActualizePixelValues(&edtTemp, &edtTempBlackbody);
							paintMat(writeTempToMat());
							//TODO raus, erst wenn fehlstelle berechnet wurde anzeigen
							paintMatError(writeTempToMat());


							//Ablauf:: ActualizeFrameParameters(0)->ERIS:: ActualizeFrameParameters-> ERIS Lib.SelectFrame(FrameIndex) oIRBACS.cpp ReadIRBDataUncompressed()-> DrawPictureMonochrome(unsigned char* StartAdress)->DrawingBitmap(&bmp)
							//uiERIS.label_Bild->setPixmap(graphicC.DrawingBitmap(&Lib.DrawPictureMonochrome(Lib.PixelData)));//WEiter
							std::cout << "setFrameParams" << std::endl;
							//ActualizePixelValues(&edtTemp, &edtTempBlackbody);
						}
						else
						{
							//MessageBox(_T("File not found"), _T("Error"), MB_OK);
							std::cout << "File not found" << std::endl;
						}
						//############## Free used memory ####################
						delete[] ansistr;
						::SysFreeString(unicodestr);
						//##################################
					}
				}
				else
				{
					std::cout << "Load DLL fails" << std::endl;
					//btnLoadLib.SetWindowText(_T("Load FAIL"));
				}
			}
			else
			{
				Lib.DeInit();
				MyGraphic.DeInit();
			}
		}


	}

	//Ursprungsfunktion
	void classERIS::ActualizePixelValues(CEdit* edtTemp, CEdit* edtTempBB)
	{
		//std::cout << "Actualized PixelValues" << std::endl;
		char FieldOutput[10];
		if (edtTempBB != NULL)
		{
			FormatDoubleToString(Lib.GetTempBlackBody(coordX, coordY), 2, _T(" K"), FieldOutput);
		}

		if (edtTemp != NULL)
		{
			FormatDoubleToString(Lib.GetTemp(coordX, coordY), 2, _T(" K"), FieldOutput);
		}
	}
	//Neue Funktion
	void classERIS::writeTempretureArrayToFile() {

		std::ofstream saveFile;
		saveFile.open("ThermoArrayTest.txt");
		double tempTemp;
		int pixelX, pixelY;
		for (pixelY = 0; pixelY < 480; pixelY++) {
			for (pixelX = 0; pixelX < 640; pixelX++) {
				tempTemp = Lib.GetTemp(pixelX, pixelY);
				saveFile << tempTemp << ";";
			}
			saveFile << "\n";
		}
		saveFile.close();

	}
	//TODO eingangsparameter ändern
	cv::Mat classERIS::writeTempToMat() {

		
		cv::Mat tempMat(pixelCameraX, pixelCameraY, CV_64F);
		int pixelX, pixelY;
		for (pixelY = 0; pixelY < 480; pixelY++) {
			for (pixelX = 0; pixelX < 640; pixelX++) {
				tempMat.at<double>(pixelX,pixelY) = Lib.GetTemp(pixelX, pixelY);
			}
		}
		return tempMat;
		//
		

	}
	void classERIS::paintMat(cv::Mat matPicture) {
		
		cv::normalize(matPicture, matPicture, 0, 255, cv::NORM_MINMAX, CV_8U);

		// convert the matrix to a QImage
		QImage img(matPicture.data, matPicture.cols, matPicture.rows, matPicture.step, QImage::Format_Grayscale8);

		// create a QPixmap from the QImage
		QPixmap pixmap = QPixmap::fromImage(img);
		//Richtig Rotiert? ggf. andere Spiegelungen
		QPixmap rotated = pixmap.transformed(QTransform().rotate(90));

		uiERIS.label_Bild->setPixmap(rotated.scaled(rotated.width(), rotated.height(), Qt::KeepAspectRatio));
		

	}
	void classERIS::paintMatError(cv::Mat matPicture) {

		cv::normalize(matPicture, matPicture, 0, 255, cv::NORM_MINMAX, CV_8U);
		QImage img(matPicture.data, matPicture.cols, matPicture.rows, matPicture.step, QImage::Format_Grayscale8);
		QPixmap pixmap = QPixmap::fromImage(img);
		QPixmap rotated = pixmap.transformed(QTransform().rotate(90));
		uiERIS.label_Fehlstelle->setPixmap(rotated.scaled(rotated.width(),rotated.height(),Qt::KeepAspectRatio));

	}
	void classERIS::changeFrameNumber()
	{
		std::cout << "Change Frame Number to \t" << actFN << std::endl;
		Lib.SelectFrame(actFN);
		std::cout << "Changed Frame" << std::endl;
		ActualizePixelValues(&edtTemp, &edtTempBlackbody);
		std::cout << "Write Matrix " << actFN << std::endl;
		paintMat(writeTempToMat());
		

		std::cout << "Matrix Done " << actFN << std::endl;
	}
}


