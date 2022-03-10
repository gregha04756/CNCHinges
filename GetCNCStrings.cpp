
#include "stdafx.h"
#include "CNCHinges.h"
#include "HitCoords.h"
#include "Knuckle.h"
#include "Knuckles.h"
using namespace std;

extern "C++" TCHAR szTitle[];

extern "C++" void ReportError(DWORD dwError);
extern "C++" void ReportError(const char* pchError);

CNCCodes GetCNCStrings(TCHAR* szCNCFileName,std::wstring** lpwstrPreKnuckles,std::wstring** lpwstrKnuckles,std::wstring** lpwstrPostKnuckles,LPHFTSELECTPARMS lpHFTSelections)
{
//	bool bSuccess = true;
	CNCCodes RetCode = NoError;
	char cch;
	int i;
	int nResult;
	int nStrLen;
	int* pvals = NULL;
	int* pparms = NULL;
	char* pch = NULL;
	char* pchinline = NULL;
	int fldcnt = 0;
	int linecnt = 0;
	int nHFT1InitPos = 0;
	int nHFT2InitPos = 0;
	int nHFT1Hits = 0;
	int nHFT2Hits = 0;
	int nCurPos = 0;
	long lLen;
	ifstream* lpinfile = NULL;
	std::string ifline;
	std::string strXNow;
	std::string strYNow;
	std::string strTemp;
	size_t nXOffset = 0;
	size_t nYOffset = 0;
	DWORD dwError;
	const char nl[] = {'\r','\n'};
	const std::string strnl(nl,2);
	const TCHAR wnl[] = {L'\n'};
	const std::wstring wstrnl(wnl,1);
	char* pchFileName = NULL;
	HRESULT hRes;
	PVOID pvTemp;

	std::list<CHitCoords> listHFT1Hits;
	std::list<CHitCoords> listHFT2Hits;

	CKnuckles* lpKnuckles = NULL;
	std::string* lpstrKnuckles = NULL;
	std::string* lpstrPostKnuckles = NULL;

	std::string strHFT1;
	std::string strHFT2;

	std::list<CKnuckle>::iterator knucklesit;
	std::filebuf* fb = NULL;
	char* pchBuffer = NULL;
	std::string* psfc = NULL;
	std::string strSortedName;
	std::ofstream blah;
	std::ifstream is;

	TCHAR szError[100];
	TCHAR* szKnuckles = NULL;
	TCHAR* szPreKnuckles = NULL;
	TCHAR* szPostKnuckles = NULL;

	*lpwstrPreKnuckles = NULL;
	*lpwstrKnuckles = NULL;
	*lpwstrPostKnuckles = NULL;
	char* pchHFT;

	try
	{
		nStrLen = lpHFTSelections->lpwstrHFT1->size();
		pchHFT = (char*)new char[nStrLen+1];
	}
	catch (exception& ee)
	{
		ReportError(ee.what());
		return Failure;
	}
	if ( NULL != pchHFT )
	{
		pvTemp = ::SecureZeroMemory((PVOID)pchHFT,(SIZE_T)(nStrLen+1));
	}
	nResult = ::WideCharToMultiByte(CP_ACP,
		0,
		lpHFTSelections->lpwstrHFT1->c_str(),
		nStrLen,
		pchHFT,
		nStrLen,
		NULL,
		NULL);

	try
	{
		strHFT1.assign(pchHFT);
	}
	catch (exception& ee)
	{
		ReportError(ee.what());
		return Failure;
	}

	if ( NULL != pchHFT )
	{
		delete[] pchHFT;
		pchHFT = NULL;
	}

	try
	{
		nStrLen = lpHFTSelections->lpwstrHFT2->size();
		pchHFT = (char*)new char[nStrLen+1];
	}
	catch (exception& ee)
	{
		ReportError(ee.what());
		return Failure;
	}
	if ( NULL != pchHFT )
	{
		pvTemp = ::SecureZeroMemory((PVOID)pchHFT,(SIZE_T)(nStrLen+1));
	}
	nResult = ::WideCharToMultiByte(CP_ACP,
		0,
		lpHFTSelections->lpwstrHFT2->c_str(),
		nStrLen,
		pchHFT,
		nStrLen,
		NULL,
		NULL);

	try
	{
		strHFT2.assign(pchHFT);
	}
	catch (exception& ee)
	{
		ReportError(ee.what());
		return Failure;
	}

	try
	{
		lpinfile = (std::ifstream*)new ifstream();
	}
	catch (std::bad_alloc ba)
	{
		::ReportError(::GetLastError());
		RetCode = Failure;
	}
	if ( RetCode == NoError )
	{
		lpinfile->exceptions(ifstream::failbit|ifstream::badbit);
		hRes = ::StringCchLength(szCNCFileName,(size_t)MAX_PATH,(size_t*)&nStrLen);
		pchFileName = (char*)new char[nStrLen+1];
		pvTemp = ::SecureZeroMemory((PVOID)pchFileName,(SIZE_T)nStrLen+1);
		nResult = ::WideCharToMultiByte(CP_ACP,
			0,
			szCNCFileName,
			nStrLen,
			pchFileName,
			nStrLen,
			NULL,
			NULL);
		try
		{
			lpinfile->open(pchFileName,ifstream::in);
		}
		catch (ifstream::failure fof)
		{
			ReportError(::GetLastError());
			RetCode = Failure;
		}
	}
	if ( RetCode == NoError )
	{
		lpinfile->seekg(0,ios::end);
		lLen = (long)lpinfile->tellg();
		fb = lpinfile->rdbuf();
		fb->pubseekpos(0,ios::in);
		pchBuffer = (char*)new char[lLen+1];
		pvTemp = ::SecureZeroMemory((PVOID)pchBuffer,(SIZE_T)lLen+1);
		fb->sgetn(pchBuffer,lLen);
		psfc = (std::string*)new std::string(pchBuffer);
	}
	if ( RetCode == NoError )
	{
		nXOffset = psfc->find(strHFT1,0);
		RetCode = (string::npos == nXOffset) ? NoHFT1 : NoError;
	}
	if ( RetCode == NoError )
	{
		nXOffset = psfc->find(strHFT2,0);
		RetCode = (string::npos == nXOffset) ? NoHFT2 : NoError;
	}
	if ( NULL != pchBuffer )
	{
		delete[] pchBuffer;
		pchBuffer = NULL;
	}
	if ( NULL != psfc )
	{
		delete psfc;
		psfc = NULL;
	}
	if ( RetCode == NoError )
	{
		try
		{
			lpinfile->seekg(ifstream::beg);					/* Seek to beginning of file			*/
		}
		catch (ifstream::failure fof)
		{
			ReportError(::GetLastError());
			RetCode = Failure;
		}
	}
	if ( (RetCode == NoError) && lpinfile->eof() )
	{
		lpinfile->clear();
	}
	if ( RetCode == NoError )
	{
		if ( lpinfile->is_open() )
		{
			lpinfile->seekg(ifstream::beg);
			while ( !lpinfile->eof() )
			{
				cch = lpinfile->peek();
				if ( lpinfile->rdstate() & ifstream::eofbit )
				{
					break;
				}
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
				linecnt++;
			}
		}
		if ( lpinfile->eof() )
		{
			lpinfile->clear();
		}
		if ( RetCode == NoError )
		{
			try
			{
				lpinfile->seekg(0,ifstream::beg);					/* Seek to beginning of file			*/
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
		}
		if ( (RetCode == NoError) && lpinfile->eof() )
		{
			lpinfile->clear();
		}
		if ( RetCode == NoError )										/* Determine location of first Hinge Form	*/
		{													/* tool hit.								*/
			while ( !lpinfile->eof() )
			{
				nCurPos = lpinfile->tellg();
				cch = lpinfile->peek();
				if ( lpinfile->rdstate() & ifstream::eofbit )
				{
					break;
				}
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
				try
				{
					if ( !(ifline.c_str()[0] == '(')  )
					{
						if ( ifline.rfind(strHFT1) != string::npos )
						{
							nHFT1InitPos = nCurPos;
						}
						if ( ifline.rfind(strHFT2) != string::npos )
						{
							nHFT2InitPos = nCurPos;
						}
					}
				}
				catch (exception& e)
				{
					ReportError(::GetLastError());
					break;
				}
			}
		}

		if ( RetCode == NoError )
		{
			is.open(pchFileName,ios::binary);
			fb = is.rdbuf();
			fb->pubseekpos(0,ios::in);
			pchBuffer = (char*)new char[nHFT1InitPos];
			pvTemp = ::SecureZeroMemory((PVOID)pchBuffer,(SIZE_T)nHFT1InitPos);
			fb->sgetn(pchBuffer,nHFT1InitPos);

			strSortedName.assign(pchFileName);
			strSortedName.insert(strSortedName.find_first_of("."),"-Knuckles_Sorted");
			blah.open(strSortedName.c_str(),ios::binary);
			psfc = (std::string*)new std::string(pchBuffer);
			blah.write(psfc->c_str(),nHFT1InitPos);
			if ( NULL != pchBuffer )
			{
				delete[] pchBuffer;
				pchBuffer = NULL;
			}
			if ( NULL != psfc )
			{
				delete psfc;
				psfc = NULL;
			}

			if ( RetCode == NoError )
			{
				try
				{
					lpinfile->seekg(ifstream::beg);					/* Seek to first tool 1 hit */
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
				}
			}
			if ( (RetCode == NoError) && lpinfile->eof() )
			{
				lpinfile->clear();
			}
			if ( RetCode == NoError )
			{
				try
				{
					szPreKnuckles = (TCHAR*)new TCHAR[nHFT1InitPos+1];
					*lpwstrPreKnuckles = (std::wstring*)new std::wstring();
				}
				catch (std::bad_alloc &ba)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
				}
				nCurPos = lpinfile->tellg();
				while ( (RetCode == NoError) && !lpinfile->eof() && (nCurPos < nHFT1InitPos) )
				{
					pvTemp = ::SecureZeroMemory((PVOID)szPreKnuckles,(SIZE_T)((nHFT1InitPos+1)*sizeof(TCHAR)));
					cch = lpinfile->peek();
					if ( lpinfile->rdstate() & ifstream::eofbit )
					{
						break;
					}
					try
					{
						getline(*lpinfile,ifline);
					}
					catch (ifstream::failure fof)
					{
						ReportError(::GetLastError());
						RetCode = Failure;
						break;
					}
					nCurPos = lpinfile->tellg();
					nResult = ::MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						ifline.c_str(),
						ifline.size(),
						szPreKnuckles,
						ifline.size());
					(*lpwstrPreKnuckles)->append(szPreKnuckles,ifline.size());
					(*lpwstrPreKnuckles)->append(wstrnl);
				}
			}
			if ( NULL != szPreKnuckles )
			{
				delete[] szPreKnuckles;
				szPreKnuckles = NULL;
			}
		}

		if ( RetCode == NoError )
		{
			lLen = (long)lpinfile->rdbuf()->pubseekoff(0,ios::end,ios::in);
			try
			{
				szPostKnuckles = (TCHAR*)new TCHAR[lLen+1];
				*lpwstrPostKnuckles = (std::wstring*)new std::wstring;
				lpstrPostKnuckles = (std::string*)new std::string;
			}
			catch (std::bad_alloc &ba)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
		}
		if ( (RetCode == NoError) && szPostKnuckles && lLen )
		{
			pvTemp = ::SecureZeroMemory((PVOID)szPostKnuckles,(SIZE_T)((lLen+1)*sizeof(TCHAR)));
		}
		
		if ( RetCode == NoError )
		{
			try
			{
				lpinfile->seekg(nHFT1InitPos);					/* Seek to first tool 1 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
		}
		if ( RetCode == NoError )										/* Determine number of first Hinge Form	*/
		{													/* tool hits */
			nHFT1Hits = 0;
			nCurPos = lpinfile->tellg();
			while ( !lpinfile->eof() && (nCurPos < nHFT2InitPos) )
			{
				cch = lpinfile->peek();
				if ( lpinfile->rdstate() & ifstream::eofbit )
				{
					break;
				}
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
				if ( !(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/') )
				{
					nHFT1Hits = (nCurPos < nHFT2InitPos) ? ++nHFT1Hits : nHFT1Hits;
				}
				if ( (ifline.c_str()[0] == '(') || (ifline.c_str()[0] == '/') )
				{
					nResult = ::MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						ifline.c_str(),
						ifline.size(),
						szPostKnuckles,
						ifline.size());
					(*lpwstrPostKnuckles)->append(szPostKnuckles,ifline.size());
					(*lpwstrPostKnuckles)->append(wstrnl);
					lpstrPostKnuckles->append(ifline.c_str(),ifline.size());
					lpstrPostKnuckles->append(strnl.c_str(),strnl.size());
				}
				nCurPos = lpinfile->tellg();
			}
		}
		if ( RetCode == NoError )
		{
			try
			{
				lpinfile->seekg(nHFT2InitPos);					/* Seek to first tool 2 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
		}
		if ( RetCode == NoError )										/* Determine number of second Hinge Form	*/
		{													/* tool hits */
			nHFT2Hits = 0;
			try
			{
				getline(*lpinfile,ifline);
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				ReportError(fof.what());
				RetCode = Failure;
			}
			nCurPos = lpinfile->tellg();
			while ( (RetCode == NoError) && !lpinfile->eof() && !(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/') )
			{
				nHFT2Hits = (!(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/')) ? ++nHFT2Hits : nHFT2Hits;
				if ( (ifline.c_str()[0] == '(') || (ifline.c_str()[0] == '/') )
				{
					nResult = ::MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						ifline.c_str(),
						ifline.size(),
						szPostKnuckles,
						ifline.size());
					(*lpwstrPostKnuckles)->append(szPostKnuckles,ifline.size());
					(*lpwstrPostKnuckles)->append(wstrnl);
					lpstrPostKnuckles->append(ifline.c_str(),ifline.size());
					lpstrPostKnuckles->append(strnl.c_str(),strnl.size());
				}
				nCurPos = lpinfile->tellg();
				cch = lpinfile->peek();
				if ( lpinfile->rdstate() & ifstream::eofbit )
				{
					break;
				}
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
			}
		}
		if ( RetCode == NoError )											/* Get post-knuckles lines */
		{
			while ( (RetCode == NoError) && !lpinfile->eof() )
			{
				nResult = ::MultiByteToWideChar(CP_ACP,
					MB_PRECOMPOSED,
					ifline.c_str(),
					ifline.size(),
					szPostKnuckles,
					ifline.size());
				(*lpwstrPostKnuckles)->append(szPostKnuckles,ifline.size());
				(*lpwstrPostKnuckles)->append(wstrnl);
				lpstrPostKnuckles->append(ifline.c_str(),ifline.size());
				lpstrPostKnuckles->append(strnl.c_str(),strnl.size());
				nCurPos = lpinfile->tellg();
				cch = lpinfile->peek();
				if ( lpinfile->rdstate() & ifstream::eofbit )
				{
					break;
				}
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
			}
		}

		if ( NULL != szPostKnuckles )
		{
			delete[] szPostKnuckles;
			szPostKnuckles = NULL;
		}


		if ( RetCode == NoError )											/* Parse for tool 1 hit X, Y coordinates */
		{
			if ( lpinfile->eof() )
			{
				lpinfile->clear();
			}
			try
			{
				lpinfile->seekg(nHFT1InitPos);					/* Seek to first tool 1 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
			for ( i = 0; i < nHFT1Hits; i++ )
			{
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}

				nXOffset = ifline.find("X");
				if ( nXOffset == string::npos )
				{
					nXOffset = ifline.find("x");
				}
				if ( nXOffset == string::npos )
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no X coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				strXNow = ifline.substr(nXOffset,string::npos);
				strTemp = strXNow.substr((size_t)0,strXNow.find_first_of(" "));
				if ( strTemp.size() > 0 )
				{
					strXNow = strTemp;
				}
				else
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no X coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				nYOffset = ifline.find("Y");
				if ( nYOffset == string::npos )
				{
					nYOffset = ifline.find("y");
				}
				if ( nYOffset == string::npos )
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no Y coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				strYNow = ifline.substr(nYOffset,string::npos);
				strTemp = strYNow.substr((size_t)0,strYNow.find_first_of(" "));
				if ( strTemp.size() > 0 )
				{
					strYNow = strTemp;
				}
				else
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no Y coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				listHFT1Hits.push_back(CHitCoords(strXNow,strYNow));
			}
		}

		if ( RetCode == NoError )											/* Parse for tool 2 hit X, Y coordinates */
		{
			try
			{
				lpinfile->seekg(nHFT2InitPos);					/* Seek to first tool 2 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
			for ( i = 0; i < nHFT2Hits; i++ )
			{
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
					break;
				}
				nXOffset = ifline.find("X");
				if ( nXOffset == string::npos )
				{
					nXOffset = ifline.find("x");
				}
				if ( nXOffset == string::npos )
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no X coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				strXNow = ifline.substr(nXOffset,string::npos);
				strTemp = strXNow.substr((size_t)0,strXNow.find_first_of(" "));
				if ( strTemp.size() > 0 )
				{
					strXNow = strTemp;
				}
				else
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no X coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				nYOffset = ifline.find("Y");
				if ( nYOffset == string::npos )
				{
					nYOffset = ifline.find("y");
				}
				if ( nYOffset == string::npos )
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no Y coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				strYNow = ifline.substr(nYOffset,string::npos);
				strTemp = strYNow.substr((size_t)0,strYNow.find_first_of(" "));
				if ( strTemp.size() > 0 )
				{
					strYNow = strTemp;
				}
				else
				{
					hRes = ::StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("File error, no Y coord found in line: %s"),ifline.c_str());
					nResult = ::MessageBox(NULL,szError,szTitle,MB_OK);
					RetCode = Failure;
					break;
				}
				listHFT2Hits.push_back(CHitCoords(strXNow,strYNow));
				cout << listHFT2Hits.back().GetStrXCoord() << " " << listHFT2Hits.back().GetStrYCoord() << endl;
			}
		}

		if ( RetCode == NoError )
		{
			try
			{
				lpKnuckles = (CKnuckles*) new CKnuckles(&listHFT1Hits,&listHFT2Hits);
			}
			catch (exception& e)
			{
				ReportError(::GetLastError());
				RetCode = Failure;
			}
		}

		if ( RetCode == NoError )
		{
			lpstrKnuckles = (std::string*)new std::string();
			for ( knucklesit = lpKnuckles->GetpKnucklesList()->begin(); knucklesit != lpKnuckles->GetpKnucklesList()->end(); knucklesit++ )
			{
				*lpstrKnuckles += "G31 " +
				knucklesit->GetpT1FirstHit()->GetStrXCoord() +
				" " +
				knucklesit->GetpT1FirstHit()->GetStrYCoord() +
				" " +
				strHFT1 +
				strnl +
				knucklesit->GetpT1SecondHit()->GetStrXCoord() +
				" " +
				knucklesit->GetpT1FirstHit()->GetStrYCoord() +
				strnl +
				"G31 " +
				knucklesit->GetpT2Hit()->GetStrXCoord() +
				" " +
				knucklesit->GetpT2Hit()->GetStrYCoord() +
				" " +
				strHFT2.c_str() +
				strnl;
			}
			blah.write(lpstrKnuckles->c_str(),lpstrKnuckles->size());
		}

		if ( RetCode == NoError )
		{
			szKnuckles = (TCHAR*)new TCHAR[lpstrKnuckles->size()+1];
			pvTemp = ::SecureZeroMemory((PVOID)szKnuckles,(SIZE_T)((lpstrKnuckles->size()+1)*sizeof(TCHAR)));
			nResult = ::MultiByteToWideChar(CP_ACP,
				MB_PRECOMPOSED,
				lpstrKnuckles->c_str(),
				lpstrKnuckles->size(),
				szKnuckles,
				lpstrKnuckles->size());
			*lpwstrKnuckles = (std::wstring*)new std::wstring(szKnuckles);
		}

		if ( (RetCode == NoError) && lpstrPostKnuckles )
		{
			blah.write(lpstrPostKnuckles->c_str(),lpstrPostKnuckles->size());
		}

		if ( NULL != pchBuffer )
		{
			delete[] pchBuffer;
			pchBuffer = NULL;
		}

		if ( NULL != lpstrPostKnuckles )
		{
			delete lpstrPostKnuckles;
			lpstrPostKnuckles = NULL;
		}

		if ( NULL != szKnuckles )
		{
			delete[] szKnuckles;
			szKnuckles = NULL;
		}
		if ( NULL != lpstrKnuckles )
		{
			delete lpstrKnuckles;
			lpstrKnuckles = NULL;
		}

		if ( NULL != lpKnuckles )
		{
			delete lpKnuckles;
			lpKnuckles = NULL;
		}

		if ( NULL != pchinline )
		{
			delete[] pchinline;
			pchinline = NULL;
		}
		if ( NULL != lpinfile )
		{
			if ( lpinfile->is_open() )
			{
				try
				{
					lpinfile->close();
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					RetCode = Failure;
				}
			}
		}
		if ( NULL != lpinfile )
		{
			delete lpinfile;
			lpinfile = NULL;
		}
		if ( NULL != pchFileName )
		{
			delete[] pchFileName;
			pchFileName = NULL;
		}
		if ( blah.is_open() )
		{
			blah.close();
		}
		if ( is.is_open() )
		{
			is.close();
		}
		return RetCode;
	}
	else
	{
		nResult = ::MessageBox(NULL,_T("File not open"),szTitle,MB_OK);
	}
	if ( NULL != pchFileName )
	{
		delete[] pchFileName;
		pchFileName = NULL;
	}
	return RetCode;
}


