#pragma once

#ifndef __TIMEMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	float GetDeltaTime();

public:
	void Update();

private:
	LARGE_INTEGER	m_OldTime1;
	LARGE_INTEGER	m_OldTime2;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CPUTick;

	float			m_fDeltaTime;
};

END

#define __TIMEMGR_H__
#endif