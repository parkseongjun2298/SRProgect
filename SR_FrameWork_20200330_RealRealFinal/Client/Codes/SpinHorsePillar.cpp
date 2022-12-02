#include "stdafx.h"
#include "SpinHorsePillar.h"
#include "SpinHorse.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CubeCol.h"


CSpinHorsePillar::CSpinHorsePillar(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr), m_pParent(nullptr)
{
}


CSpinHorsePillar::~CSpinHorsePillar()
{
	Release();
}

int CSpinHorsePillar::Update()
{
	ENGINE::CGameObject::Update();
	//HeightCheck();

	return NO_EVENT;
}

void CSpinHorsePillar::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CSpinHorsePillar::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix() * m_pParent->GetWorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 8; ++i)
	//{
	//	// 로컬 -> 뷰 스페이스
	//	D3DXMATRIX matWorld;
	//	matWorld = pTransform->GetWorldMatrix() * m_pParent->GetWorldMatrix() * pObserver->GetViewMatrix();
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

	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CSpinHorsePillar::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_fSpeed = 1.f;

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_COL*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_COL[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_COL) * 8);

	return S_OK;
}

void CSpinHorsePillar::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

HRESULT CSpinHorsePillar::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SpinHorsePillar");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SpinHorsePillar Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));


	//// Texture
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SpinHorseHorse");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SpinHorseHorse Clone Add Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorsePillar Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorsePillar Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	m_pSubjectMgr->Subscribe(L"Camera", pObserver);

	return S_OK;
}

CSpinHorsePillar * CSpinHorsePillar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, CSpinHorse * pParent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CSpinHorsePillar* pInstance = new CSpinHorsePillar(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pParent = pParent;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSpinHorsePillar::HeightCheck()
{
	//// 0보다 작을 시
	//if (m_tInfo.vPos.y < 0.f)
	//{
	//	m_fSpeed = -m_fSpeed;	// 스피드를 역스피드 ex) 10 -> -10
	//	m_tInfo.vPos.y = 0.f;
	//}
	//else if (m_tInfo.vPos.y > 4.f)
	//{
	//	m_fSpeed = -m_fSpeed;
	//	m_tInfo.vPos.y = 4.f;
	//}
	//m_tInfo.vPos.y += m_fSpeed * m_pTimeMgr->GetDeltaTime();

	//dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetPos(m_tInfo.vPos);

	if (m_tInfo.vSize.y > 1.f)
	{
		m_tInfo.vSize.y = 1.f;
		m_fSpeed = -m_fSpeed;
	}
	else if (m_tInfo.vSize.y < 0.55f)
	{
		m_tInfo.vSize.y = 0.55f;
		m_fSpeed = -m_fSpeed;
	}
	m_tInfo.vSize.y -= m_fSpeed * m_pTimeMgr->GetDeltaTime() / 6.5f;

	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetSize(m_tInfo.vSize);
}
