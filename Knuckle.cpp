#include "StdAfx.h"
#include "Knuckle.h"

CKnuckle::CKnuckle(void)
{
	m_pcoordsT1FirstHit = NULL;
	m_pcoordsT1SecondHit = NULL;
	m_pcoordsT2Hit = NULL;
}
CKnuckle::CKnuckle(CHitCoords* pT1FirstHit,CHitCoords* pT1SecondHit,CHitCoords* pT2Hit) : m_pcoordsT1FirstHit(pT1FirstHit),\
		m_pcoordsT1SecondHit(pT1SecondHit),\
		m_pcoordsT2Hit(pT2Hit)
{
}

CHitCoords* CKnuckle::GetpT1FirstHit() const
{
	return this->m_pcoordsT1FirstHit;
}

CHitCoords* CKnuckle::GetpT1SecondHit() const
{
	return this->m_pcoordsT1SecondHit;
}

CHitCoords* CKnuckle::GetpT2Hit() const
{
	return this->m_pcoordsT2Hit;
}

CKnuckle::~CKnuckle(void)
{
}
