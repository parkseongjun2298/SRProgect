#include "stdafx.h"
#include "SpinHorsePlane.h"
#include "SpinHorse.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CubeCol.h"
#include "CubeTex.h"


CSpinHorsePlane::CSpinHorsePlane(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr), m_pParent(nullptr)
{
}


CSpinHorsePlane::~CSpinHorsePlane()
{
	Release();
}

int CSpinHorsePlane::Update()
{
	ENGINE::CGameObject::Update();
	PlaneSpin();

	return NO_EVENT;
}

void CSpinHorsePlane::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CSpinHorsePlane::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);
	
	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();

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

	//	// 투영변환 전 예외처리
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

}

HRESULT CSpinHorsePlane::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	return S_OK;
}

void CSpinHorsePlane::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

HRESULT CSpinHorsePlane::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;
	
	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SpinHorsePlane");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SpinHorsePlane Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SpinHorsePlane");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SpinHorsePlane Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorsePlane Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorsePlane Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	m_pSubjectMgr->Subscribe(L"Camera", pObserver);

	return S_OK;
}

CSpinHorsePlane * CSpinHorsePlane::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, CSpinHorse* pParent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CSpinHorsePlane* pInstance = new CSpinHorsePlane(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pParent = pParent;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSpinHorsePlane::PlaneSpin()
{
	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->MoveAngle(ENGINE::ANGLE_Y, m_pParent->GetAngle());
}
