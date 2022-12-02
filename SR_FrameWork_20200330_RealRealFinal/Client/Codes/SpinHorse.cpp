#include "stdafx.h"
#include "SpinHorse.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CubeCol.h"
#include "CubeTex.h"
#include "SoundMgr.h"

#include "SpinHorseHorse.h"
#include "SpinHorsePillar.h"
#include "SpinHorsePlane.h"
#include "Queue.h"
#include "DeadObj.h"

#include "Customer.h"

CSpinHorse::CSpinHorse(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pConvertVtx(nullptr), m_pOriginVtx(nullptr), m_bIsStop(false), m_fAngle(0.f), m_fPlayTime(0.f),
	m_bPlay(true)
{
	m_vecHorse.reserve(4);
	m_vecPillar.reserve(4);
	m_vecPlane.reserve(2);
}


CSpinHorse::~CSpinHorse()
{
	Release();
}

int CSpinHorse::Update()
{

	m_fPlayTime += m_pTimeMgr->GetDeltaTime();
	// 플레이 시간이 30초가 지나고 10초의 쿨타임이 지난후 다시 재가동.
	if (m_fPlayTime >= 16.f && m_bPlay == false)
	{
		for (int i = 0; i < 4; ++i)
		{
			CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
			ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
			if (nullptr != pCustomer)
				m_vecHorse[i]->PushSeat(0, dynamic_cast<CCustomer*>(pCustomer));
		}

		m_bPlay = true;
		/*CSoundMgr::GetInstance()->PlaySound(L"SpinHorseTheme.mp3", CSoundMgr::CHANNELID::RIDES);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.02f);*/
		m_fPlayTime = 0.f;
	}
	// 플레이 시간이 30초가 지났으면, 멈추게함.
	else if (m_fPlayTime >= 15.f)
	{
		// 플레이되고 최초의 한번 들어왔을때만 m_bPlay가 true일것이라 한번만에 타게함.
		if (m_bPlay)
		{
			// 탔던 손님들을 모두 내보낸다.
			for (int i = 0; i < 4; ++i)
				m_vecHorse[i]->ClearSeat(dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->GetPos());
			// 놀이기구 작동이 멈췄으므로 소리도 끈다.
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::RIDES);
		}
		m_bPlay = false;
	}
	if (m_bPlay)
	{
		SpinHorsePlay();
		for (auto& pHorse : m_vecHorse)
		{
			pHorse->Update();
		}

		for (auto& pPillar : m_vecPillar)
		{
			pPillar->Update();
		}

		for (auto& pPlane : m_vecPlane)
		{
			pPlane->Update();
		}
	}

	return ENGINE::CGameObject::Update();;
}

void CSpinHorse::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();

	for (auto& pHorse : m_vecHorse)
	{
		pHorse->LateUpdate();
	}

	for (auto& pPillar : m_vecPillar)
	{
		pPillar->LateUpdate();
	}

	for (auto& pPlane : m_vecPlane)
	{
		pPlane->LateUpdate();
	}
}

void CSpinHorse::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 8; ++i)
	//{
	//	// 로컬 -> 뷰 스페이스
	//	D3DXMATRIX matWorld;
	//	matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&matWorld);

	//	// 투영 변환전 예외 처리
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	// 뷰 -> 투영 스페이스
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	for (auto& pHorse : m_vecHorse)
	{
		pHorse->Render();
	}

	for (auto& pPillar : m_vecPillar)
	{
		pPillar->Render();
	}

	for (auto& pPlane : m_vecPlane)
	{
		pPlane->Render();
	}
}

HRESULT CSpinHorse::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	//// Horse(목마)
	CSpinHorseHorse* pHorse = CSpinHorseHorse::Create(m_pGraphicDev,
		D3DXVECTOR3(3.f, 1.f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pHorse, L"SpinHorseHorse Create Failed", E_FAIL);
	m_vecHorse.push_back(pHorse);

	pHorse = CSpinHorseHorse::Create(m_pGraphicDev,
		D3DXVECTOR3(0.f, 1.f, 3.f), this, 90.f);
	NULL_CHECK_MSG_RETURN(pHorse, L"SpinHorseHorse Create Failed", E_FAIL);
	m_vecHorse.push_back(pHorse);

	pHorse = CSpinHorseHorse::Create(m_pGraphicDev,
		D3DXVECTOR3(-3.f, 1.f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pHorse, L"SpinHorseHorse Create Failed", E_FAIL);
	m_vecHorse.push_back(pHorse);

	pHorse = CSpinHorseHorse::Create(m_pGraphicDev,
		D3DXVECTOR3(0.f, 1.f, -3.f), this, 90.f);
	NULL_CHECK_MSG_RETURN(pHorse, L"SpinHorseHorse Create Failed", E_FAIL);
	m_vecHorse.push_back(pHorse);

	//// Pilliar(기둥)
	CSpinHorsePillar* pPillar = CSpinHorsePillar::Create(m_pGraphicDev,
		D3DXVECTOR3(3.f, 9.f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pPillar, L"SpinHorsePillar Create Failed", E_FAIL);
	m_vecPillar.push_back(pPillar);

	pPillar = CSpinHorsePillar::Create(m_pGraphicDev,
		D3DXVECTOR3(0.f, 9.f, 3.f), this);
	NULL_CHECK_MSG_RETURN(pPillar, L"SpinHorsePillar Create Failed", E_FAIL);
	m_vecPillar.push_back(pPillar);

	pPillar = CSpinHorsePillar::Create(m_pGraphicDev,
		D3DXVECTOR3(-3.f, 9.f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pPillar, L"SpinHorsePillar Create Failed", E_FAIL);
	m_vecPillar.push_back(pPillar);

	pPillar = CSpinHorsePillar::Create(m_pGraphicDev,
		D3DXVECTOR3(0.f, 9.f, -3.f), this);
	NULL_CHECK_MSG_RETURN(pPillar, L"SpinHorsePillar Create Failed", E_FAIL);
	m_vecPillar.push_back(pPillar);

	//// Plane(바닥)
	// 윗바닥
	CSpinHorsePlane* pPlane = CSpinHorsePlane::Create(m_pGraphicDev,
		D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z), this);
	NULL_CHECK_MSG_RETURN(pPlane, L"SpinHorsePlane Create Failed", E_FAIL);
	m_vecPlane.push_back(pPlane);

	// 아랫바닥
	pPlane = CSpinHorsePlane::Create(m_pGraphicDev,
		D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 9.f, m_tInfo.vPos.z), this);
	NULL_CHECK_MSG_RETURN(pPlane, L"SpinHorsePlane Create Failed", E_FAIL);
	m_vecPlane.push_back(pPlane);

	//CSoundMgr::GetInstance()->PlaySound(L"SpinHorseTheme.mp3", CSoundMgr::CHANNELID::RIDES);
	//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.02f);

	m_eRideType = ENGINE::RIDE_SPINHORSE;

	return S_OK;
}

void CSpinHorse::Release()
{
	for_each(m_vecHorse.begin(), m_vecHorse.end(), ENGINE::SafeDelete<CSpinHorseHorse*>);
	for_each(m_vecPlane.begin(), m_vecPlane.end(), ENGINE::SafeDelete<CSpinHorsePlane*>);
	for_each(m_vecPillar.begin(), m_vecPillar.end(), ENGINE::SafeDelete<CSpinHorsePillar*>);

	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

HRESULT CSpinHorse::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SpinHorse");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SpinHorse Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SpinHorse");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SpinHorse Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	//m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorse Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorse Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	m_pSubjectMgr->Subscribe(L"Camera", pObserver);

	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"SpinHorse");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

CSpinHorse * CSpinHorse::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CSpinHorse* pInstance = new CSpinHorse(pGraphicDev);
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

D3DXMATRIX CSpinHorse::GetWorldMatrix()
{
	return dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->GetWorldMatrix();
}

const float & CSpinHorse::GetAngle() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_fAngle;
}

ENGINE::SAVE_INFO CSpinHorse::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 1;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

void CSpinHorse::SpinHorsePlay()
{
	m_fAngle = 45.f * m_pTimeMgr->GetDeltaTime();
	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->MoveAngle(ENGINE::ANGLE_Y , m_fAngle);

	//if (m_pKeyMgr->KeyDown(ENGINE::KEY_F3))
	//{
	//	CSoundMgr::GetInstance()->PlaySound(L"SpinHorseTheme.mp3", CSoundMgr::PLAYER);
	//	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::PLAYER, 0.2f);
	//}
}
