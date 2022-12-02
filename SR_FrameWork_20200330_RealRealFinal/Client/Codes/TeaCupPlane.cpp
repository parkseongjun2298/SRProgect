#include "stdafx.h"
#include "TeaCupPlane.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "TeaCup.h"

#include "PipeLine.h"
#include "CubeCol.h"
#include "CameraObserver.h"

#include "Customer.h"

CTeaCupPlane::CTeaCupPlane(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pParent(nullptr), m_pConvertVtx(nullptr), m_pOriginVtx(nullptr)
{
	m_vecTeaCup.reserve(4);
}


CTeaCupPlane::~CTeaCupPlane()
{
	Release();
}

void CTeaCupPlane::SetSpinRight(bool bCheck)
{
	m_bSpinRight = bCheck;
}

void CTeaCupPlane::SetSpinLeft(bool bCheck)
{
	m_bSpinLeft = bCheck;
}

int CTeaCupPlane::Update()
{
	CGameObject::Update();
	SpinStateCheck();
	RidesWorks();

	for (auto& pTeaCup : m_vecTeaCup)
	{
		pTeaCup->Update();
	}
	return 0;
}

void CTeaCupPlane::LateUpdate()
{
	CGameObject::LateUpdate();

	for (auto& pTeaCup : m_vecTeaCup)
	{
		pTeaCup->LateUpdate();
	}
}

void CTeaCupPlane::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	ENGINE::CTransform* pParentTrans = dynamic_cast<ENGINE::CTransform*>(m_pParent->GetComponent(L"Transform"));
	NULL_CHECK(pParentTrans);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	pTransform->SetWorldMatrix(pTransform->GetWorldMatrix() * pParentTrans->GetWorldMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&pTransform->GetWorldMatrix());

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());

	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	//m_mapComponent[L"Texture"]->Render(0);
	//m_mapComponent[L"Buffer"]->Render();

	//ENGINE::CTransform* pParentTrans = dynamic_cast<ENGINE::CTransform*>(m_pParent->GetComponent(L"Transform"));

	//pTransform->SetWorldMatrix(pTransform->GetWorldMatrix() * pParentTrans->GetWorldMatrix());

	for (auto& pTeaCup : m_vecTeaCup)
	{
		pTeaCup->Render();
	}
}

HRESULT CTeaCupPlane::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	//ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	//NULL_CHECK_RETURN(pBuffer, E_FAIL);

	//m_pOriginVtx = reinterpret_cast<ENGINE::VTX_COL*>(pBuffer->GetOriginVtx());
	//NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	//m_pConvertVtx = new ENGINE::VTX_COL[8];
	//memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_COL) * 8);
	///*
	//	 3
	//4		2
	//	 1
	//*/
	CTeaCup* pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 1.5f, 2.f), this);
	NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	m_vecTeaCup.push_back(pTeaCup);

	pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(2.f, 1.5f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	m_vecTeaCup.push_back(pTeaCup);

	pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 1.5f, -2.f), this);
	NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	m_vecTeaCup.push_back(pTeaCup);

	pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(-2.f, 1.5f, 0.f), this);
	NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	m_vecTeaCup.push_back(pTeaCup);

	//CTeaCup* pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x, pTransform->GetPos().y, pTransform->GetPos().z + 5.f), this);
	//NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	//m_vecTeaCup.push_back(pTeaCup);

	//pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x + 5.f, pTransform->GetPos().y, pTransform->GetPos().z), this);
	//NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	//m_vecTeaCup.push_back(pTeaCup);

	//pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x, pTransform->GetPos().y, pTransform->GetPos().z - 5.f), this);
	//NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	//m_vecTeaCup.push_back(pTeaCup);

	//pTeaCup = CTeaCup::Create(m_pGraphicDev, D3DXVECTOR3(pTransform->GetPos().x - 5.f, pTransform->GetPos().y, pTransform->GetPos().z), this);
	//NULL_CHECK_MSG_RETURN(pTeaCup, L"TeaCup Create Failed", E_FAIL);
	//m_vecTeaCup.push_back(pTeaCup);

	m_eRideType = ENGINE::RIDE_SPINTEACUP;

	return S_OK;
}

HRESULT CTeaCupPlane::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	//// Buffer
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_TeaCupPlane");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_TeaCupPlane Clone Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	//// Texture
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Monster");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Monster Clone Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"TeaCupPlane Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"TeaCupPlane Observer Create Failde", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));
	return S_OK;
}

void CTeaCupPlane::Release()
{
	for_each(m_vecTeaCup.begin(), m_vecTeaCup.end(), ENGINE::SafeDelete<CTeaCup*>);
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CTeaCupPlane::SpinStateCheck()
{
	if (m_bSpinRight)
	{
		// m_bRightSpin이 true라면, 오른쪽으로 돌라는 신호.
		for (auto& pTeaCup : m_vecTeaCup)
			pTeaCup->SetRightSpin(true);
		m_bSpinRight = false;
	}

	if (m_bSpinLeft)
	{
		// m_bSpinLeft이 true라면, 왼쪽으로 돌라는 신호.
		for (auto& pTeaCup : m_vecTeaCup)
			pTeaCup->SetLeftSpin(true);
		m_bSpinLeft = false;
	}
}

void CTeaCupPlane::RidesWorks()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	pTransform->MoveAngle(ENGINE::ANGLE_Y, m_fAngle * m_pTimeMgr->GetDeltaTime());
}

void CTeaCupPlane::PushSeat(int iIndex, CCustomer * pCustomer)
{
	NULL_CHECK_MSG(pCustomer, L"TeaCup Customer Push Failed");
	m_vecTeaCup[iIndex]->PushSeat(pCustomer);
}

void CTeaCupPlane::ClearSeat(D3DXVECTOR3 vPos)
{
	for (int i = 0; i < 4; ++i)
		m_vecTeaCup[i]->ClearSeat(vPos);
}

CTeaCupPlane * CTeaCupPlane::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, ENGINE::CGameObject * pParent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	NULL_CHECK_RETURN(pParent, nullptr);

	CTeaCupPlane* pInstance = new CTeaCupPlane(pGraphicDev);

	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pParent = pParent;
	pInstance->m_fAngle = (float)(rand() % 40);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}
