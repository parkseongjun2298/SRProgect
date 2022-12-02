#include "stdafx.h"
#include "ChurroShop.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

CChurroShop::CChurroShop(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
}


CChurroShop::~CChurroShop()
{
	Release();
}

int CChurroShop::Update()
{
	ENGINE::CGameObject::Update();
	return NO_EVENT;
}

void CChurroShop::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CChurroShop::Render()
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
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&pTransform->GetWorldMatrix());

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());

	//	// ������ ����� �ܺο� �����ϴ� �������� ������ȯ���� ���� ��Ų��.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	////m_pGraphicDev->SetTexture(0, nullptr);

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_ChurroShop"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_ChurroShop"]->Render();
}

HRESULT CChurroShop::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_ChurroShop"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////


	return S_OK;
}

HRESULT CChurroShop::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_ChurroShop");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_ChurroShop Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_ChurroShop", pComponent));


	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_ChurroShop");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_ChurroShop Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	return S_OK;
}

void CChurroShop::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CChurroShop * CChurroShop::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CChurroShop* pInstance = new CChurroShop(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0);


	return pInstance;
}
