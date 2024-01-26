#ifdef __cplusplus
#include <iostream>
extern "C" {
#endif

#include <stdint.h>
#include "hGlobal.h"
#include "hGraphics.h"


#ifndef __LDIRBACS__
#define __HIRBACS__
#ifndef __BORLANDC__
	typedef HINSTANCE   tHandle; // 32 bit

	typedef unsigned int	uint;	//In Visual Studio always interpreted as 4 bytes. Size of "int" can depend on system architecture! See also: https://msdn.microsoft.com/en-gb/library/s3f49ktz.aspx
	
#endif
	typedef double			TDateTime;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Typedefs of specific structures and enumerations
	//////////////////////////////////////////////////////////////////////////////////////////////////
	enum { eTempBlackbodyDouble = 0, eTempCorrected, eDigitalValue, eTempBlackbodySingle };

	enum {
		eParaWidth = 0,
		eParaHeight,
		eParaTempAv,
		eParaSpan,
		eParaEmission,
		eParaDistance,	//5
		eParaTempPath,
		eParaTempEnv,
		eParaAbsorption,
		eParaIRBVersion,
		eParaZoom,		//10
		eParaCameraName,
		eParaCameraSN,
		eParaObejectiveName,
		eParaPixelFormat,
		eParaTransmission,	//15
		eParaLambda,
		eParaLambdaDelta,
		eParaTimestampMilli,
		eParaTimestampAbs,
		eParaCalLower,		//20
		eParaCalUpper,
		eParaTriggerState,
		eParaIntegrationTime,
		eParaHFOV,
		eParaVFOV,			//25
		eParaSmooth,
		eParaTempCam,
		eParaTempSens,
	};


	#pragma pack(push,1)
	typedef struct
	{
		uint16_t Year;
		uint16_t Month;
		uint16_t DayOfWeek;
		uint16_t Day;
		uint16_t Hour;
		uint16_t Minute;
		uint16_t Second;
		uint16_t MilliSecond;
	}TSystemTime;
	#pragma(pop)
	 
	#pragma pack(push,1)
	typedef struct 
	{
		uint16_t pixelFormat;
		uint16_t compression;
		uint16_t imgWidth;
		uint16_t imgHeight;
		uint16_t upperLeftX;
		uint16_t upperLeftY;
		uint16_t firstValidX;
		uint16_t lastValidX;
		uint16_t firstValidY;
		uint16_t lastValidY;
		float	 position;
	}TIRBgeomInfo;
	#pragma pack(pop)

	#pragma pack(push,1)
	typedef struct
	{
		float	 emissivity;
		float	 objDistance;
		float	 ambTemp;
		float	 absoConst;
		float	 pathTemp;
		int32_t	 version;
	}TIRBobjPars;
	#pragma pack(pop)

	#pragma pack(push,1)
	typedef struct
	{
		uint8_t	 cbData[1568];
	}TIRBCalibPars1;
	#pragma pack(pop)

	#pragma pack(push,1)
	typedef struct
	{
		float	 level;
		float	 span;
		TDateTime imgTime;
		float	 imgMilliTime;     // Millisekunden 
		uint16_t imgAccu;
		char	 imageComment[80];
		float	 zoom_hor;
		float	 zoom_vert;
		int16_t	 imgMilliTimeEx;  // evtl. Mikrosekunden 
	}TIRBImageInfo;
	#pragma pack(pop)

#pragma pack(push,1)
	typedef struct
	{
		TIRBgeomInfo	geomInfo;
		TIRBobjPars		objectPars;
		TIRBCalibPars1	calibPars;
		TIRBImageInfo	imgInfo;
	}TIRBImageData1;
#pragma pack(pop)


	typedef TIRBImageData1 *PIRBImageData1;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Typedefs for Lib-Functions
	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef uint	*(_stdcall *tfpLoadIRB)						(const char* fn);
	typedef void	(_stdcall *tfpUnloadIRB)					(const uint *aHandle);
	typedef bool	(_stdcall *tfpVersion)						(int32_t* MainVersion, int32_t* SubVersion);

	typedef int32_t	(_stdcall *tfpGetFrameCount)				(const uint *aHandle);
	typedef int32_t	(_stdcall *tfpgetIRBIndices)				(const uint *aHandle, const int32_t *irbIdxList);

	typedef int32_t	(_stdcall *tfpGetFrameNumber)				(const uint *aHandle);
	typedef void	(_stdcall *tfpSetFrameNumber)				(const uint *aHandle, const int32_t FrameNumber);
	typedef int32_t	(_stdcall *tfpGetFrameNumberByArrayIndex)	(const uint *aHandle);
	typedef void	(_stdcall *tfpSetFrameNumberByArrayIndex)	(const uint *aHandle, const int32_t FrameNumber);

	typedef double	(_stdcall *tfpGetTempBlackbodyOfXY)			(const uint *aHandle, const int32_t xx, const int32_t yy);
	typedef double	(_stdcall *tfpGetTempOfXY)					(const uint *aHandle, const int32_t xx, const int32_t yy);
	typedef double	(_stdcall *tfpGetDigitalValuesOfXY)			(const uint *aHandle, const int32_t xx, const int32_t yy);
	typedef int32_t (_stdcall *tfpReadPixelData)				(const uint *aHandle, const void *pData, const int32_t what);

	typedef int32_t (_stdcall *tfpReadIRBData)					(const uint *aHandle, const PIRBImageData1 pIRBFrame);
	typedef int32_t (_stdcall *tfpReadIRBDataUncompressed)		(const uint *aHandle, const unsigned char* pIRBFrame);

	typedef bool	(_stdcall *tfpgetFrameTimeStamp)			(const uint *aHandle, TSystemTime *timestamp);

	typedef bool	(_stdcall *tfpgetParam)						(const uint *aHandle, const int what, double *value);
	typedef bool	(_stdcall *tfpgetParamS)					(const uint *aHandle, const int what, char	*value);

	


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Class-Definition for access IRB-data via Library
	//////////////////////////////////////////////////////////////////////////////////////////////////

	class oLibHandler
	{
	private:
		//Addresses of Lib and IRB-File
		tHandle				hLib;
		uint				*hIRBFile;
		//Declaration of Lib-functions
		tfpVersion			fVersion;

		tfpLoadIRB			fLoadIRBFile;
		tfpUnloadIRB		fUnloadIRBFile;

		tfpGetFrameCount	fGetFrameCount;
		tfpgetIRBIndices	fGetIRBIndices;

		tfpGetFrameNumber	fgetFrameNumber;
		tfpSetFrameNumber	fSetFrameNumber;
		tfpGetFrameNumberByArrayIndex fGetFrameNumberByArrayIndex;
		tfpSetFrameNumberByArrayIndex fSetFrameNumberByArrayIndex;

		tfpGetTempBlackbodyOfXY fGetTempBlackbodyOfXY;
		tfpGetTempOfXY			fGetTempOfXY;
		tfpGetDigitalValuesOfXY	fGetDigitalValuesOfXY;
		tfpReadPixelData		fReadPixelData;
		tfpReadIRBData			fReadIRBData;
		tfpReadIRBDataUncompressed fReadIRBDataUncompressed;

		tfpgetFrameTimeStamp	fgetFrameTimeStamp;
		tfpgetParam				fGetParam;
		tfpgetParamS			fGetParamS;

		// Private/Local Information of IRB-file
		int	FrameCount;
		int *FrameIndexes;
		int LastFrameIndex = -1;

		TIRBImageData1 *pIRBImageData;
		void *ImageDataBuffer;
		//Neu in Public
		unsigned char *PixelData;

		float TempMin;
		float TempMax;

		oGraphicControl *GraphicController;

		void freeAndNullPtr(void **pointer);

	public:
		//unsigned char* PixelData;
		//////////////////////////////////////
		//Properties
		bool	IsLibLoaded()	{ return (hLib != NULL); };
		bool	IsIRBLoaded()	{ return (hIRBFile != NULL); };
		uint16_t GetWidth()
		{

			if (hIRBFile != NULL)				
				{				
				return pIRBImageData->geomInfo.imgWidth;}
			else
				{				
				return 0;}
		}
		uint16_t GetHeight()
		{
			if (hIRBFile != NULL)
				{
				std::cout << "not Zero" << std::endl;
				return pIRBImageData->geomInfo.imgHeight;}
			else
				{
				std::cout << "Zero" << std::endl;
				return 0;}
		}


		bool	InitFunctions(void);
		//Load/Unload Lib for accessing the library
		bool	LoadLib(const char* fn);
		void	UnloadLib(void);
		//external Functions of the library access
		void	VersionLib(void);
		bool	LoadIRB(const char* fn, oGraphicControl *GraphicCtrl);
		void	UnloadIRB(void);

		int		GetFrameCount(void);

		double	GetParameter(int what);
		void	GetParameterS(int what, char* value);

		void InitFrameInfo(void);

		void GetParameterString(int ParameterIndex, char *Output);
		bool SelectFrame(int FrameNumber);
		int ReadPixelData(void *PixelData, int PixelType);

		bool ReadIRBDataUncompressed(void);
		//NEU
		void DrawPictureMonochrome(unsigned char *StartAdress);
		//Bitmap DrawPictureMonochrome(unsigned char* StartAdress);
		void DeInit(void);

		double GetTempBlackBody(int x, int y);
		double GetTemp(int x, int y);

	protected:
	};




#endif  /* __LDIRBACS__ */

#ifdef __cplusplus
}
#endif	/* C++ */
