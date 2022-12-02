#include "stdafx.h"
#include "YellowFlowerMgr.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"YellowFlower.h"


CYellowFlowerMgr::CYellowFlowerMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CYellowFlowerMgr::~CYellowFlowerMgr()
{
	Release();
}

int CYellowFlowerMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pFlower->Update();

	return NO_EVENT;
}

void CYellowFlowerMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pFlower->LateUpdate();
}

void CYellowFlowerMgr::Render()
{
	m_pFlower->Render();
}

HRESULT CYellowFlowerMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_pFlower = CYellowFlower::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 0.f,1.f,0.f }, m_tInfo.vSize );
	
	
	return  S_OK;;
}

HRESULT CYellowFlowerMgr::AddComponent()
{
	return S_OK;
}

void CYellowFlowerMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	ENGINE::SafeDelete(m_pFlower);


}

ENGINE::SAVE_INFO CYellowFlowerMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 12;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CYellowFlowerMgr * CYellowFlowerMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CYellowFlowerMgr* pInstance = new CYellowFlowerMgr(pGraphicDev);
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

D3DXVECTOR3 CYellowFlowerMgr::GetPos()
{
		return m_tInfo.vPos;
}
