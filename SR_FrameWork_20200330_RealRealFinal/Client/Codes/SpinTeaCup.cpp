#include "stdafx.h"
#include "SpinTeaCup.h"
#include "Transform.h"
#include "CubeTex.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "TeaCupPlane.h"
#include "SoundMgr.h"

#include "Queue.h"
#include "DeadObj.h"
#include "Customer.h"

CSpinTeaCup::CSpinTeaCup(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pConvertVtx(nullptr), m_pOriginVtx(nullptr), m_fWorkTime(0.f), m_fPlayTime(0.f),
	m_bPlay(true)
{
	m_vecTeaCupPlane.reserve(4);
}


CSpinTeaCup::~CSpinTeaCup()
{
	Release();
}

int CSpinTeaCup::Update()
{
	
	m_fPlayTime += m_pTimeMgr->GetDeltaTime();
	if (m_fPlayTime >= 16.f)
	{
		PushCustomer();
		m_bPlay = true;
		//CSoundMgr::GetInstance()->PlaySound(L"둥글게둥글게.mp3", CSoundMgr::CHANNELID::RIDES);
		//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.02f);
		m_fPlayTime = 0.f;
		m_fWorkTime = 0.f;
	}
	else if (m_fPlayTime >= 15.f)
	{
		if (m_bPlay)
		{
			//CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::RIDES);
			for (int i = 0; i < 4; ++i)
				m_vecTeaCupPlane[i]->ClearSeat(dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->GetPos());
		}
		m_bPlay = false;
	}
	if (m_bPlay)
	{
		RidesWorks();
		KeyInput();

		for (auto& pTeaCup : m_vecTeaCupPlane)
			pTeaCup->Update();
	}

	return ENGINE::CGameObject::Update();
}

void CSpinTeaCup::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();

	for (auto& pTeaCup : m_vecTeaCupPlane)
		pTeaCup->LateUpdate();
}

void CSpinTeaCup::Render()
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
	//	D3DXMATRIX matWorld;
	//	matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();

	//	// 뷰 스페이스까지 전환.
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&matWorld);

	//	// 근평면과 원평면 외부에 존재하는 정점들을 투영변환에서 제외시킨다. (추려내기!!)
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	for (auto& pTeaCup : m_vecTeaCupPlane)
		pTeaCup->Render();
}

HRESULT CSpinTeaCup::Initialize()
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

	// TeaCups
	/*
		2
	3		1
		0
	*/
	CTeaCupPlane* pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 0.f, -5.f), this);	// 0
	NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	m_vecTeaCupPlane.push_back(pTeaCup);
	pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(5.f, 0.f, 0.f), this);	// 1
	NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	m_vecTeaCupPlane.push_back(pTeaCup);
	pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 0.f, 5.f), this);	// 2
	NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	m_vecTeaCupPlane.push_back(pTeaCup);
	pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(-5.f, 0.f, 0.f), this);	// 3
	NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	m_vecTeaCupPlane.push_back(pTeaCup);

	//CTeaCupPlane* pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x, pTransform->GetPos().y, pTransform->GetPos().z - 5.f), this);	// 0
	//NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	//m_vecTeaCupPlane.push_back(pTeaCup);
	//pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x + 5.f, pTransform->GetPos().y, pTransform->GetPos().z), this);	// 1
	//NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	//m_vecTeaCupPlane.push_back(pTeaCup);
	//pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x, pTransform->GetPos().y, pTransform->GetPos().z + 5.f), this);	// 2
	//NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	//m_vecTeaCupPlane.push_back(pTeaCup);
	//pTeaCup = CTeaCupPlane::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x - 5.f, pTransform->GetPos().y, pTransform->GetPos().z), this);	// 3
	//NULL_CHECK_RETURN(pTeaCup, E_FAIL);
	//m_vecTeaCupPlane.push_back(pTeaCup);

	m_eRideType = ENGINE::RIDE_SPINTEACUP;

	return S_OK;
}

HRESULT CSpinTeaCup::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SpinTeaCup");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SpinTeaCup Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SpinTeaCup");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SpinTeaCup Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinTeaCup Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"SpinTeaCup");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CSpinTeaCup::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);

	for_each(m_vecTeaCupPlane.begin(), m_vecTeaCupPlane.end(), ENGINE::SafeDelete<CTeaCupPlane*>);
}

CSpinTeaCup * CSpinTeaCup::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CSpinTeaCup* pInstance = new CSpinTeaCup(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_fAngleY = fAngleY;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}

void CSpinTeaCup::RidesWorks()
{
	float fAngleSpeedY = 30.f * m_pTimeMgr->GetDeltaTime();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	pTransform->MoveAngle(ENGINE::ANGLE_Y, fAngleSpeedY);

	// 시간이 지나면 z 축을 기준으로 회전을 하여 위로 올라간듯한 연출
	m_fWorkTime += m_pTimeMgr->GetDeltaTime();

	if (1.f <= m_fWorkTime && 5.f > m_fWorkTime)
	{
		if (15.f >= pTransform->GetAngle(ENGINE::ANGLE_Z))
		{
			float fAngleSpeedZ = 15.f * m_pTimeMgr->GetDeltaTime() / 2.f;
			pTransform->MoveAngle(ENGINE::ANGLE_Z, fAngleSpeedZ);
		}
	}

	if (5.f <= m_fWorkTime && 15.f > m_fWorkTime)
	{
		if (0.f <= pTransform->GetAngle(ENGINE::ANGLE_Z))
		{
			float fAngleSpeedZ = 15.f * m_pTimeMgr->GetDeltaTime() / 4.f;
			pTransform->MoveAngle(ENGINE::ANGLE_Z, -fAngleSpeedZ);
		}
	}

	if (15.f <= m_fWorkTime && 25.f > m_fWorkTime)
	{
		if (-15.f <= pTransform->GetAngle(ENGINE::ANGLE_Z))
		{
			float fAngleSpeedZ = 15.f * m_pTimeMgr->GetDeltaTime() / 2.f;
			pTransform->MoveAngle(ENGINE::ANGLE_Z, -fAngleSpeedZ);
		}
	}

	if (25.f <= m_fWorkTime && 35.f > m_fWorkTime)
	{
		if (0.f >= pTransform->GetAngle(ENGINE::ANGLE_Z))
		{
			float fAngleSpeedZ = 15.f * m_pTimeMgr->GetDeltaTime() / 4.f;
			pTransform->MoveAngle(ENGINE::ANGLE_Z, fAngleSpeedZ);
		}
	}
}

void CSpinTeaCup::KeyInput()
{
	// 일단은 1,3번은 오른쪽으로 돌면 2,4번은 왼쪽으로 돌게함.
	int iTempCount = 0;
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_ADD))
	{
		for (auto& pPlane : m_vecTeaCupPlane)
		{
			++iTempCount;
			if (1 == iTempCount % 2)
				pPlane->SetSpinRight(true);
			else
				pPlane->SetSpinLeft(true);
		}
	}

	// 1,3번은 왼쪽으로 2,4번은 오른쪽으로
	//if (m_pKeyMgr->KeyDown(ENGINE::KEY_SPACE))
	//{
	//	for (auto& pPlane : m_vecTeaCupPlane)
	//	{
	//		++iTempCount;
	//		if (1 == iTempCount % 2)
	//			pPlane->SetSpinLeft(true);
	//		else
	//			pPlane->SetSpinRight(true);
	//	}
	//}
}

void CSpinTeaCup::PushCustomer()
{
	// 손님들 찻잔에 집어넣기.
	// 찻잔구조는 찻잔큰틀-> 찻잔을 4개씩 가지고있는 바닥 -> 바닥안에 포함된 찻잔으로 총, 4 * 4 = 16개의 손님이 들어갈수 있다.
	// m_vecTeaCupPlane.size() = 4;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
			ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
			if (nullptr != pCustomer)
				m_vecTeaCupPlane[i]->PushSeat(j, dynamic_cast<CCustomer*>(pCustomer));
		}
	}
}

ENGINE::SAVE_INFO CSpinTeaCup::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 5;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}
