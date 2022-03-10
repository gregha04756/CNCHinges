#pragma once
#include "stdafx.h"
#include "Knuckle.h"
#include "HitCoords.h"

using namespace std;

#define T11DIFF 0.465L
#define T12DIFF 0.103L
#define TOL 0.005L

class CKnuckles
{
public:
	CKnuckles(void);
	CKnuckles(std::list<CHitCoords>* plistT1Hits,std::list<CHitCoords>* plistT2Hits);
	int GetKnucklesCount();
	std::list<CKnuckle>* GetpKnucklesList();
	~CKnuckles(void);
private:
	std::list<CHitCoords>* m_plistT1Hits;
	std::list<CHitCoords>* m_plistT2Hits;
	std::list<CKnuckle>* m_plistKnuckles;
};
