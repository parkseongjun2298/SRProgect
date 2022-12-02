#include "stdafx.h"
#include "EffectCount.h"
#include "Transform.h"
#include "RcTex.h"
#include "PipeLine.h"
#include "CameraObserver.h"
#include "DeadObj.h"

CEffectCount::CEffectCount(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr), m_bIsRender(false)
{
}


CEffectCount::~CEffectCount()
{
	Release();
}

int CEffectCount::Update()
{
	return ENGINE::CGameObject::Update();
}

void CEffectCount::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CEffectCount::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 4; ++i)
	{
		// 로컬 -> 뷰스페이스
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// 투영스페이스 변환전 예외처리
		if (1.f > m_pConvertVtx[i].vPos.z || 1000.f < m_pConvertVtx[i].vPos.z)
			continue;

		// 뷰 -> 투영스페이스
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	if (m_bIsMius)
		m_mapComponent[L"Texture_Minus"]->Render();
	else
		m_mapComponent[L"Texture_Plus"]->Render();
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CEffectCount::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	return S_OK;
}

HRESULT CEffectCount::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Effect");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_EffectPlus Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_EffectPlus");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_EffectPlus Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Plus", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_EffectMinus");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_EffectMinus Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Minus", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"EffectPlus Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"EffectPlus Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	// Dead Obj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"EffectPlus DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CEffectCount::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

bool CEffectCount::Get_RenderState()
{
	return m_bIsRender;
}

void CEffectCount::Set_RenderState(bool bCheck)
{
	m_bIsRender = bCheck;
}

void CEffectCount::Set_Pos(D3DXVECTOR3 vPos)
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	pTransform->SetPos(vPos);
}

void CEffectCount::Set_Minus(bool bCheck)
{
	m_bIsMius = bCheck;
}

CEffectCount* CEffectCount::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CEffectCount* pInstance = new CEffectCount(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}
