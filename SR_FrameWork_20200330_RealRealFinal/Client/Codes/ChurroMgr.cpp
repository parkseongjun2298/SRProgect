#include "stdafx.h"
#include "ChurroMgr.h"

#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"ChurroShop.h"


CChurroMgr::CChurroMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CChurroMgr::~CChurroMgr()
{
	Release();
}

int CChurroMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pChurroShop->Update();

	return NO_EVENT;
}

void CChurroMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pChurroShop->LateUpdate();
}

void CChurroMgr::Render()
{
	m_pChurroShop->Render();
}

HRESULT CChurroMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	
																												  
	m_pChurroShop = CChurroShop::Create(m_pGraphicDev, m_tInfo.vPos+ D3DXVECTOR3{ 0.f,2.6f,0.f }, m_tInfo.vSize+D3DXVECTOR3{4.f,4.f,4.f});


	
	


	return  S_OK;;
}

HRESULT CChurroMgr::AddComponent()
{
	

	return S_OK;
}

void CChurroMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	
	ENGINE::SafeDelete(m_pChurroShop);
	
	
}

CChurroMgr * CChurroMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CChurroMgr* pInstance = new CChurroMgr(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
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

ENGINE::SAVE_INFO CChurroMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 8;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}
