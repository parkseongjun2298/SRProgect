#include "stdafx.h"
#include "VikingLeg.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

CVikingLeg::CVikingLeg(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())

{

}

CVikingLeg::~CVikingLeg()
{
	Release();
}

int CVikingLeg::Update()
{
	ENGINE::CGameObject::Update();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	pTransform->SetAngle(ENGINE::ANGLE_Z, m_fAngle);
	return NO_EVENT;
}

void CVikingLeg::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CVikingLeg::Render()
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

	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	m_pGraphicDev->SetTexture(0, nullptr);
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingLegL"])->SetConvertVtx(m_pConvertVtx);
	

	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_VikingLegL"]->Render();
	

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &MATRIX::Identity());
}

HRESULT CVikingLeg::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//왼쪽
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingLegL"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);



	////오른쪽
	//ENGINE::CVIBuffer* pBuffer2 = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingLegR"]);
	//NULL_CHECK_RETURN(pBuffer2, E_FAIL);

	//m_pOriginVtx[1] = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer2->GetOriginVtx());
	//NULL_CHECK_RETURN(m_pOriginVtx[1], E_FAIL);

	//m_pConvertVtx[1] = new ENGINE::VTX_CUBETEX[8];
	//memcpy(m_pConvertVtx[1], m_pOriginVtx[1], sizeof(ENGINE::VTX_CUBETEX) * 8);
	//

	m_eRideType = ENGINE::RIDE_VIKING;

	return S_OK;
}

HRESULT CVikingLeg::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingLegL");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingLegL Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_VikingLegL", pComponent));


	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingLegR");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingLegR Clone Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Buffer_VikingLegR", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_VikingLeg");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_VikingLeg Clone Failed", E_FAIL);
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

void CVikingLeg::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);


}


CVikingLeg* CVikingLeg::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleX)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CVikingLeg* pInstance = new CVikingLeg(pGraphicDev);
	pInstance->m_fAngle = fAngleX;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}