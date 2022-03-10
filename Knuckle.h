#pragma once
#include "stdafx.h"
#include "HitCoords.h"
using namespace std;

class CKnuckle
{
public:
	CKnuckle(void);
	CKnuckle(CHitCoords* pT1FirstHit,CHitCoords* pT1SecondHit,CHitCoords* pT2Hit);
	CHitCoords* GetpT1FirstHit() const;
	CHitCoords* GetpT1SecondHit() const;
	CHitCoords* GetpT2Hit() const;
	~CKnuckle(void);
private:
	CHitCoords* m_pcoordsT1FirstHit;
	CHitCoords* m_pcoordsT1SecondHit;
	CHitCoords* m_pcoordsT2Hit;
};
