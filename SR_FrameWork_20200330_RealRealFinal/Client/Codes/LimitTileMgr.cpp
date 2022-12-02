#include "stdafx.h"
#include "LimitTileMgr.h"

#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"LimitTile.h"


CLimitTileMgr::CLimitTileMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CLimitTileMgr::~CLimitTileMgr()
{
	Release();
}

int CLimitTileMgr::Update()
{

	ENGINE::CGameObject::Update();
	for (auto& iter : m_vTile)
	{
		iter->Update();
	}

	return NO_EVENT;
}

void CLimitTileMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	for (auto& iter : m_vTile)
	{
		iter->LateUpdate();
	}
}

void CLimitTileMgr::Render()
{
	for (auto& iter : m_vTile)
	{
		iter->Render();
	}
}

HRESULT CLimitTileMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_vTile.push_back(CLimitTile::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize, 0, 90, 90));
	m_vTile.push_back(CLimitTile::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 160.f,0.f,0.f }, m_tInfo.vSize, 0, 90, 90));


	m_vTile.push_back(CLimitTile::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 0.f,0.f,160.f }, m_tInfo.vSize, 0, 180, 90));
	m_vTile.push_back(CLimitTile::Create(m_pGraphicDev, m_tInfo.vPos + D3DXVECTOR3{ 0.f,0.f,0.f }, m_tInfo.vSize, 0, 180, 90));




	return  S_OK;;
}

HRESULT CLimitTileMgr::AddComponent()
{


	return S_OK;
}

void CLimitTileMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	for_each(m_vTile.begin(), m_vTile.end(), ENGINE::SafeDelete<CLimitTile*>);


}

CLimitTileMgr * CLimitTileMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CLimitTileMgr* pInstance = new CLimitTileMgr(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}
