#include "stdafx.h"
#include "RedFlowerMgr.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"RedFlower.h"


CRedFlowerMgr::CRedFlowerMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CRedFlowerMgr::~CRedFlowerMgr()
{
	Release();
}

int CRedFlowerMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pFlower->Update();

	return NO_EVENT;
}

void CRedFlowerMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pFlower->LateUpdate();
}

void CRedFlowerMgr::Render()
{
	m_pFlower->Render();
}

HRESULT CRedFlowerMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_pFlower = CRedFlower::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 0.f,1.f,0.f }, m_tInfo.vSize );
	
	
	return  S_OK;;
}

HRESULT CRedFlowerMgr::AddComponent()
{
	return S_OK;
}

void CRedFlowerMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	ENGINE::SafeDelete(m_pFlower);


}

ENGINE::SAVE_INFO CRedFlowerMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 11;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CRedFlowerMgr * CRedFlowerMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CRedFlowerMgr* pInstance = new CRedFlowerMgr(pGraphicDev);
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

D3DXVECTOR3 CRedFlowerMgr::GetPos()
{
		return m_tInfo.vPos;
}
