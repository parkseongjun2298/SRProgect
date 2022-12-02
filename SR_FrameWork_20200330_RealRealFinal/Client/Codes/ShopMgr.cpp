#include "stdafx.h"
#include "ShopMgr.h"

#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"Shop.h"


CShopMgr::CShopMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CShopMgr::~CShopMgr()
{
	Release();
}

int CShopMgr::Update()
{

	ENGINE::CGameObject::Update();
	m_pShop->Update();

	return NO_EVENT;
}

void CShopMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pShop->LateUpdate();
}

void CShopMgr::Render()
{
	m_pShop->Render();
}

HRESULT CShopMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	
																												  
	m_pShop = CShop::Create(m_pGraphicDev, m_tInfo.vPos+ D3DXVECTOR3{ 0.f,2.6f,0.f }, m_tInfo.vSize+D3DXVECTOR3{4.f,4.f,4.f});


	
	


	return  S_OK;;
}

HRESULT CShopMgr::AddComponent()
{
	

	return S_OK;
}

void CShopMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	
	ENGINE::SafeDelete(m_pShop);
	
	
}

ENGINE::SAVE_INFO CShopMgr::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 7;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CShopMgr * CShopMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShopMgr* pInstance = new CShopMgr(pGraphicDev);
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
