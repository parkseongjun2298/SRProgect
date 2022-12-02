#pragma once

#ifndef __FRAME_H__

#include "Component.h"

BEGIN(ENGINE)

class CTimeMgr;
class ENGINE_DLL CFrame :
	public CComponent
{
private:
	explicit CFrame();
public:
	virtual ~CFrame();

public:
	virtual void LateUpdate() override;

public:
	WORD	GetCurrentFrame() { return (WORD)m_fFrameStart; }
	void	SetFrameExecute(bool bFrameExecute) { m_bFrameExecute = bFrameExecute; }
	void	ChangeFrame(float fFrameStart, WORD dwFrameMax, float fCyclePerSecond);

public:
	static CFrame* Create(float fFrameStart, WORD dwFrameMax, float fCyclePerSecond, bool bFrameExecute);

private:
	bool		m_bFrameExecute;
	CTimeMgr*	m_pTimeMgr;
	float		m_fFrameStart;
	WORD		m_dwFrameMax;
	float		m_fFrameSpeed;
};

END

#define __FRAME_H__
#endif