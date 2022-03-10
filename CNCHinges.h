#pragma once

#include "resource.h"

#define HINGEFORMT1 "T4"
#define HINGEFORMT2 "T13"

enum CNCCodes {NoError,NoHFT1,NoHFT2,Failure};

typedef struct tagHFTSelectParms {std::wstring* lpwstrHFT1;std::wstring* lpwstrHFT2;} HFTSELECTPARMS,*LPHFTSELECTPARMS;

using namespace std;
