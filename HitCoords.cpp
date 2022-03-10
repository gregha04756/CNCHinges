#include "StdAfx.h"
#include "HitCoords.h"

CHitCoords::CHitCoords(void)
{
	m_szXCoord = "0.0";
	m_szYCoord = "0.0";
}

CHitCoords::~CHitCoords(void)
{
}

std::string CHitCoords::GetStrXCoord()
{
	return m_szXCoord;
}

std::string CHitCoords::GetStrYCoord()
{
	return m_szYCoord;
}

double CHitCoords::GetDblXCoord()
{
	return ::atof(m_szXCoord.substr(1).c_str());
}

double CHitCoords::GetDblYCoord()
{
	return ::atof(m_szYCoord.substr(1).c_str());
}