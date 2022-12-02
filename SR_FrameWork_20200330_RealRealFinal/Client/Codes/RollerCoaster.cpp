#include "stdafx.h"
#include "RollerCoaster.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "PipeLine.h"
#include "Train.h"
#include "Customer.h"
#include "Queue.h"
#include "DeadObj.h"
#include "Rail.h"

CRollerCoaster::CRollerCoaster(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),	
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
	
}

CRollerCoaster::~CRollerCoaster()
{
	Release();
}

int CRollerCoaster::Update()
{
	if (RAIL_END == m_pTrain->Update())
	{
		CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
		ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
		if(nullptr != pCustomer)
			m_pTrain->PushSeat(0, dynamic_cast<CCustomer*>(pCustomer));
	}
	return ENGINE::CGameObject::Update();
}

void CRollerCoaster::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pTrain->LateUpdate();
}

void CRollerCoaster::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	//±âÂ÷ ·»´õ
	m_pTrain->Render();
}

HRESULT CRollerCoaster::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_pTrain = CTrain::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f, }, this);

	m_eRideType = ENGINE::RIDE_ROLLERCOASTER;

	return S_OK;
}

HRESULT CRollerCoaster::Initialize(ENGINE::ROLLER_INFO tInfo)
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_pTrain = CTrain::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f, }, this);
	m_pTrain->SetRailInfo(tInfo);
	m_eRideType = ENGINE::RIDE_ROLLERCOASTER;

	return S_OK;
}

HRESULT CRollerCoaster::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Queue
	pComponent = CQueue::Create(m_vPos, L"RollerCoaster");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CRollerCoaster::Release()
{
	ENGINE::SafeDelete(m_pTrain);
}

ENGINE::SAVE_INFO CRollerCoaster::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo = ENGINE::CGameObject::GetSaveInfo();
	tInfo.iRideTag = 4;
	return tInfo;
}

ENGINE::ROLLER_INFO CRollerCoaster::GetRollerInfo()
{
	ENGINE::ROLLER_INFO tInfo;
	tInfo.vPos = m_vPos;
	tInfo.vSize = m_vSize;
	tInfo.fAngleY = m_fAngle;
	ZeroMemory(&tInfo.tRailInfo, sizeof(tInfo.tRailInfo));
	for (int i = 0; i < m_pTrain->GetRail()->size(); ++i)
	{
		tInfo.tRailInfo[i] = dynamic_cast<CRail*>((*m_pTrain->GetRail())[i])->GetRailInfo();
	}
	return tInfo;
}

CRollerCoaster* CRollerCoaster::Create(LPDIRECT3DDEVICE9 pGraphicDev, Vec3 vPos, Vec3 vSize, float fAngleY, bool bCreateMode, ENGINE::ROLLER_INFO tInfo)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CRollerCoaster* pInstance = new CRollerCoaster(pGraphicDev);
	pInstance->m_vPos = vPos;
	pInstance->m_vSize = vSize;
	pInstance->m_fAngle = fAngleY;

	if (!bCreateMode)
	{
		if (FAILED(pInstance->Initialize()))
		{
			ENGINE::SafeDelete(pInstance);
			return nullptr;
		}
	}
	else
	{
		if (FAILED(pInstance->Initialize(tInfo)))
		{
			ENGINE::SafeDelete(pInstance);
			return nullptr;
		}
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}
