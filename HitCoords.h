#pragma once
#include "stdafx.h"

using namespace std;

class CHitCoords
{
public:
	CHitCoords(void);
	CHitCoords(std::string x,std::string y) : m_szXCoord(x), m_szYCoord(y){}
	std::string GetStrXCoord();
	std::string GetStrYCoord();
	double GetDblXCoord();
	double GetDblYCoord();
	~CHitCoords(void);
private:
	std::string m_szXCoord;
	std::string m_szYCoord;
};
