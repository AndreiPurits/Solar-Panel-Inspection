// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

//Neu
//#define _WIN32_WINNT_MAXVER     0x0601


#if !defined(AFX_STDAFX_H__79E63136_6AC5_4B2D_A411_C027D5C3EFB5__INCLUDED_)
#define AFX_STDAFX_H__79E63136_6AC5_4B2D_A411_C027D5C3EFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

//neu
//extern "C" {}
//#include <Windows.h>
//#include <SDKDDKVer.h>
//#include <afxwin.h> // MFC-Kern- und -Standardkomponenten

//extern "C++" {
#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT
//}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__79E63136_6AC5_4B2D_A411_C027D5C3EFB5__INCLUDED_)
