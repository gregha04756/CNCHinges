// trash37.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HitCoords.h"
#include "Knuckle.h"
#include "Knuckles.h"
using namespace std;

#define MAX_LOADSTRING 100
#define NUMFILELINES 3
#define NUMPARMS 0x22


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
	int nResult = ::MessageBox(NULL,(LPWSTR)szErrMsg,_T("Trash35"),MB_OK);
	if ( lpErrMsg )
	{
		::LocalFree(lpErrMsg);
		lpErrMsg = NULL;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bSuccess = true;
	char cch;
	int i;
	int nResult;
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
	ifstream* lpinfile;
	std::string ifline;
	std::string strXNow;
	std::string strYNow;
	std::string strTemp;
	size_t nXOffset;
	size_t nYOffset;
	DWORD dwError;
	char nl[] = {0x0d,0x0a};
	const std::string strnl(nl,2);

	std::list<CHitCoords> listHFT1Hits;
	std::list<CHitCoords> listHFT2Hits;

	CKnuckles* lpKnuckles = NULL;
	std::string* lpstrKnuckles;

	const std::string strHFT1 = "T4";
	const std::string strHFT2 = "T13";

	std::list<CKnuckle>::iterator knucklesit;
	std::filebuf* fb;
	char* pchBuffer;
	std::string* psfc;
	std::ofstream blah;
	std::ifstream is;
	std::wstring *lpwstrKnuckles;

	try
	{
		lpinfile = (std::ifstream*)new ifstream();
	}
	catch (std::bad_alloc ba)
	{
		cout << "Memory error: " << ba.what() << "\n";
		bSuccess = false;
	}
	if ( bSuccess )
	{
		lpinfile->exceptions(ifstream::failbit|ifstream::badbit);
		try
		{
			lpinfile->open("Tx 20ga2.PRO",ifstream::in);
		}
		catch (ifstream::failure fof)
		{
			ReportError(::GetLastError());
			cout << "File open error: " << fof.what() << "\n";
			bSuccess = false;
		}
	}
	if ( bSuccess )
	{
		if ( lpinfile->is_open() )
		{
			cout << "File open\n";
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
					break;
				}
				linecnt++;
			}
		}
		if ( bSuccess )
		{
			cout << "Number of lines: " << linecnt << endl;
		}
		if ( bSuccess )
		{
			try
			{
				lpinfile->seekg(ifstream::beg);					/* Seek to beginning of file			*/
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
			}
		}
		if ( bSuccess )										/* Determine location of first Hinge Form	*/
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
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
					cout << "Exception: " << e.what() << endl;
					break;
				}
			}
		}
		
		
		is.open ("Tx 20ga2.PRO",ios::binary);
		fb = is.rdbuf();
		fb->pubseekpos(0,ios::in);
		pchBuffer = (char*) new char[nHFT1InitPos];
		fb->sgetn(pchBuffer,nHFT1InitPos);

		blah.open("blah.txt",ios::binary);
		psfc = new std::string(pchBuffer);
		blah.write(psfc->c_str(),nHFT1InitPos);
		delete[] pchBuffer;
		delete psfc;

		
		
		if ( bSuccess )
		{
			cout <<"Getting " << strHFT1 << " count\n";
			try
			{
				lpinfile->seekg(nHFT1InitPos);					/* Seek to first tool 1 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
			}
		}
		if ( bSuccess )										/* Determine number of first Hinge Form	*/
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
					break;
				}
				if ( !(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/') )
				{
					nHFT1Hits = (nCurPos < nHFT2InitPos) ? ++nHFT1Hits : nHFT1Hits;
				}
				nCurPos = lpinfile->tellg();
			}
		}
		if ( bSuccess )
		{
			cout << "Number of " << strHFT1 << " hits: " << nHFT1Hits << endl;
		}
		if ( bSuccess )
		{
			cout <<"Getting " << strHFT2 << " count\n";
			try
			{
				lpinfile->seekg(nHFT2InitPos);					/* Seek to first tool 2 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
			}
		}
		if ( bSuccess )										/* Determine number of second Hinge Form	*/
		{													/* tool hits */
			nHFT2Hits = 0;
			try
			{
				getline(*lpinfile,ifline);
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
			}
			nCurPos = lpinfile->tellg();
			while ( bSuccess && !lpinfile->eof() && !(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/') )
			{
				nHFT2Hits = (!(ifline.c_str()[0] == '(') && !(ifline.c_str()[0] == '/')) ? ++nHFT2Hits : nHFT2Hits;
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
					break;
				}
			}
		}
		if ( bSuccess )
		{
			cout << "Number of " << strHFT2 << " hits: " << nHFT2Hits << endl;
		}
		if ( !(bSuccess = ((nHFT2Hits*2) == nHFT1Hits) ? true : false) )
		{
			cout << "File corrupted" << endl;
		}
		if ( bSuccess )											/* Parse for tool 1 hit X, Y coordinates */
		{
			try
			{
				lpinfile->seekg(nHFT1InitPos);					/* Seek to first tool 1 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
					break;
				}

#if 0
				if ( NULL != pchinline )
				{
					delete[] pchinline;
					pchinline = NULL;
				}
				try
				{
					pchinline = (char*) new char[ifline.size()+1];
				}
				catch (exception& e)
				{
					ReportError(::GetLastError());
					cout << "Exception: " << e.what() << endl;
					bSuccess = false;
					break;
				}
				strcpy(pchinline,ifline.c_str());
				pch = strtok(pchinline," ");
				while (pch != NULL)
				{
					cout << pch << " ";
					pch = strtok(NULL," ");
				}
				cout << endl;
#endif

				nXOffset = ifline.find("X");
				if ( nXOffset == string::npos )
				{
					nXOffset = ifline.find("x");
				}
				if ( nXOffset == string::npos )
				{
					cout << "File error, no X coord found in line: " << ifline << endl;
					bSuccess = false;
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
					cout << "File error, no X coord found in line: " << ifline << endl;
					bSuccess = false;
					break;
				}
				nYOffset = ifline.find("Y");
				if ( nYOffset == string::npos )
				{
					nYOffset = ifline.find("y");
				}
				if ( nYOffset == string::npos )
				{
					cout << "File error, no Y coord found in line: " << ifline << endl;
					bSuccess = false;
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
					cout << "File error, no Y coord found in line: " << ifline << endl;
					bSuccess = false;
					break;
				}
				listHFT1Hits.push_back(CHitCoords(strXNow,strYNow));
				cout << listHFT1Hits.back().GetStrXCoord() << " " << listHFT1Hits.back().GetStrYCoord() << endl;
			}
		}





		if ( bSuccess )											/* Parse for tool 2 hit X, Y coordinates */
		{
			try
			{
				lpinfile->seekg(nHFT2InitPos);					/* Seek to first tool 2 hit */
			}
			catch (ifstream::failure fof)
			{
				ReportError(::GetLastError());
				cout << "File error: " << fof.what() << "\n";
				bSuccess = false;
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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
					break;
				}
				nXOffset = ifline.find("X");
				if ( nXOffset == string::npos )
				{
					nXOffset = ifline.find("x");
				}
				if ( nXOffset == string::npos )
				{
					cout << "File error, no X coord found in line: " << ifline << endl;
					bSuccess = false;
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
					cout << "File error, no X coord found in line: " << ifline << endl;
					bSuccess = false;
					break;
				}
				nYOffset = ifline.find("Y");
				if ( nYOffset == string::npos )
				{
					nYOffset = ifline.find("y");
				}
				if ( nYOffset == string::npos )
				{
					cout << "File error, no Y coord found in line: " << ifline << endl;
					bSuccess = false;
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
					cout << "File error, no Y coord found in line: " << ifline << endl;
					bSuccess = false;
					break;
				}
				listHFT2Hits.push_back(CHitCoords(strXNow,strYNow));
				cout << listHFT2Hits.back().GetStrXCoord() << " " << listHFT2Hits.back().GetStrYCoord() << endl;
			}
		}


		if ( bSuccess )
		{
			try
			{
				lpKnuckles = (CKnuckles*) new CKnuckles(&listHFT1Hits,&listHFT2Hits);
			}
			catch (exception& e)
			{
				ReportError(::GetLastError());
				cout << "Exception: " << e.what() << endl;
				bSuccess = false;
			}
		}

		if ( bSuccess )
		{
			cout << "Knuckle count: " << lpKnuckles->GetKnucklesCount() << endl;
			for ( knucklesit = lpKnuckles->GetpKnucklesList()->begin(); knucklesit != lpKnuckles->GetpKnucklesList()->end(); knucklesit++ )
			{
				cout << "G31 " << knucklesit->GetpT1FirstHit()->GetStrXCoord() << " " << knucklesit->GetpT1FirstHit()->GetStrYCoord() << " " << strHFT1 << endl <<
					knucklesit->GetpT1SecondHit()->GetStrXCoord() << " " << knucklesit->GetpT1FirstHit()->GetStrYCoord()<< endl <<
					"G31 " << knucklesit->GetpT2Hit()->GetStrXCoord() << " " << knucklesit->GetpT2Hit()->GetStrYCoord() << " " << strHFT2 << endl;

			}
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
#if 0
				blah.write("G31 ",4);
				blah.write(knucklesit->GetpT1FirstHit()->GetStrXCoord().c_str(),knucklesit->GetpT1FirstHit()->GetStrXCoord().size());
				blah.write(" ",1);
				blah.write(knucklesit->GetpT1FirstHit()->GetStrYCoord().c_str(),knucklesit->GetpT1FirstHit()->GetStrYCoord().size());
				blah.write(" ",1);
				blah.write(strHFT1.c_str(),strHFT1.size());
				blah.write(nl,2);
				blah.write(knucklesit->GetpT1SecondHit()->GetStrXCoord().c_str(),knucklesit->GetpT1SecondHit()->GetStrXCoord().size());
				blah.write(knucklesit->GetpT1FirstHit()->GetStrYCoord().c_str(),knucklesit->GetpT1FirstHit()->GetStrYCoord().size());
				blah.write(nl,2);
				blah.write("G31 ",4);
				blah.write(knucklesit->GetpT2Hit()->GetStrXCoord().c_str(),knucklesit->GetpT2Hit()->GetStrXCoord().size());
				blah.write(" ",1);
				blah.write(knucklesit->GetpT2Hit()->GetStrYCoord().c_str(),knucklesit->GetpT2Hit()->GetStrYCoord().size());
				blah.write(" ",1);
				blah.write(strHFT2.c_str(),strHFT2.size());
				blah.write(nl,2);
#endif
			}
			blah.write(lpstrKnuckles->c_str(),lpstrKnuckles->size());
		}

		TCHAR* szKnuckles = (TCHAR*)new TCHAR[lpstrKnuckles->size()];
		nResult = ::MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			lpstrKnuckles->c_str(),
			lpstrKnuckles->size(),
			szKnuckles,
			lpstrKnuckles->size());
		lpwstrKnuckles = (std::wstring*) new std::wstring(szKnuckles);


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
					cout << "File error: " << fof.what() << "\n";
					bSuccess = false;
				}
			}
		}
		if ( NULL != lpinfile )
		{
			delete lpinfile;
			lpinfile = NULL;
		}
		if ( lpstrKnuckles )
		{
			delete lpstrKnuckles;
		}
		blah.close();
		is.close();
		return 0;
	}
	else
	{
		cout << "File not open\n";
	}
	return 0;
}


