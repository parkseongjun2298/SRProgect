#include "stdafx.h"
#include "BumperCarMainField.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "Texture.h"
#include "PipeLine.h"
#include "RcTexLying.h"

CBumperCarMainField::CBumperCarMainField(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance())
{
}


CBumperCarMainField::~CBumperCarMainField()
{
	Release();
}

int CBumperCarMainField::Update()
{
	CGameObject::Update();
	return 0;
}

void CBumperCarMainField::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CBumperCarMainField::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 4; ++i)
	{
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// 투영스페이스 범위 밖에 정점들을 투영변환에서 제외시키기
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());

	}
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBumperCarMainField::Initialize()
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

HRESULT CBumperCarMainField::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_BumperCarField");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_BumperCarField Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_BumperCarMainField");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_BumperCarMainField Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_vRight = { 1.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };
	m_vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	return S_OK;
}

void CBumperCarMainField::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CBumperCarMainField * CBumperCarMainField::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CBumperCarMainField* pInstance = new CBumperCarMainField(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}
