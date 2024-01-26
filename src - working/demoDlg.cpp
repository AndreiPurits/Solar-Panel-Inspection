// demoDlg.cpp : Implementierungsdatei
//

#include <stdio.h>
#include <direct.h>
#include <winbase.h>


#include "stdafx.h"

#include "hGlobal.h"	//has to included before demo.h
#include "hGraphics.h"

#include "demo.h"
#include "demoDlg.h"

//Neu
#include <iostream>
#include <fstream>
#include <string>
//#include <windows.h>





//#include <id2>
/*#include <Windows.h>
#include <ObjIdl.h>
#include "gdiplus.h"	//Needs windows.h
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")	//if not included causes Linker-Error*/




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


oLibHandler Lib;
oGraphicControl MyGraphic;

CEdit	*EditLink[PARAM_COUNT];
bool editchangelock = false;
int coordX;
int coordY;



int ChangeNumberEditWithInt(CEdit *Edit, int Delta, int LowerLimit, int UpperLimit);
void ActualizeFrameParameters(int FrameIndex);
void ActualizePixelValues(CEdit *edtTemp, CEdit* edtTempBB);


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg Dialogfeld


CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CDemoDlg)
		// HINWEIS: Der Klassenassistent fügt hier Member-Initialisierung ein
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BtnLoadLib, btnLoadLib);
	DDX_Control(pDX, IDC_EDIT_COORDX, edtCoordX);
	DDX_Control(pDX, IDC_EDIT_COORDY, edtCoordY);
	DDX_Control(pDX, IDC_SPIN_CORDX, spnCoordX);
	DDX_Control(pDX, IDC_SPIN_COORDY, spnCoordY);
	DDX_Control(pDX, IDC_SPIN_FRAMEINDEX, spnFrameIndex);
	DDX_Control(pDX, IDC_EDIT_FRAMEINDEX, edtFrameIndex);
	DDX_Control(pDX, IDC_EDIT_FRAMECOUNTS, edtFrameCount);
	DDX_Control(pDX, IDC_EDIT_FILENAME, edtFilename);
	DDX_Control(pDX, IDC_EDIT_PARA0, edtParam0);
	DDX_Control(pDX, IDC_EDIT_PARA1, edtParam1);
	DDX_Control(pDX, IDC_EDIT_PARA2, edtParam2);
	DDX_Control(pDX, IDC_EDIT_PARA3, edtParam3);
	DDX_Control(pDX, IDC_EDIT_PARA4, edtParam4);
	DDX_Control(pDX, IDC_EDIT_PARA5, edtParam5);
	DDX_Control(pDX, IDC_EDIT_PARA6, edtParam6);
	DDX_Control(pDX, IDC_EDIT_PARA7, edtParam7);
	DDX_Control(pDX, IDC_EDIT_PARA8, edtParam8);
	DDX_Control(pDX, IDC_EDIT_PARA9, edtParam9);
	DDX_Control(pDX, IDC_EDIT_PARA10, edtParam10);
	DDX_Control(pDX, IDC_EDIT_PARA11, edtParam11);
	DDX_Control(pDX, IDC_EDIT_PARA12, edtParam12);
	DDX_Control(pDX, IDC_EDIT_PARA13, edtParam13);
	DDX_Control(pDX, IDC_EDIT_PARA14, edtParam14);
	DDX_Control(pDX, IDC_STATIC_LABELImage, lblImage2);
	DDX_Control(pDX, IDC_LBL_PARA0, lblParam0);
	DDX_Control(pDX, IDC_LBL_PARA1, lblParam1);
	DDX_Control(pDX, IDC_LBL_PARA2, lblParam2);
	DDX_Control(pDX, IDC_LBL_PARA3, lblParam3);
	DDX_Control(pDX, IDC_LBL_PARA4, lblParam4);
	DDX_Control(pDX, IDC_LBL_PARA5, lblParam5);
	DDX_Control(pDX, IDC_LBL_PARA6, lblParam6);
	DDX_Control(pDX, IDC_LBL_PARA7, lblParam7);
	DDX_Control(pDX, IDC_LBL_PARA8, lblParam8);
	DDX_Control(pDX, IDC_LBL_PARA9, lblParam9);
	DDX_Control(pDX, IDC_LBL_PARA10, lblParam10);
	DDX_Control(pDX, IDC_LBL_PARA11, lblParam11);
	DDX_Control(pDX, IDC_LBL_PARA12, lblParam12);
	DDX_Control(pDX, IDC_LBL_PARA13, lblParam13);
	DDX_Control(pDX, IDC_LBL_PARA14, lblParam14);
	DDX_Control(pDX, IDC_STATIC_FRAMEINDEX2, lblFrameCounts);
	DDX_Control(pDX, IDC_STATIC_FRAMEINDEX, lblFrameIndex);
	DDX_Control(pDX, IDC_STATIC_COORDX, lblCoordX);
	DDX_Control(pDX, IDC_STATIC_COORDY, lblCoordY);
	DDX_Control(pDX, IDC_EDTBLACKBODY, edtTempBlackbody);
	DDX_Control(pDX, IDC_EDTTEMP, edtTemp);
	DDX_Control(pDX, IDC_LBLBLACKBODY, lblBlackbody);
	DDX_Control(pDX, IDC_LBLTEMP, lblTemp);
	DDX_Control(pDX, IDC_LBLPATH, lblPath);
	DDX_Control(pDX, IDC_BUTTON4, btnRepaint);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnLoadLib, &CDemoDlg::OnBnClickedBtnloadlib)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CORDX, &CDemoDlg::OnDeltaposSpinCordx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COORDY, &CDemoDlg::OnDeltaposSpinCoordy)
	ON_EN_CHANGE(IDC_EDIT_FRAMEINDEX, &CDemoDlg::OnEnChangeEditFrameindex)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FRAMEINDEX, &CDemoDlg::OnDeltaposSpinFrameindex)
	ON_EN_CHANGE(IDC_EDIT_COORDX, &CDemoDlg::OnEnChangeEditCoordx)
	ON_EN_CHANGE(IDC_EDIT_COORDY, &CDemoDlg::OnEnChangeEditCoordy)
	ON_BN_CLICKED(IDC_BUTTON4, &CDemoDlg::OnBnClickedButton4)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg Nachrichten-Handler

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	bool error = FALSE;
	int ii;

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	

	edtFilename.SetWindowText(_T(".\\PCB.irb"));

	for (ii = 0; ii < PARAM_COUNT; ii++)
	{
		switch (ii)
		{
		case eParaWidth:			{EditLink[ii] = &edtParam0; break; }
		case eParaHeight:			{EditLink[ii] = &edtParam1; break; }
		case eParaTempAv:			{EditLink[ii] = NULL; break; }
		case eParaSpan:				{EditLink[ii] = NULL; break; }
		case eParaEmission:			{EditLink[ii] = &edtParam2; break; }
		case eParaDistance:			{EditLink[ii] = &edtParam5; break; }
		case eParaTempPath:			{EditLink[ii] = &edtParam6; break; }
		case eParaTempEnv:			{EditLink[ii] = &edtParam3; break; }
		case eParaAbsorption:		{EditLink[ii] = &edtParam4; break; }
		case eParaIRBVersion:		{EditLink[ii] = NULL; break; }
		case eParaZoom:				{EditLink[ii] = NULL; break; }
		case eParaCameraName:		{EditLink[ii] = &edtParam10; break; }
		case eParaCameraSN:			{EditLink[ii] = &edtParam11; break; }
		case eParaObejectiveName:	{EditLink[ii] = &edtParam12; break; }
		case eParaPixelFormat:		{EditLink[ii] = NULL; break; }
		case eParaTransmission:		{EditLink[ii] = NULL; break; }
		case eParaLambda:			{EditLink[ii] = NULL; break; }
		case eParaLambdaDelta:		{EditLink[ii] = NULL; break; }
		case eParaTimestampMilli:	{EditLink[ii] = &edtParam8; break; }
		case eParaTimestampAbs:		{EditLink[ii] = &edtParam9; break; }
		case eParaCalLower:			{EditLink[ii] = &edtParam13; break; }
		case eParaCalUpper:			{EditLink[ii] = &edtParam14; break; }
		case eParaTriggerState:		{EditLink[ii] = NULL; break; }
		case eParaIntegrationTime:	{EditLink[ii] = NULL; break; }
		case eParaHFOV:				{EditLink[ii] = NULL; break; }
		case eParaVFOV:				{EditLink[ii] = NULL; break; }
		case eParaSmooth:			{EditLink[ii] = NULL; break; }
		case eParaTempCam:			{EditLink[ii] = NULL; break; }
		case eParaTempSens:			{EditLink[ii] = NULL; break; }
		default:					{EditLink[ii] = NULL; break; }
		}
	}


	//Set positions of Ctrl-Elements
	const int HEIGHTS = 20;
	const int DELTAY = 0;
	const int DELTAX = 0;
	const int STARTPOSY = 10 + 0 * (HEIGHTS + DELTAY);
	

	const int LBL_STARTPOSX = 20;
	const int LBL_WIDTHS = 90;

	const int EDT_STARTPOSX = LBL_STARTPOSX + LBL_WIDTHS + DELTAX;
	const int EDT_WIDTHS = 150;

	const int SPN_WIDTH = 20;

	const int COLUMN_WIDTH = EDT_STARTPOSX + EDT_WIDTHS;

	SetCWndPosition(&edtFilename, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 0 (HEIGHTS + DELTAY), EDT_WIDTHS + COLUMN_WIDTH, HEIGHTS);
	SetCWndPosition(&lblPath, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 0 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	SetCWndPosition(&edtFrameCount, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 2 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblFrameCounts, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 2 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	SetCWndPosition(&spnFrameIndex, 0 * COLUMN_WIDTH + EDT_STARTPOSX + EDT_WIDTHS - SPN_WIDTH, STARTPOSY + 3 * (HEIGHTS + DELTAY), SPN_WIDTH,				HEIGHTS);
	SetCWndPosition(&edtFrameIndex, 0 * COLUMN_WIDTH + EDT_STARTPOSX,							STARTPOSY + 3 * (HEIGHTS + DELTAY), EDT_WIDTHS - SPN_WIDTH, HEIGHTS);
	SetCWndPosition(&lblFrameIndex, 0 * COLUMN_WIDTH + LBL_STARTPOSX,							STARTPOSY + 3 * (HEIGHTS + DELTAY), LBL_WIDTHS,				HEIGHTS);

	SetCWndPosition(&edtParam0, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 5 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam1, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 6 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam2, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 7 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam3, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 8 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam4, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 9 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam5, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 10 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam6, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 11 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam7, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 12 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam8, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 13 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam9, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 14 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam10, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 15 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam11, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 16 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam12, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 17 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam13, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 18 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&edtParam14, 0 * COLUMN_WIDTH + EDT_STARTPOSX, STARTPOSY + 19 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);

	
	SetCWndPosition(&lblParam0, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 5 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam1, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 6 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam2, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 7 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam3, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 8 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam4, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 9 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam5, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 10 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam6, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 11* (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam7, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 12 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam8, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 13 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam9, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 14 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam10, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 15 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam11, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 16 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam12, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 17 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam13, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 18 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblParam14, 0 * COLUMN_WIDTH + LBL_STARTPOSX, STARTPOSY + 19 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	
	SetCWndPosition(&spnCoordX,	1 * COLUMN_WIDTH + EDT_STARTPOSX + EDT_WIDTHS - SPN_WIDTH,	STARTPOSY + 2 * (HEIGHTS + DELTAY), SPN_WIDTH, HEIGHTS);
	SetCWndPosition(&edtCoordX,	1 * COLUMN_WIDTH + EDT_STARTPOSX,							STARTPOSY + 2 * (HEIGHTS + DELTAY), EDT_WIDTHS - SPN_WIDTH, HEIGHTS);
	SetCWndPosition(&lblCoordX, 1 * COLUMN_WIDTH + LBL_STARTPOSX,							STARTPOSY + 2 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);
	SetCWndPosition(&spnCoordY,	1 * COLUMN_WIDTH + EDT_STARTPOSX + EDT_WIDTHS - SPN_WIDTH,	STARTPOSY + 3 * (HEIGHTS + DELTAY), SPN_WIDTH, HEIGHTS);
	SetCWndPosition(&edtCoordY,	1 * COLUMN_WIDTH + EDT_STARTPOSX,							STARTPOSY + 3 * (HEIGHTS + DELTAY), EDT_WIDTHS - SPN_WIDTH, HEIGHTS);
	SetCWndPosition(&lblCoordY, 1 * COLUMN_WIDTH + LBL_STARTPOSX,							STARTPOSY + 3 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	SetCWndPosition(&edtTempBlackbody, 1 * COLUMN_WIDTH + EDT_STARTPOSX,					STARTPOSY + 5 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblBlackbody, 1 * COLUMN_WIDTH + LBL_STARTPOSX,						STARTPOSY + 5 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	SetCWndPosition(&edtTemp,	1 * COLUMN_WIDTH + EDT_STARTPOSX,							STARTPOSY + 6 * (HEIGHTS + DELTAY), EDT_WIDTHS, HEIGHTS);
	SetCWndPosition(&lblTemp,	1 * COLUMN_WIDTH + LBL_STARTPOSX,							STARTPOSY + 6 * (HEIGHTS + DELTAY), LBL_WIDTHS, HEIGHTS);

	SetCWndPosition(&lblImage2, 1 * COLUMN_WIDTH + LBL_STARTPOSX,							STARTPOSY + 8 * (HEIGHTS + DELTAY), 320, 240);

	SetCWndPosition(&btnLoadLib, 2 * COLUMN_WIDTH + 20, STARTPOSY + 0 * (HEIGHTS + DELTAY), LBL_WIDTHS - 20, HEIGHTS);
	SetCWndPosition(&btnRepaint, 2 * COLUMN_WIDTH + 20, STARTPOSY + 1 * (HEIGHTS + DELTAY), LBL_WIDTHS - 20, HEIGHTS);

	WriteParameterToEdit(&lblParam0,"Width");
	WriteParameterToEdit(&lblParam1,"Height");
	WriteParameterToEdit(&lblParam2,"Epsilon");
	WriteParameterToEdit(&lblParam3,"EnvTemp");
	WriteParameterToEdit(&lblParam4,"Absorption");
	WriteParameterToEdit(&lblParam5,"Obj.dist");
	WriteParameterToEdit(&lblParam6,"PathTemp");
	WriteParameterToEdit(&lblParam7,"old Timestamp");
	WriteParameterToEdit(&lblParam8,"MilliSeconds");
	WriteParameterToEdit(&lblParam9,"Timestamp");
	WriteParameterToEdit(&lblParam10,"Camera");
	WriteParameterToEdit(&lblParam11,"Serial No.");
	WriteParameterToEdit(&lblParam12,"Lens");
	WriteParameterToEdit(&lblParam13,"Calib Lower");
	WriteParameterToEdit(&lblParam14,"Calib Upper");

	WriteParameterToEdit(&lblTemp,"Corrected temp.");
	WriteParameterToEdit(&lblBlackbody,"Blackbody temp.");

	WriteParameterToEdit(&edtFrameIndex, "0");
	WriteParameterToEdit(&edtCoordX, "0");
	WriteParameterToEdit(&edtCoordY, "0");
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}


void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CDemoDlg::OnPaint() 
{



	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	

}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CDemoDlg::DestroyWindow() 
{
	Lib.DeInit();
	return CDialog::DestroyWindow();
}


void CDemoDlg::OnBnClickedBtnloadlib()
{

#ifdef WIN64
	const char* DLLName = "C:\\Program Files\\InfraTec\\IRBACS_SDK\\v2\\irbacs_win64.dll";
#else
	#ifdef WIN32
	const char* DLLName = _T("irbacs_win32.dll");
	#else
		#error No platform specifies!;
	#endif
#endif

	CString filename = "";
	char sFrameCount[11];
	
	if (!Lib.IsLibLoaded())
	{
		MyGraphic.Init(&lblImage2, this);
		if (Lib.LoadLib(DLLName))
		{
			btnLoadLib.SetWindowText(_T("Unload"));
			if (Lib.InitFunctions())
			{
				MessageBox(_T("Init functions fails"), _T("Error"), MB_OK);
			}
			else
			{
				edtFilename.GetWindowText(filename); 
				// ############# Convertion to UTF-8 #################
				// Function LoadIRB takes UTF-8 strings
				// Windows CString is a ASCII string of Codepage 8859-1 for example
				// and must be converted to UTF-8

				int lenA = lstrlenA((LPCTSTR)filename);
				BSTR unicodestr;

				int lenW = ::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, 0, 0);
				if (lenW > 0)
				{
					// Check whether conversion was successful
					unicodestr = ::SysAllocStringLen(0, lenW);
					::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, unicodestr, lenW);
				}

				char *ansistr;
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
					//sFrameCount = IntToString(Lib.GetFrameCount());
					FormatIntToString(Lib.GetFrameCount(), "", sFrameCount);
					edtFrameCount.SetWindowText(sFrameCount);
					ActualizeFrameParameters(0);
					ActualizePixelValues(&edtTemp, &edtTempBlackbody);
				}
				else
				{
					MessageBox(_T("File not found"), _T("Error"), MB_OK);
				}
				//############## Free used memory ####################
				delete[] ansistr;
				::SysFreeString(unicodestr);
				//##################################
			}
		}
		else
		{
			btnLoadLib.SetWindowText(_T("Load FAIL"));
		}
	}
	else
	{
		Lib.DeInit();
		MyGraphic.DeInit();

		btnLoadLib.SetWindowText(_T("Load"));
		for (uint32_t ii = 0; ii < PARAM_COUNT; ii++)
		{
			WriteParameterToEdit(EditLink[ii], _T(""));
		}
		WriteParameterToEdit(&edtFrameCount, _T(""));
		WriteParameterToEdit(&edtFrameIndex, _T("0"));
		WriteParameterToEdit(&edtCoordX, _T("0"));
		WriteParameterToEdit(&edtCoordY, _T("0"));
	}
}



void CDemoDlg::OnDeltaposSpinCordx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	ChangeNumberEditWithInt(&edtCoordX, pNMUpDown->iDelta, 0, (Lib.GetWidth() - 1));

	*pResult = 0;
}


void CDemoDlg::OnDeltaposSpinCoordy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ChangeNumberEditWithInt(&edtCoordY, pNMUpDown->iDelta, 0, (Lib.GetHeight() - 1));
	*pResult = 0;
}


void CDemoDlg::OnEnChangeEditFrameindex()
{
	if (editchangelock) { return; }
	editchangelock = true;
	ActualizeFrameParameters( ChangeNumberEditWithInt(&edtFrameIndex, 0, 0, (Lib.GetFrameCount() - 1)));
	ActualizePixelValues(&edtTemp, &edtTempBlackbody);
	editchangelock = false;
}


void CDemoDlg::OnDeltaposSpinFrameindex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (editchangelock) { return; }	//Writing the incremented value back to the edit calls an OnEnChangeEditFrameindex-Event so the function will be called twice
	
	editchangelock = true;
	ActualizeFrameParameters(ChangeNumberEditWithInt(&edtFrameIndex, pNMUpDown->iDelta, 0, (Lib.GetFrameCount() - 1)));
	ActualizePixelValues(&edtTemp, &edtTempBlackbody);
	editchangelock = false;
	*pResult = 0;
}

int ChangeNumberEditWithInt(CEdit *Edit, int Delta, int LowerLimit, int UpperLimit)
{
	CString FieldInput;
	char FieldOutput[10];	//Limited to counts of (u)int32
	int valueNew = 0;
	int valueOld = 0;

	//Read current value from edit
	Edit->GetWindowText(FieldInput);
	valueOld = atoi(FieldInput);

	//calculate new value
	valueNew = valueOld - Delta;
	if (valueNew >= UpperLimit) { valueNew = UpperLimit; }
	if (valueNew < LowerLimit)	{ valueNew = LowerLimit; }

	if (valueOld != valueNew)
	{
		FormatIntToString(valueNew, "", FieldOutput);
		Edit->SetWindowText(FieldOutput);
	}
	return valueNew;
}
//TODO ofstream includen, ggf seperator anpassen, testen
//#include <ofstream>


void CDemoDlg::OnEnChangeEditCoordx()
{
	std::ofstream saveFile;
	saveFile.open("ThermoArrayTest.txt");
	coordX = ChangeNumberEditWithInt(&edtCoordX, 0, 0, (Lib.GetWidth() - 1));
	ActualizePixelValues(&edtTemp, &edtTempBlackbody);
	//TODO
	//Lib.ReadPixelData(&edtTemp, 1);
	//tfpReadPixelData(hIRBFile, PixelData, PixelType);
	double tempTemp;
	int pixelX, pixelY;
	for (pixelY = 0; pixelY < Lib.GetHeight(); pixelY++) {
		for (pixelX = 0; pixelX < Lib.GetWidth(); pixelX++) {
			tempTemp = Lib.GetTemp(pixelX, pixelY);
			saveFile << tempTemp <<";";
			//saveFile << changeDecimalSeparator(tempTemp) << ";";
		}
		saveFile << "\n";
	}
	saveFile.close();
	
}
/*
std::string CDemoDlg::changeDecimalSeparator(double value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << std::setprecision(2) << value;
	return ss.str();
}*/


void CDemoDlg::OnEnChangeEditCoordy()
{
	coordY = ChangeNumberEditWithInt(&edtCoordY, 0, 0, (Lib.GetHeight() - 1));
	ActualizePixelValues(&edtTemp, &edtTempBlackbody);
}


void CDemoDlg::OnBnClickedButton4()
{
	MyGraphic.Reset();
	Lib.ReadIRBDataUncompressed();
}

void ActualizeFrameParameters(int FrameIndex)
{
	int ii;
	char Value[64];

	if (Lib.SelectFrame(FrameIndex))
	{
		for (ii = 0; ii < PARAM_COUNT; ii++)
		{
			Lib.GetParameterString(ii, Value);
			WriteParameterToEdit(EditLink[ii], Value);
		}
	}
}

void ActualizePixelValues(CEdit *edtTemp, CEdit* edtTempBB)
{
	char FieldOutput[10];
	if (edtTempBB != NULL)
	{
		FormatDoubleToString(Lib.GetTempBlackBody(coordX, coordY), 2, _T(" K"), FieldOutput);
		edtTempBB->SetWindowText(FieldOutput);
	}

	if (edtTemp != NULL)
	{
		FormatDoubleToString(Lib.GetTemp(coordX, coordY), 2, _T(" K"), FieldOutput);
		edtTemp->SetWindowText(FieldOutput);
	}

}



