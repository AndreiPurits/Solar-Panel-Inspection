#ifdef __cplusplus
extern "C" {
#endif

#ifndef ldirbgrab
#define ldirbgrab
#ifndef __BORLANDC__
  typedef int   THandle; // 32 bit
#endif
  typedef unsigned short WORDBOOL;
  typedef unsigned char  BYTE;

typedef THandle	( __stdcall *tfpInitgrabber)( const THandle aHandle, const char* strFullIniFilename, 
									  const char* strIniSection );
typedef int (_stdcall *tfpGetimgwidth)( const THandle hGrabber );
typedef int (_stdcall *tfpGetimgheight)( const THandle hGrabber );
typedef int (_stdcall *tfpGrabpicture)( const THandle hGrabber, void* pbuffer, const int nRingPufferPos );
typedef int (_stdcall *tfpGrabvalues)( const THandle hGrabber, void* pbuffer, const int nRingPufferPos );
typedef int (_stdcall *tfpGrabframe)( const THandle hGrabber, WORD* pbuffer, 
			     const int nRingPufferPos, 
                 WORDBOOL &bHeaderChanged );
typedef int (_stdcall *tfpGetcalib)( const THandle hGrabber, WORD* pbuffer, 
			      WORDBOOL &bHeaderChanged );
typedef int (_stdcall *tfpGetlut)( const THandle hGrabber, BYTE* pbuffer);
  
typedef WORDBOOL (_stdcall *tfpStartgrabber)( const THandle hGrabber );
typedef  WORDBOOL (_stdcall *tfpStopgrabber)( const THandle hGrabber );
typedef  WORDBOOL (_stdcall *tfpClosegrabber)( const THandle hGrabber );
typedef  void (_stdcall *tfpReleasegrabber)( const THandle hGrabber );
// char strAnswer[MaxAnswerLength];
typedef  WORDBOOL (_stdcall *tfpSendcommand)( const THandle hGrabber, 
				 const char* strCommand, void* strAnswer,
                                 int nTimeout );
typedef  THandle  (_stdcall *tfpGethwnd)();


#define MaxAnswerLength 100


#endif  /* ldirbgrab */

#ifdef __cplusplus
}
#endif	/* C++ */

