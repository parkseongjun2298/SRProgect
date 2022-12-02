#include "stdafx.h"
#include "FlowerMgr.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"Flower.h"


CFlowerMgr::CFlowerMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CFlowerMgr::~CFlowerMgr()
{
	Release();
}

int CFlowerMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pFlower->Update();

	return NO_EVENT;
}

void CFlowerMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pFlower->LateUpdate();
}

void CFlowerMgr::Render()
{
	m_pFlower->Render();
}

HRESULT CFlowerMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_pFlower = CFlower::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 0.f,1.f,0.f }, m_tInfo.vSize );
	
	
	return  S_OK;;
}

HRESULT CFlowerMgr::AddComponent()
{
	return S_OK;
}

void CFlowerMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	ENGINE::SafeDelete(m_pFlower);


}

ENGINE::SAVE_INFO CFlowerMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 10;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CFlowerMgr * CFlowerMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFlowerMgr* pInstance = new CFlowerMgr(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos + D3DXVECTOR3{ TERRAIN_ITV / 2.f,0.f,TERRAIN_ITV / 2.f };
	
	pInstance->m_tInfo.vSize = vSize;
	pInstance->m_fAngleY = fAngleY;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}

D3DXVECTOR3 CFlowerMgr::GetPos()
{
		return m_tInfo.vPos;
}
