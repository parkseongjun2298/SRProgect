#include "stdafx.h"
#include "UiBase.h"


CUiBase::CUiBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev)
{
}


CUiBase::~CUiBase()
{
}

void CUiBase::KeyInput()
{
}

CUiBase * CUiBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiBase* pInstance = new CUiBase(pGraphicDev);
	pInstance->m_vPos = D3DXVECTOR3(vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z);
	pInstance->m_vSize = vSize;
	pInstance->m_wstrTextureTag = wstrTextureTag;
	pInstance->m_iTextureIndex = 0;
	pInstance->m_bMgr = true;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
