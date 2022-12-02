#include "stdafx.h"
#include "FerrisLeg.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

CFerrisLeg::CFerrisLeg(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fAngleX(0.f)
{

}


CFerrisLeg::~CFerrisLeg()
{
	Release();
}

int CFerrisLeg::Update()
{
	ENGINE::CGameObject::Update();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	pTransform->SetAngle(ENGINE::ANGLE_X, m_fAngleX);

	return NO_EVENT;
}

void CFerrisLeg::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();


	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetAngle(ENGINE::ANGLE_X, m_fAngleX);
}

void CFerrisLeg::Render()
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

	m_pGraphicDev->SetTexture(0, nullptr);
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisLeg"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_FerrisLeg"]->Render();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &MATRIX::Identity());

}

HRESULT CFerrisLeg::Initialize()
{

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisLeg"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);


	m_eRideType = ENGINE::RIDE_FERRIS;



	return S_OK;
}

HRESULT CFerrisLeg::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_FerrisLeg");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_FerrisLeg Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_FerrisLeg", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_FerrisLeg");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_FerrisLeg Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 5.f, 15.f, 0.f };
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

void CFerrisLeg::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);

}

CFerrisLeg * CFerrisLeg::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleX)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFerrisLeg* pInstance = new CFerrisLeg(pGraphicDev);
	pInstance->m_vTemp = vPos;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0);

	pInstance->m_fAngleX = fAngleX;

	return pInstance;
}

