#include "DeadObj.h"
#include "PipeLine.h"

USING(ENGINE)

CDeadObj::CDeadObj()
	: m_bDead(false)
{	
	
}


CDeadObj::~CDeadObj()
{
}

int CDeadObj::Update()
{
	if (m_bDead)
		return DEAD_OBJ;
	return NO_EVENT;
}

CDeadObj * CDeadObj::Create()
{
	return new CDeadObj;
}
