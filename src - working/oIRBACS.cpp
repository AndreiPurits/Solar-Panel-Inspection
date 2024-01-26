#include "StdAfx.h"
#include <Windows.h>
#include <string>
#include "fstream"
#include "stdio.h"

#include "hGlobal.h"
#include "hIRBACS.h"
#include "hGraphics.h"


#include <iostream>

using namespace std;







bool oLibHandler::LoadLib(const char* fn)
{
	return ((hLib = LoadLibrary(fn)) != NULL);
}

void oLibHandler::UnloadLib(void)
{
	FreeLibrary(hLib);
}

void oLibHandler::VersionLib(void)
{
	int MainVer;
	int SubVer;
	bool success;

	success = fVersion(&MainVer, &SubVer);
}


bool oLibHandler::LoadIRB(const char* fn, oGraphicControl *GraphicCtrl)
{
	if ((hIRBFile = fLoadIRBFile(fn)) != NULL)
	{
		GraphicController = GraphicCtrl;
		return true;
	}
	else
	{
		return false;
	}
}

void oLibHandler::UnloadIRB(void)
{
	if (hIRBFile == NULL) {return; }
	fUnloadIRBFile(hIRBFile);
}

bool oLibHandler::InitFunctions(void)
{
	bool error = FALSE;

	if (hLib == NULL) { return true;}

//	if ((				= ()				GetProcAddress(hLib, ""))				== NULL) { error = TRUE; }
	if ((fVersion		= (tfpVersion)		GetProcAddress(hLib, "version"))		== NULL) { error = TRUE; }
	if ((fLoadIRBFile	= (tfpLoadIRB)		GetProcAddress(hLib, "loadIRB"))		== NULL) { error = TRUE; }
	if ((fUnloadIRBFile = (tfpUnloadIRB)	GetProcAddress(hLib, "unloadIRB"))		== NULL) { error = TRUE; }
	if ((fGetFrameCount = (tfpGetFrameCount)GetProcAddress(hLib, "getFrameCount"))	== NULL) { error = TRUE; }
	if ((fGetIRBIndices = (tfpgetIRBIndices)GetProcAddress(hLib, "getIRBIndices"))	== NULL) { error = TRUE; }

	if ((fgetFrameNumber				= (tfpGetFrameNumber)				GetProcAddress(hLib, "getFrameNumber"))			== NULL) { error = TRUE; }
	if ((fSetFrameNumber				= (tfpSetFrameNumber)				GetProcAddress(hLib, "setFrameNumber"))			== NULL) { error = TRUE; }
	if ((fGetFrameNumberByArrayIndex	= (tfpGetFrameNumberByArrayIndex)	GetProcAddress(hLib, "getFrameNbByArrayIdx"))	== NULL) { error = TRUE; }
	if ((fSetFrameNumberByArrayIndex	= (tfpSetFrameNumberByArrayIndex)	GetProcAddress(hLib, "setFrameNbByArrayIdx"))	== NULL) { error = TRUE; }
	if ((fGetTempBlackbodyOfXY			= (tfpGetTempBlackbodyOfXY)			GetProcAddress(hLib, "getTempBBXY"))			== NULL) { error = TRUE; }
	if ((fGetTempOfXY					= (tfpGetTempOfXY)					GetProcAddress(hLib, "getTempXY"))				== NULL) { error = TRUE; }
	if ((fGetDigitalValuesOfXY			= (tfpGetDigitalValuesOfXY)			GetProcAddress(hLib, "getDigValXY"))			== NULL) { error = TRUE; }
	if ((fReadPixelData					= (tfpReadPixelData)				GetProcAddress(hLib, "readPixelData"))			== NULL) { error = TRUE; }

	if ((fReadIRBData					= (tfpReadIRBData)					GetProcAddress(hLib, "readIRBData"))			== NULL) { error = TRUE; }
	if ((fReadIRBDataUncompressed		= (tfpReadIRBDataUncompressed)		GetProcAddress(hLib, "readIRBDataUncompressed")) == NULL) { error = TRUE; }


	if ((fGetParam						= (tfpgetParam)						GetProcAddress(hLib, "getParam"))				== NULL) { error = TRUE; }
	if ((fGetParamS						= (tfpgetParamS)					GetProcAddress(hLib, "getParamS"))				== NULL) { error = TRUE; }
	if ((fgetFrameTimeStamp				= (tfpgetFrameTimeStamp)			GetProcAddress(hLib, "getFrameTimeStamp"))		== NULL) { error = TRUE; }
	
	return error;
}

int oLibHandler::GetFrameCount(void)
{
	int wert;

	if (hIRBFile == NULL)
	{
		FrameCount = 0;
	}
	else
	{
		FrameCount = fGetFrameCount(hIRBFile);
		FrameIndexes = new int[FrameCount];

		wert = fGetIRBIndices(hIRBFile, FrameIndexes);

		//SelectFrame(0);
		
	}
	
	return FrameCount;
}

double	oLibHandler::GetParameter(int what)
{
	bool success;
	double value;
	
	if (hIRBFile == NULL)
	{
		value = 0;
	}
	else
	{
		success = fGetParam(hIRBFile, what, &value);
	}

	return value;
}

void oLibHandler::GetParameterS(int what, char* value)
{
	if (hIRBFile == NULL)
	{
		value = 0;
	}
	else
	{
		fGetParamS(hIRBFile, what, value);
	}

}

void oLibHandler::InitFrameInfo(void)
{
	//
}


void oLibHandler::GetParameterString(int ParameterIndex, char *Output)
{
	std::cout << "Get Param String" << std::endl;
	double value;
	void *pValue;
	TDataTypes datatype;
	int precision;
	char *unit;
	std::cout << "Define Variables" << std::endl;
	if (hLib == NULL) { return; }
	
	std::cout << "Start Switch" << std::endl;
	//if pValue is initiated, the parameters will be read from the header directly, else by the GetParamFunction from the dll
	switch (ParameterIndex)
	{
		std::cout << "In Switch" << std::endl;
	case eParaWidth:			{datatype = tUint16;		precision = 0; unit = "";	pValue = &pIRBImageData->geomInfo.imgWidth;		break; }
	case eParaHeight:			{datatype = tUint16;		precision = 0; unit = "";	pValue = &pIRBImageData->geomInfo.imgHeight;	break; }
	case eParaTempAv:			{datatype = tfloat;			precision = 0; unit = "K";	pValue = &pIRBImageData->imgInfo.level;			break; }
	case eParaSpan:				{datatype = tfloat;			precision = 0; unit = "";	pValue = &pIRBImageData->imgInfo.span;			break; }
	case eParaEmission:			{datatype = tfloat;			precision = 3; unit = "";	pValue = &pIRBImageData->objectPars.emissivity;	break; }
	case eParaDistance:			{datatype = tfloat;			precision = 2; unit = "m";	pValue = &pIRBImageData->objectPars.objDistance;break; }
	case eParaTempPath:			{datatype = tfloat;			precision = 2; unit = "K";	pValue = &pIRBImageData->objectPars.pathTemp;	break; }
	case eParaTempEnv:			{datatype = tfloat;			precision = 2; unit = "K";	pValue = &pIRBImageData->objectPars.ambTemp;	break; }
	case eParaAbsorption:		{datatype = tfloat;			precision = 3; unit = "";	pValue = &pIRBImageData->objectPars.absoConst;	break; }
	case eParaIRBVersion:		{datatype = tUint32;		precision = 0; unit = "";	pValue = &pIRBImageData->objectPars.version;	break; }
	case eParaZoom:				{datatype = tfloat;			precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaCameraName:		{datatype = tpChar;			precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaCameraSN:			{datatype = tpChar;			precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaObejectiveName:	{datatype = tpChar;			precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaPixelFormat:		{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaTransmission:		{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaLambda:			{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaLambdaDelta:		{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaTimestampMilli:	{datatype = tDouble;		precision = 2; unit = "";	pValue = NULL;			break; }
	case eParaTimestampAbs:		{datatype = tpTimestamp;	precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaCalLower:			{datatype = tDouble;		precision = 2; unit = "K";	pValue = NULL;			break; }
	case eParaCalUpper:			{datatype = tDouble;		precision = 2; unit = "K";	pValue = NULL;			break; }
	case eParaTriggerState:		{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaIntegrationTime:	{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaHFOV:				{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaVFOV:				{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaSmooth:			{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaTempCam:			{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	case eParaTempSens:			{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	default:					{datatype = tDouble;		precision = 0; unit = "";	pValue = NULL;			break; }
	}



	switch (datatype)
	{
		case tInt16:
		{
			int16_t *pInt16 = (int16_t*)pValue;
			
			if (pValue == NULL)
			{ 
				value = GetParameter(ParameterIndex);
				FormatDoubleToString(value, precision, unit, Output);
			}
			else
			{
				FormatIntToString(*pInt16, unit, Output);
			}
			break;
		}
		case tUint16:
		{
			uint16_t *pUint16 = (uint16_t*)pValue;

			if (pValue == NULL)
			{
				value = GetParameter(ParameterIndex);
				FormatDoubleToString(value, precision, unit, Output);
			}
			else
			{
				FormatIntToString(*pUint16, unit, Output);
			}
			break;
		}
		case tUint32:
		{
			uint32_t *pUint32 = (uint32_t*)pValue;

			if (pValue == NULL)
			{
				value = GetParameter(ParameterIndex);
				FormatDoubleToString(value, precision, unit, Output);
			}
			else
			{
				FormatIntToString(*pUint32, unit, Output);
			}
			break;
		}
		case tfloat:
		{
			float *pFloat = (float*)pValue;

			if (pValue == NULL)
				{value = GetParameter(ParameterIndex);}
			else  
				{value = (double)*pFloat; }
			FormatDoubleToString(value, precision, unit, Output);
			break;
		}
		case tDouble:
		{
			double * pdblvalue = (double*)pValue;

			if (pValue == NULL)
				{value = GetParameter(ParameterIndex);}
			else
				{value = (double)*pdblvalue;}
			FormatDoubleToString(value, precision, unit, Output);
			break;
		}
		case tpChar:
		{
			GetParameterS(ParameterIndex, Output);
			
			break;
		}
		case tpTimestamp:
		{
			TSystemTime Systemtime;
			bool result;

			result = fgetFrameTimeStamp(hIRBFile, &Systemtime);
			sprintf(Output, "%02i.%02i.%04i %02i:%02i:%02i,%03i", Systemtime.Day, Systemtime.Month, Systemtime.Year,   Systemtime.Hour, Systemtime.Minute, Systemtime.Second, Systemtime.MilliSecond);
			break;
		}

	}
}

bool oLibHandler::SelectFrame(int FrameNumber)
{
	
	uint16_t value;
	
	//uint8_t ImageDataBuffer[200000];
	std::cout << "If Conditions " << std::endl;
	if (hLib == NULL) { return false; }
	if (LastFrameIndex == FrameNumber) { return false; }
	std::cout << "Passed " << std::endl;
	fSetFrameNumberByArrayIndex(hIRBFile, FrameNumber);	//Select Active Frame
	std::cout << "SetFrameNumberByArray " << std::endl;
	value = fGetFrameNumberByArrayIndex(hIRBFile);	//Check if frame selected
	std::cout << "GetFrameNumberByArray " << std::endl;
	if (value == FrameNumber)
	{
		LastFrameIndex = FrameNumber;
		std::cout << "LastFrameIndex \t" <<FrameNumber<< std::endl;
		return ReadIRBDataUncompressed();
		std::cout << "Return" << std::endl;
	}
	else
	{
		std::cout << "Else" << std::endl;
		return false;
	}
}

// Free and NULL Pointer
void oLibHandler::freeAndNullPtr(void **pointer)
{
	if (*pointer != NULL)
	{
		free(*pointer);
		*pointer = NULL;
	}
}


bool oLibHandler::ReadIRBDataUncompressed(void)
{
	int Length;
	std::cout << "start Reading" << std::endl;
	if (hIRBFile == NULL) { 
		std::cout << "hIRBFile == NULL" << std::endl;
		return false; }
	

	Length = fReadIRBDataUncompressed(hIRBFile, NULL);
	//std::cout << "Length= \t"<< Length << std::endl;
	if (Length <= 0) return false;

	//FreeAndNull(ImageDataBuffer);	//Get free if still file is loaded
	//free(ImageDataBuffer);
	//ImageDataBuffer = NULL;
	std::cout << "freeAndNullPtr" << std::endl;
	freeAndNullPtr((void**)&ImageDataBuffer);
	std::cout << "molloc" << std::endl;
	ImageDataBuffer = malloc(Length);

	pIRBImageData	= (TIRBImageData1*)ImageDataBuffer;
	PixelData		= (unsigned char*)pIRBImageData + sizeof(TIRBImageData1);

	Length = fReadIRBDataUncompressed(hIRBFile, (unsigned char*)pIRBImageData);
	std::cout << "DrawPicture" << std::endl;
	//DrawPictureMonochrome(PixelData);// raus da nur Frame gesetzt werden soll
	std::cout << "reading done" << std::endl;
	return true;
}

void oLibHandler::DrawPictureMonochrome(unsigned char* StartAdress)
{
	std::cout << "start Draw Monchrome" << std::endl;
	uint16_t* aPW;
	uint16_t* aStart;
	unsigned int pY;

	uint16_t min = 0xFFFF;
	uint16_t max = 0;
	uint16_t val;
	uint8_t bb;

	float clrWR;

	RECT lblDim;
	int ww, hh;
	float stretchX, stretchY;
	int xx, yy;

	if ((StartAdress == NULL) || (GraphicController == NULL)) { 
		std::cout << "Start Address or GraphicController =0" << std::endl; return; }
	//Fehler https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclientrect
	/*
	std::cout << "Get Client Rect" << std::endl;//Fehler
	GraphicController->myControl->GetClientRect(&lblDim);
	std::cout << "Got Client Rect" << std::endl;
	*/
	//Koordinaten anders festlegen obere Linke Ecke 0,0
	ww = 640 - 0;
	hh = 480 - 0;
	stretchX = float(ww) / float(pIRBImageData->geomInfo.imgWidth);
	stretchY = float(hh) / float(pIRBImageData->geomInfo.imgHeight);

	Bitmap bmp(ww, hh, PixelFormat32bppRGB);

	aStart = (uint16_t*)StartAdress;
	aPW = aStart;

	yy = pIRBImageData->geomInfo.imgWidth * pIRBImageData->geomInfo.imgHeight;

	while (yy > 0)
	{
		val = *aPW;
		if (val < min) min = val;
		if (val > max) max = val;
		aPW++;
		yy--;
	}

	clrWR = 255.0f / (max - min);

	yy = hh - 1;
	while (yy >= 0)
	{
		pY = (unsigned int)(yy / stretchY) * pIRBImageData->geomInfo.imgWidth;
		xx = ww - 1;
		while (xx >= 0)
		{
			aPW = (uint16_t*)((unsigned int)(xx / stretchX) + pY + aStart);

			bb = (uint8_t)((*aPW - min) * clrWR);
			bmp.SetPixel(xx, yy, RGB(bb, bb, bb));
			xx--;
		}
		yy--;
	}
	std::cout << "start Drawing Bitmap" << std::endl;
	GraphicController->DrawingBitmap(&bmp);
	std::cout << "done Drawing Bitmap" << std::endl;
}
/*
Bitmap oLibHandler::DrawPictureMonochrome(unsigned char* StartAdress)
{
	
	uint16_t *aPW;
	uint16_t *aStart;
	unsigned int pY;

	uint16_t min = 0xFFFF;
	uint16_t max = 0;
	uint16_t val;
	uint8_t bb;

	float clrWR;

	RECT lblDim;
	int ww, hh;
	float stretchX, stretchY;
	int xx, yy;

	if ((StartAdress == NULL) || (GraphicController == NULL)) { return; }

	GraphicController->myControl->GetClientRect(&lblDim);
	ww = lblDim.right - lblDim.left;
	hh = lblDim.bottom - lblDim.top;
	stretchX = float(ww) / float(pIRBImageData->geomInfo.imgWidth);
	stretchY = float(hh) / float(pIRBImageData->geomInfo.imgHeight);

	Bitmap bmp(ww, hh, PixelFormat32bppRGB);

	aStart = (uint16_t *)StartAdress;
	aPW = aStart;

	yy = pIRBImageData->geomInfo.imgWidth * pIRBImageData->geomInfo.imgHeight;

	while (yy > 0)
	{
		val = *aPW;
		if (val < min) min = val;
		if (val > max) max = val;
		aPW++;
		yy--;
	}

	clrWR = 255.0f / (max - min);
	
	yy = hh - 1;
	while (yy >= 0)
	{
		pY = (unsigned int)(yy / stretchY) * pIRBImageData->geomInfo.imgWidth;
		xx = ww - 1;
		while (xx >= 0)
		{
			aPW = (uint16_t*)((unsigned int)(xx / stretchX) + pY + aStart);

			bb = (uint8_t)((*aPW - min) * clrWR);
			bmp.SetPixel(xx, yy, RGB(bb, bb, bb));
			xx--;
		}
		yy--;
	}
	
	return bmp;
}*/

//Destruction (FreeAndNull) of *PixelData has to take place external
int oLibHandler::ReadPixelData(void *PixelData, int PixelType)
{
	int len;
	
	if (hIRBFile == NULL) { return 0; }

	len = fReadPixelData(hIRBFile, NULL, PixelType);

	if (len > 0)
	{
		switch (PixelType)
		{
		case eTempBlackbodyDouble: { break; }
		case eTempCorrected: {len += 240;  break; }
		case eDigitalValue: {break; }
		case eTempBlackbodySingle: {break; }
		default: {return false; }
		}

		PixelData = malloc(len);
		len = fReadPixelData(hIRBFile, PixelData, PixelType);
	}
	return len;

}

double oLibHandler::GetTempBlackBody(int x, int y)
{
	if (hIRBFile == NULL)
	{
		return 0.0;
	}
	else
	{
		return fGetTempBlackbodyOfXY(hIRBFile, x, y);
	}
}

double oLibHandler::GetTemp(int x, int y)
{
	if (hIRBFile == NULL)
	{
		return 0.0;
	}
	else
	{
		return fGetTempOfXY(hIRBFile, x, y);
	}

}

void oLibHandler::DeInit(void)
{

	UnloadIRB();
	UnloadLib();
	//Addresses of Lib and IRB-File
	hLib = NULL;
	hIRBFile = NULL;
	//Declaration of Lib-functions
	fVersion = NULL;
	fLoadIRBFile = NULL;
	fUnloadIRBFile = NULL;
	fGetFrameCount = NULL;
	fGetIRBIndices = NULL;
	fgetFrameNumber = NULL;
	fSetFrameNumber = NULL;
	fGetFrameNumberByArrayIndex = NULL;
	fSetFrameNumberByArrayIndex = NULL;
	fGetTempBlackbodyOfXY = NULL;
	fGetTempOfXY = NULL;
	fGetDigitalValuesOfXY = NULL;
	fReadPixelData = NULL;
	fReadIRBData = NULL;
	fReadIRBDataUncompressed = NULL;
	fgetFrameTimeStamp = NULL;
	fGetParam = NULL;
	fGetParamS = NULL;

	// Private/Local Information of IRB-file
	FrameCount = -1;
	free(FrameIndexes);
	FrameIndexes = NULL;
	LastFrameIndex = -1;

	//TIRBImageData1 IRBImageData;
	pIRBImageData = NULL;
	//FreeAndNull(ImageDataBuffer);
	free(ImageDataBuffer);
	ImageDataBuffer = NULL;
	PixelData = NULL;
	if (GraphicController != NULL) { GraphicController->ClearGraphic(); }

}



