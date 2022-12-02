#include "stdafx.h"
#include "Mouse.h"
#include "CollMouse.h"
#include "CameraObserver.h"
#include "GameObject.h"
#include "Transform.h"
#include "TileMgr.h"
#include "Tile.h"

#include "Viking.h"
#include "SpinHorse.h"
#include "FerrisWheel.h"
#include "GyroDrop.h"
#include "RollerCoaster.h"
#include "SpinTeaCup.h"
#include "GyroSwing.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	// Observer
	m_pCameraObserver = CCameraObserver::Create();
	ENGINE::CSubjectMgr::GetInstance()->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_pCameraObserver));

	//Collision Mouse
	m_pCollMouse = CCollMouse::Create(m_pCameraObserver);
	NULL_CHECK_MSG(m_pCollMouse, L"CollMouse Create Failed");
}

void CMouse::Update()
{
	/*
	if (m_pMouse == nullptr)
		return;

	D3DXVECTOR3 vPickPos = {};
	bool		bPick = false;

	bPick = dynamic_cast<CCollMouse*>(m_pCollMouse)->PickTerrain(&vPickPos, ENGINE::LAYER_OBJECT, L"Terrain");
	if (bPick)
	{
		if (m_pMouse != nullptr)
		{
			Vec3 vPos = dynamic_cast<ENGINE::CTransform*>(m_pMouse->GetComponent(L"Transform"))->GetPos();
			vPickPos.y = vPos.y;
			dynamic_cast<ENGINE::CTransform*>(m_pMouse->GetComponent(L"Transform"))->SetPos(vPickPos);
		}
		if (m_pTile != nullptr)
			dynamic_cast<ENGINE::CTransform*>(m_pTile->GetComponent(L"Transform"))->SetPos(vPickPos);
	}
	*/
}

void CMouse::LateUpdate()
{
}

void CMouse::Render()
{
	/*
	if (m_pTile != nullptr)
		m_pTile->Render();
	if (m_pMouse != nullptr)
		m_pMouse->Render();
	*/
}

void CMouse::Release()
{
	ENGINE::CSubjectMgr::GetInstance()->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_pCameraObserver));
	ENGINE::SafeDelete(m_pCollMouse);
	ENGINE::SafeDelete(m_pMouse);
	ENGINE::SafeDelete(m_pTile);
}

void CMouse::CreateMouse(wstring wstrTag, int iIdx)
{
	ENGINE::SafeDelete(m_pMouse);
	ENGINE::SafeDelete(m_pTile);
	if (wstrTag == L"Texture_Tile")
	{
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), wstrTag, iIdx);
	}
	else if (wstrTag == L"Texture_VikingUi")
	{
		m_pMouse = CViking::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 13.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f);
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), L"Texture_Tile", iIdx, 5.f);
	}
	else if (wstrTag == L"Texture_SpinHorseUi")
	{
		m_pMouse = CSpinHorse::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f);
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), L"Texture_Tile", iIdx, 5.f);
	}
	else if (wstrTag == L"Texture_GyroDropUi")
	{
		m_pMouse = CGyroDrop::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 20.f, 0.f), { 1.f, 1.f, 1.f, });
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), L"Texture_Tile", iIdx, 5.f);
	}
	else if (wstrTag == L"Texture_RollerCoasterUi")
	{
		m_pMouse = CRollerCoaster::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f);
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), L"Texture_Tile", iIdx, 5.f);
	}
	else if (wstrTag == L"Texture_TeaCupUi")
	{
		m_pMouse = CSpinTeaCup::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 3.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f);
		m_pTile = CTile::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f), L"Texture_Tile", iIdx, 5.f);
	}
	else if (wstrTag == L"Texture_FerrisUi")
	{
		m_pMouse = CFerrisWheel::Create(m_pGraphicDev, Vec3(0.f, 0.f, 0.f) + Vec3(0.f, 43.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f);
		m_pTile = CTile::Create(m_pGraphicDev, {0.f, 0.f, 0.f}, L"Texture_Tile", iIdx, 5.f);
	}
}
