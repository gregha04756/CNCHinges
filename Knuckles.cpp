#include "StdAfx.h"
#include "Knuckles.h"

CKnuckles::CKnuckles(void)
{
	m_plistT1Hits = NULL;
	m_plistT2Hits = NULL;
	m_plistKnuckles = NULL;
}

CKnuckles::CKnuckles(std::list<CHitCoords>* plistT1Hits,std::list<CHitCoords>* plistT2Hits) : m_plistT1Hits(plistT1Hits),
		m_plistT2Hits(plistT2Hits)
{
	int i;
	double dTemp;
	double dX1Temp;
	double dX2Temp;
	double dY1Temp;
	double dY2Temp;
	CKnuckle* pk;
	std::list<CHitCoords>::iterator hcit11;
	std::list<CHitCoords>::iterator hcit12;
	std::list<CHitCoords>::iterator hcit2;
	try
	{
		m_plistKnuckles = (std::list<CKnuckle>*)new std::list<CKnuckle>;
	}
	catch (std::bad_alloc &ba)
	{
		throw ba;
	}
	hcit11 = plistT1Hits->begin();
	hcit12 = plistT2Hits->begin();
	for ( hcit2 = plistT2Hits->begin(); hcit2 != plistT2Hits->end(); hcit2++ )
	{
		for ( hcit11 = plistT1Hits->begin(); hcit11 != plistT1Hits->end(); hcit11++ )
		{
			dY1Temp = hcit11->GetDblYCoord();
			dY2Temp = hcit2->GetDblYCoord();
			dTemp = ::abs(dY1Temp-dY2Temp);
			if (  dTemp < TOL )
			{
				dX1Temp = hcit11->GetDblXCoord();
				dX2Temp = hcit2->GetDblXCoord();
				dTemp = ::abs(T11DIFF-(dX1Temp-dX2Temp));
				if ( dTemp < TOL )
				{
					break;
				}
			}
		}

		for ( hcit12 = plistT1Hits->begin(); hcit12 != plistT1Hits->end(); hcit12++ )
		{
			if ( ::abs(hcit12->GetDblYCoord()-hcit2->GetDblYCoord()) < TOL )
			{
				if ( ::abs(T12DIFF-(hcit12->GetDblXCoord()-hcit2->GetDblXCoord())) < TOL )
				{
					break;
				}
			}
		}
		
		pk = (CKnuckle*)new CKnuckle(&(*hcit11),&(*hcit12),&(*hcit2));
		this->m_plistKnuckles->push_back(*pk);
	}
}

int CKnuckles::GetKnucklesCount()
{
	return (int)this->m_plistKnuckles->size();
}

std::list<CKnuckle>* CKnuckles::GetpKnucklesList()
{
	return this->m_plistKnuckles;
}

CKnuckles::~CKnuckles(void)
{
	if ( NULL != m_plistKnuckles )
	{
		delete m_plistKnuckles;
		m_plistKnuckles = NULL;
	}
}
