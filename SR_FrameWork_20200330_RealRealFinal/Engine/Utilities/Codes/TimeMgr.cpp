#include "TimeMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fDeltaTime(0.f)
{
	// 하드웨어가 가지고 있는 고해상도 타이머 값을 얻어오는 함수.
	// 여기서 타이머 값은 CPU가 얼마나 진동하고있는지 누적한 값이다.
	QueryPerformanceCounter(&m_OldTime1);
	QueryPerformanceCounter(&m_OldTime2);
	QueryPerformanceCounter(&m_CurTime);

	// CPU의 주파수를 얻어오는 함수. 1초 동안 CPU가 얼마나 진동하는가?
	QueryPerformanceFrequency(&m_CPUTick);
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDeltaTime()
{
	return m_fDeltaTime; // 한 프레임당 걸리는 시간.
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurTime);

	// 주기적으로 주파수 갱신.
	if (m_CurTime.QuadPart - m_OldTime2.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTime2 = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime1.QuadPart) / m_CPUTick.QuadPart;
	m_OldTime1 = m_CurTime;
}
