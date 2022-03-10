// CNCHinges.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CNCHinges.h"
#include "HitCoords.h"
#include "Knuckle.h"
#include "Knuckles.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
static HMODULE g_msft_edit;
static HMODULE g_rich_edit;
std::wstring wstrTitle;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
extern "C++" CNCCodes GetCNCStrings(TCHAR* szCNCFileName,std::wstring** pwstrPreKnuckles,std::wstring** pwstrKnuckles,std::wstring** pwstrPostKnuckles,LPHFTSELECTPARMS lpHFTSelections);
extern "C++" INT_PTR CALLBACK HFTSelectDlg(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam);

void ReportError(DWORD dwError)
{
	TCHAR* lpErrMsg = NULL;
	TCHAR szErrMsg[1024];
	DWORD dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpErrMsg,
		MAX_LOADSTRING,
		NULL);
	HRESULT hRes = StringCchPrintf(szErrMsg, sizeof(szErrMsg)/sizeof(TCHAR), _T("Error %d: %s"),dwError,lpErrMsg);
	int nResult = ::MessageBox(NULL,(LPWSTR)szErrMsg,szTitle,MB_OK);
	if ( lpErrMsg )
	{
		::LocalFree(lpErrMsg);
		lpErrMsg = NULL;
	}
}

void ReportError(const char* pchError)
{
	TCHAR* lpErrMsg = NULL;
	TCHAR szErrMsg[1024];
	PVOID pvTemp = ::SecureZeroMemory((PVOID)szErrMsg,sizeof(szErrMsg));
	size_t nLen = ::strnlen_s(pchError,sizeof(szErrMsg)/sizeof(TCHAR));
	int nResult = ::MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED,
		pchError,
		nLen,
		szErrMsg,
		sizeof(szErrMsg)/sizeof(TCHAR));
//	HRESULT hRes = StringCchPrintf(szErrMsg, sizeof(szErrMsg)/sizeof(TCHAR), _T("Error %d: %s"),dwError,lpErrMsg);
	nResult = ::MessageBox(NULL,(LPWSTR)szErrMsg,_T("Trash35"),MB_OK);
	if ( lpErrMsg )
	{
		::LocalFree(lpErrMsg);
		lpErrMsg = NULL;
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	BOOL bResult;
	DWORD dwError;
	INITCOMMONCONTROLSEX iccex = {0};

	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_STANDARD_CLASSES|ICC_BAR_CLASSES;
	bResult = ::InitCommonControlsEx(&iccex);
	dwError = ::GetLastError();
	if ( !bResult )
	{
		if ( ERROR_SUCCESS != dwError )
		{
			ReportError(::GetLastError());
			return 0;
		}
	}

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CNCHINGES, szWindowClass, MAX_LOADSTRING);
	wstrTitle.assign(szTitle);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CNCHINGES));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_CNCHINGES));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CNCHINGES);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass,
	   szTitle,
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT,
	   0,
	   CW_USEDEFAULT,
	   0,
	   NULL,
	   NULL,
	   hInstance,
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult;
	static BOOL bSuccess;
	DWORD dwResult;
	DWORD dwError;
	int wmId, wmEvent;
	int nResult;
	long lStatusWH;
	long lFileTextWH;
	PAINTSTRUCT ps;
	HDC hdc;
	static OPENFILENAME ofn;
	static TCHAR szOpenDlgFileName[MAX_PATH];
	static int nStrLen;
	static int idStatus;
	static HMODULE hmodMSFT;
	static HMODULE hmodRICH;
	PVOID pvTemp;
	HRESULT hRes;
	static HWND hwndFileText;
	static HWND hwndStatus;
	static RECT crect;
	static RECT rectStatus;
	static HMENU hMainMenu;
	LRESULT lRes;
	const std::wstring wstrIntro(_T("On the menu above click on File, then Open... to select a file"));
	const std::wstring wstrIntro01(_T("\nHinge Form Tool1 = "));
	const std::wstring wstrIntro02(_T("\nHinge Form Tool2 = "));
	const std::wstring wstrIntro03(_T("\nTo select Hinge Form tools click Tooling on the menu above"));
	const std::wstring NegHFT1Msg(_T("The selected file does not contain any Hinge Form Tool 1 hits\nPlease select onother file"));
	const std::wstring NegHFT2Msg(_T("The selected file does not contain any Hinge Form Tool 2 hits\nPlease select onother file"));
	static CHARFORMAT2 cf2CharFormat;
	const TCHAR szFontFace[] = _T("Calibri");
	static std::wstring* lpwstrPreK;
	static std::wstring* lpwstrK;
	static std::wstring* lpwstrPostK;
	static std::wstring* lpwstrCNCAll;
	static std::wstring wstrInfoMsg;
	static std::wstring wstrStatus;
	CNCCodes rc;
	INT_PTR ipResult;
	static LPHFTSELECTPARMS lpHFTSelections;
	static std::wstring wstrHFT1;
	static std::wstring wstrHFT2;

	switch (message)
	{
	case WM_CREATE:
		wstrHFT1.assign(_T(HINGEFORMT1));
		wstrHFT2.assign(_T(HINGEFORMT2));
		try
		{
			lpHFTSelections = (LPHFTSELECTPARMS)new HFTSELECTPARMS;
		}
		catch (exception &ee)
		{
			ReportError(ee.what());
			DestroyWindow(hWnd);
			break;
		}
		if ( NULL != lpHFTSelections )
		{
			lpHFTSelections->lpwstrHFT1 = &wstrHFT1;
			lpHFTSelections->lpwstrHFT2 = &wstrHFT2;
		}
		bSuccess = true;
		pvTemp = ::SecureZeroMemory((PVOID)szOpenDlgFileName,sizeof(szOpenDlgFileName));
		hMainMenu = ::GetMenu(hWnd);
		bResult = ::EnableMenuItem(hMainMenu,IDM_OPENFILE,MF_GRAYED);
		nStrLen = 0;
		lpwstrPreK = NULL;
		lpwstrK = NULL;
		lpwstrPostK = NULL;
		lpwstrCNCAll = NULL;
		wstrInfoMsg.assign(wstrIntro);
		wstrInfoMsg.append(wstrIntro01);
		wstrInfoMsg.append(wstrHFT1);
		wstrInfoMsg.append(wstrIntro02);
		wstrInfoMsg.append(wstrHFT2);
		wstrInfoMsg.append(wstrIntro03);

		idStatus = IDC_STATUSBAR;
		hwndStatus = CreateWindowEx(
			0,                       // no extended styles
			STATUSCLASSNAME,         // name of status bar class
			(PCTSTR)NULL,           // no text when first created
			SBARS_SIZEGRIP|WS_CHILD|WS_VISIBLE, // includes a sizing grip and creates a visible child window
			0, 0, 0, 0,              // ignores size and position
			hWnd,              // handle to parent window
			(HMENU)NULL,       // child window identifier
			hInst,                   // handle to application instance
			NULL);                   // no window creation data
//			(HMENU)idStatus,       // child window identifier

//		if ( ERROR_SUCCESS != (dwError = ::GetLastError()) )
//		{
//			ReportError(dwError);
//			break;
//		}
		if ( NULL != hwndStatus )
		{
			wstrStatus.assign(_T("Hinge Form Tool 1: "));
			wstrStatus.append(lpHFTSelections->lpwstrHFT1->c_str());
			wstrStatus.append(_T(" Hinge Form Tool 2: "));
			wstrStatus.append(lpHFTSelections->lpwstrHFT2->c_str());
			bResult = ::SetWindowText(hwndStatus,wstrStatus.c_str());
		}
		hmodMSFT = ::LoadLibrary(_T("Msftedit.dll"));
		dwError = ::GetLastError();
		if ( hmodMSFT )
		{
			bResult = ::GetClientRect(hWnd,&crect);
			bResult = ::GetClientRect(hwndStatus,&rectStatus);
			hwndFileText = ::CreateWindowEx(0,
				MSFTEDIT_CLASS,
				NULL,
				WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE|WS_THICKFRAME|ES_SUNKEN,
				crect.left,
				crect.top,
				crect.right-crect.left,
				(crect.bottom-crect.top)-(rectStatus.bottom-rectStatus.top),
				hWnd,
				NULL,
				hInst,
				NULL);
			if ( NULL == hwndFileText )
			{
				ReportError(::GetLastError());
				break;
			}
			bResult = ::EnableMenuItem(hMainMenu,IDM_OPENFILE,MF_ENABLED);
			pvTemp = ::SecureZeroMemory((PVOID)&cf2CharFormat,sizeof(cf2CharFormat));
			cf2CharFormat.cbSize = sizeof(CHARFORMAT2);
			cf2CharFormat.dwMask = CFM_FACE;
			hRes = ::StringCchPrintf(cf2CharFormat.szFaceName,sizeof(cf2CharFormat.szFaceName)/sizeof(TCHAR),_T("%s"),szFontFace);
			lRes = ::SendMessage(hwndFileText,EM_SETCHARFORMAT,(WPARAM)SCF_ALL,(LPARAM)&cf2CharFormat);
			break;
		}
		if ( !hmodMSFT )
		{
			hmodRICH = ::LoadLibrary(_T("Richedit20.dll"));
			dwError = ::GetLastError();
			if ( ERROR_SUCCESS != dwError )
			{
				ReportError(dwError);
				break;
			}
			if ( hmodRICH )
			{
				::GetClientRect(hWnd,&crect);
				hwndFileText = ::CreateWindowEx(0,MSFTEDIT_CLASS,NULL,WS_CHILD|WS_VISIBLE|WS_THICKFRAME,crect.left,crect.top,crect.right-crect.left,crect.bottom-crect.top,hWnd,NULL,hInst,NULL);
				if ( NULL == hwndFileText )
				{
					ReportError(::GetLastError());
					break;
				}
				bResult = ::EnableMenuItem(hMainMenu,IDM_OPENFILE,MF_ENABLED);
				pvTemp = ::SecureZeroMemory((PVOID)&cf2CharFormat,sizeof(cf2CharFormat));
				cf2CharFormat.cbSize = sizeof(CHARFORMAT2);
				cf2CharFormat.dwMask = CFM_FACE;
				hRes = ::StringCchPrintf(cf2CharFormat.szFaceName,sizeof(cf2CharFormat.szFaceName)/sizeof(TCHAR),_T("%s"),szFontFace);
				lRes = ::SendMessage(hwndFileText,EM_SETCHARFORMAT,(WPARAM)SCF_ALL,(LPARAM)&cf2CharFormat);
				break;
			}
		}
		if ( ERROR_SUCCESS != (dwError = ::GetLastError()) )
		{
			ReportError(dwError);
			break;
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_OPENFILE:
			if ( NULL != lpwstrPreK )
			{
				delete lpwstrPreK;
				lpwstrPreK = NULL;
			}
			if ( NULL != lpwstrK )
			{
				delete lpwstrK;
				lpwstrK = NULL;
			}
			if ( NULL != lpwstrPostK )
			{
				delete lpwstrPostK;
				lpwstrPostK = NULL;
			}
			if ( NULL != lpwstrCNCAll )
			{
				delete lpwstrCNCAll;
				lpwstrCNCAll = NULL;
			}
			pvTemp = ::SecureZeroMemory((PVOID)szOpenDlgFileName,sizeof(szOpenDlgFileName));
			pvTemp = ::SecureZeroMemory((PVOID)&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = (LPWSTR)szOpenDlgFileName;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szOpenDlgFileName)/sizeof(TCHAR);
			ofn.lpstrFilter = _T("PRO files\0*.PRO\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			bResult = ::GetOpenFileName(&ofn);
			dwError = ::GetLastError();
			if ( !bResult )
			{
				if ( ERROR_SUCCESS != dwError )
				{
					ReportError(dwError);
				}
				break;
			}
			nStrLen = 0;
			hRes = ::StringCchLength(szOpenDlgFileName,sizeof(szOpenDlgFileName)/sizeof(TCHAR),(size_t*)&nStrLen);
			if ( nStrLen )
			{
				rc = GetCNCStrings(szOpenDlgFileName,&lpwstrPreK,&lpwstrK,&lpwstrPostK,lpHFTSelections);
			}
			if ( nStrLen && (rc == NoError) )
			{
				wstrTitle.assign(szTitle);
				if ( NULL != ofn.lpstrFile )
				{
					wstrTitle.append(_T(" - "));
					wstrTitle.append(ofn.lpstrFile);
				}
				bResult = ::SetWindowText(hWnd,wstrTitle.c_str());
				lpwstrCNCAll = new std::wstring;
				lpwstrCNCAll->append(*lpwstrPreK);
				lpwstrCNCAll->append(*lpwstrK);
				lpwstrCNCAll->append(*lpwstrPostK);
			}
			if ( nStrLen && (rc == NoHFT1) )
			{
				wstrInfoMsg.assign(NegHFT1Msg);
			}
			if ( nStrLen && (rc == NoHFT2) )
			{
				wstrInfoMsg.assign(NegHFT2Msg);
			}
			bResult = ::InvalidateRect(hwndFileText,NULL,true);
			bResult = ::InvalidateRect(hWnd,NULL,true);
			break;
		case IDM_HFTSELECT:
			if ( NULL == lpHFTSelections )
			{
				try
				{
					lpHFTSelections = (LPHFTSELECTPARMS)new HFTSELECTPARMS;
				}
				catch (std::exception &ee)
				{
					ReportError(ee.what());
					break;
				}
			}
			if ( NULL != lpHFTSelections )
			{
				lpHFTSelections->lpwstrHFT1 = &wstrHFT1;
				lpHFTSelections->lpwstrHFT2 = &wstrHFT2;
				ipResult = ::DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_HFTSELECTBOX),hWnd,HFTSelectDlg,(LPARAM)lpHFTSelections);
				if ( IDOK == ipResult )
				{
					wstrInfoMsg.assign(wstrIntro);
					wstrInfoMsg.append(wstrIntro01);
					wstrInfoMsg.append(wstrHFT1);
					wstrInfoMsg.append(wstrIntro02);
					wstrInfoMsg.append(wstrHFT2);
					wstrInfoMsg.append(wstrIntro03);
					if ( NULL != hwndStatus )
					{
						wstrStatus.assign(_T("Hinge Form Tool 1: "));
						wstrStatus.append(lpHFTSelections->lpwstrHFT1->c_str());
						wstrStatus.append(_T(" Hinge Form Tool 2: "));
						wstrStatus.append(lpHFTSelections->lpwstrHFT2->c_str());
						bResult = ::SetWindowText(hwndStatus,wstrStatus.c_str());
					}
					bResult = ::InvalidateRect(hwndFileText,NULL,true);
					bResult = ::InvalidateRect(hWnd,NULL,true);
				}
			}
			break;
		case IDM_ABOUT:
			ipResult = DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUTBOX),hWnd,About);
			break;
		case IDM_EXIT:
			if ( NULL != lpwstrPreK )
			{
				delete lpwstrPreK;
				lpwstrPreK = NULL;
			}
			if ( NULL != lpwstrK )
			{
				delete lpwstrK;
				lpwstrK = NULL;
			}
			if ( NULL != lpwstrPostK )
			{
				delete lpwstrPostK;
				lpwstrPostK = NULL;
			}
			if ( NULL != lpwstrCNCAll )
			{
				delete lpwstrCNCAll;
				lpwstrCNCAll = NULL;
			}
			if ( NULL != lpHFTSelections )
			{
				delete lpHFTSelections;
				lpHFTSelections = NULL;
			}
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		if ( NULL != hwndFileText )
		{
			if ( NULL != lpwstrCNCAll )
			{
				if ( (NULL != lpwstrCNCAll->c_str()) && lpwstrCNCAll->size() )
				{
					lRes = ::SendMessage(hwndFileText,WM_SETTEXT,(WPARAM)NULL,(LPARAM)lpwstrCNCAll->c_str());
				}
			}
			else
			{
				lRes = ::SendMessage(hwndFileText,WM_SETTEXT,(WPARAM)NULL,(LPARAM)wstrInfoMsg.c_str());
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		bResult = ::GetClientRect(hWnd,&crect);
		bResult = ::GetClientRect(hwndStatus,&rectStatus);
		lStatusWH = rectStatus.bottom-rectStatus.top;
		lStatusWH <<= 16;
		lStatusWH += rectStatus.right-rectStatus.left;
		lRes = ::SendMessage(hwndStatus,WM_SIZE,(WPARAM)SIZE_RESTORED,(LPARAM)lStatusWH);
		bResult = ::GetClientRect(hwndStatus,&rectStatus);
		lFileTextWH = (crect.bottom-crect.top)-(rectStatus.bottom-rectStatus.top);
		lFileTextWH <<= 16;
		lFileTextWH += crect.right-crect.left;
//		lRes = ::SendMessage(hwndFileText,WM_SIZE,(WPARAM)SIZE_RESTORED,(LPARAM)lFileTextWH);
		bResult = ::MoveWindow(hwndFileText,
			crect.left,
			crect.top,crect.right-crect.left,
			(crect.bottom-crect.top)-(rectStatus.bottom-rectStatus.top),
			true);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
