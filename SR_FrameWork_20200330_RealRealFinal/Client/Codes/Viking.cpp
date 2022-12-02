#include "stdafx.h"
#include "Viking.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"VikingLeg.h"
#include"VikingShip.h"
#include "Queue.h"
#include "DeadObj.h"
#include "Customer.h"
CViking::CViking(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{
}


CViking::~CViking()
{
	Release();
}

int CViking::Update()
{
	for (auto& iter : m_VecVikingLeg)
		iter->Update();
	if (!m_VikingShip->GetPlaying())
	{
		for (int i = 0; i < 10; ++i)
		{
			CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
			ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
			if (nullptr != pCustomer)
				m_VikingShip->PushSeat(i, dynamic_cast<CCustomer*>(pCustomer));
		}
		m_VikingShip->PlayViking();
	}
	m_VikingShip->Update();

	return ENGINE::CGameObject::Update();
}

void CViking::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	for (auto& iter : m_VecVikingLeg)
		iter->LateUpdate();
	m_VikingShip->LateUpdate();
}

void CViking::Render()
{

	for (auto& iter : m_VecVikingLeg)
		iter->Render();
	m_VikingShip->Render();
}

HRESULT CViking::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_VikingShip = CVikingShip::Create(m_pGraphicDev, m_tInfo.vPos , m_tInfo.vSize , 270.f);
	NULL_CHECK_RETURN(m_VikingShip, E_FAIL);


	m_VecVikingLeg.push_back(CVikingLeg::Create(m_pGraphicDev, m_tInfo.vPos , m_tInfo.vSize , 0, 150));

	m_VecVikingLeg.push_back(CVikingLeg::Create(m_pGraphicDev, m_tInfo.vPos , m_tInfo.vSize , 0, 210));


	m_eRideType = ENGINE::RIDE_VIKING;

	return  S_OK;;
}

HRESULT CViking::AddComponent()
{

	ENGINE::CComponent* pComponent = nullptr;
	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"Viking");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));


	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CViking::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	for_each(m_VecVikingLeg.begin(), m_VecVikingLeg.end(), ENGINE::SafeDelete<CVikingLeg*>);


	ENGINE::SafeDelete(m_VikingShip);


}

D3DXVECTOR3 CViking::GetPos()
{
	return m_tInfo.vPos;
}

ENGINE::SAVE_INFO CViking::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 0;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CViking * CViking::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CViking* pInstance = new CViking(pGraphicDev);
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
