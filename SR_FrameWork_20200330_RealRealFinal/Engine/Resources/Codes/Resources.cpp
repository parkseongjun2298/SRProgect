#include "Resources.h"

USING(ENGINE)

CResources::CResources(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev),
	m_pRefCnt(new DWORD(1))
{
}


CResources::~CResources()
{
	if (0 == *m_pRefCnt)
		SafeDelete(m_pRefCnt);	
}
