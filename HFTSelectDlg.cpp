#include "stdafx.h"
#include "CNCHinges.h"

extern "C++" void ReportError(DWORD dwError);
extern "C++" void ReportError(const char* pchError);

// Message handler for Hinge Form Tool  Selection box.
INT_PTR CALLBACK HFTSelectDlg(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	BOOL bResult;
	int i;
	UINT uiResult;
	HRESULT hRes;
	LRESULT lRes;
	static LPHFTSELECTPARMS lpHFTs;
	TCHAR szHFTSelection[8];
	PVOID pvTemp;
	const std::wstring wstrTurretTools[] = {
		_T("T1"),
		_T("T2"),
		_T("T3"),
		_T("T4"),
		_T("T5"),
		_T("T6"),
		_T("T7"),
		_T("T8"),
		_T("T9"),
		_T("T10"),
		_T("T11"),
		_T("T12"),
		_T("T13"),
		_T("T14"),
		_T("T15"),
		_T("T16"),
		_T("T17"),
		_T("T18"),
		_T("T19"),
		_T("T20"),
		_T("T21"),
		_T("T22"),
		_T("T23"),
		_T("T24"),
		_T("T25"),
		_T("T26"),
		_T("T27"),
		_T("T28"),
		_T("T29"),
		_T("T30")};

	switch (message)
	{
	case WM_INITDIALOG:
		lpHFTs = (LPHFTSELECTPARMS)lParam;
		for ( i = 0; i < sizeof(wstrTurretTools)/sizeof(std::wstring); i++ )
		{
			lRes = ::SendMessage(::GetDlgItem(hDlg,IDC_SELECTHFT1LIST),LB_ADDSTRING,(WPARAM)0,(LPARAM)wstrTurretTools[i].c_str());
			lRes = ::SendMessage(::GetDlgItem(hDlg,IDC_SELECTHFT2LIST),LB_ADDSTRING,(WPARAM)0,(LPARAM)wstrTurretTools[i].c_str());
		}
		bResult = ::SetDlgItemText(hDlg,IDC_STATICHFT1TEXT,lpHFTs->lpwstrHFT1->c_str());
		bResult = ::SetDlgItemText(hDlg,IDC_STATICHFT2TEXT,lpHFTs->lpwstrHFT2->c_str());
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_SELECTHFT1LIST && HIWORD(wParam) == LBN_DBLCLK)
		{
			lRes = ::SendMessage((HWND)lParam,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			bResult = ::SetDlgItemText(hDlg,IDC_STATICHFT1TEXT,wstrTurretTools[lRes].c_str());
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_SELECTHFT2LIST && HIWORD(wParam) == LBN_DBLCLK)
		{
			lRes = ::SendMessage((HWND)lParam,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			bResult = ::SetDlgItemText(hDlg,IDC_STATICHFT2TEXT,wstrTurretTools[lRes].c_str());
			return (INT_PTR)TRUE;
		}
		if ( LOWORD(wParam) == IDOK )
		{
			pvTemp = ::SecureZeroMemory((PVOID)szHFTSelection,sizeof(szHFTSelection));
			uiResult = ::GetDlgItemText(hDlg,IDC_STATICHFT1TEXT,szHFTSelection,sizeof(szHFTSelection)/sizeof(TCHAR));
			lpHFTs->lpwstrHFT1->assign(szHFTSelection);
			pvTemp = ::SecureZeroMemory((PVOID)szHFTSelection,sizeof(szHFTSelection));
			uiResult = ::GetDlgItemText(hDlg,IDC_STATICHFT2TEXT,szHFTSelection,sizeof(szHFTSelection)/sizeof(TCHAR));
			lpHFTs->lpwstrHFT2->assign(szHFTSelection);
			EndDialog(hDlg,LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if ( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog(hDlg,LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
