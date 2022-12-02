#include "Frame.h"
#include "TimeMgr.h"

USING(ENGINE)

CFrame::CFrame()
	: m_pTimeMgr(CTimeMgr::GetInstance())
	, m_fFrameStart(0), m_dwFrameMax(0)
	, m_fFrameSpeed(0.f)
{
}


CFrame::~CFrame()
{
}

void CFrame::LateUpdate()
{
	if (!m_bFrameExecute)
		return;
	m_fFrameStart += m_fFrameSpeed * m_dwFrameMax * m_pTimeMgr->GetDeltaTime();
	if (m_fFrameStart >= m_dwFrameMax)
		m_fFrameStart -= m_dwFrameMax;
}

void CFrame::ChangeFrame(float fFrameStart, WORD dwFrameMax, float fCyclePerSecond)
{
	m_fFrameStart = fFrameStart;
	m_dwFrameMax = dwFrameMax;
	m_fFrameSpeed = fCyclePerSecond;
}

CFrame * CFrame::Create(float fFrameStart, WORD dwFrameMax, float fCyclePerSecond, bool bFrameExecute)
{
	CFrame* pInstance = new CFrame;
	pInstance->m_fFrameStart = fFrameStart;
	pInstance->m_dwFrameMax = dwFrameMax;
	pInstance->m_fFrameSpeed = fCyclePerSecond;
	pInstance->m_bFrameExecute = bFrameExecute;
	return pInstance;
}