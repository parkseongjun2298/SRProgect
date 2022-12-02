#include "stdafx.h"
#include "CottenCandyMgr.h"

#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"CottenCandyShop.h"


CCottenCandyMgr::CCottenCandyMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CCottenCandyMgr::~CCottenCandyMgr()
{
	Release();
}

int CCottenCandyMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pCottenCandtShop->Update();

	return NO_EVENT;
}

void CCottenCandyMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pCottenCandtShop->LateUpdate();
}

void CCottenCandyMgr::Render()
{
	m_pCottenCandtShop->Render();
}

HRESULT CCottenCandyMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	
																												  
	m_pCottenCandtShop = CCottenCandyShop::Create(m_pGraphicDev, m_tInfo.vPos+ D3DXVECTOR3{ 0.f,2.6f,0.f }, m_tInfo.vSize+D3DXVECTOR3{4.f,4.f,4.f});


	
	


	return  S_OK;;
}

HRESULT CCottenCandyMgr::AddComponent()
{
	

	return S_OK;
}

void CCottenCandyMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	
	ENGINE::SafeDelete(m_pCottenCandtShop);
	
	
}

ENGINE::SAVE_INFO CCottenCandyMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 9;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CCottenCandyMgr * CCottenCandyMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CCottenCandyMgr* pInstance = new CCottenCandyMgr(pGraphicDev);
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
