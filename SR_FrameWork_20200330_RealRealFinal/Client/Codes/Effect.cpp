#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev)
{
}


CEffect::~CEffect()
{
}

void CEffect::Render()
{
}

HRESULT CEffect::Initialize()
{
	return S_OK;
}

HRESULT CEffect::AddComponent()
{
	return S_OK;
}

void CEffect::Release()
{
}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CEffect* pInstance = new CEffect(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}
