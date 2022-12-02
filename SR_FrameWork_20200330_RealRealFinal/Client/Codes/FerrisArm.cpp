#include "stdafx.h"
#include "FerrisArm.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"FerrisSeat.h"
#include"FerrisBody.h"
CFerrisArm::CFerrisArm(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fAngleZ(0.f)
{

}


CFerrisArm::~CFerrisArm()
{
	Release();
}

int CFerrisArm::Update()
{
	ENGINE::CGameObject::Update();
	m_pSeat->Update();
	RideFerrisSeat();



	return NO_EVENT;
}

void CFerrisArm::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();

	m_pSeat->LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetAngle(ENGINE::ANGLE_Z, m_fAngleZ);


	Vec3 vSize = pTransform->GetSize();
	Vec3 vPos = pTransform->GetPos();
	float fAngle[3] = { pTransform->GetAngle(ENGINE::ANGLE_X), pTransform->GetAngle(ENGINE::ANGLE_Y), pTransform->GetAngle(ENGINE::ANGLE_Z) };

	Matrix matScale, matRotateX, matRotateY, matRotateZ, matTrans, matWorld, matGong, matParent;
	matScale = MATRIX::Scaling(vSize);
	matRotateX = MATRIX::RotateX(D3DXToRadian(fAngle[0]));
	matRotateY = MATRIX::RotateY(D3DXToRadian(fAngle[1]));
	matRotateZ = MATRIX::RotateZ(D3DXToRadian(fAngle[2]));
	matTrans = MATRIX::Translation(vPos);
	matGong = MATRIX::RotateZ(D3DXToRadian(10.f));



	matWorld = matScale * matRotateY * matRotateX * matRotateZ * matTrans/* * matGong*m_vParant*/;

	pTransform->SetWorldMatrix(matWorld);

}

void CFerrisArm::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(pTransform->GetWorldMatrix()* m_pBody->Get_Matrix()));
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(pTransform->GetWorldMatrix()* m_pBody->Get_Matrix()));

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
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisArm"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_FerrisArm"]->Render();

	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &MATRIX::Identity());*/

	m_pSeat->Render();

}

HRESULT CFerrisArm::Initialize()
{

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisArm"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);




	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	//시트 생성
	//Vec3 vDir = VEC3::Normalize(m_tInfo.vPos - m_pBody->GetPos());
	m_pSeat = CFerrisSeat::Create(m_pGraphicDev, m_tInfo.vPos/* + vDir * 30.f*/, m_tInfo.vSize, 0.f, this);
	NULL_CHECK_RETURN(m_pSeat, E_FAIL);

	m_eRideType = ENGINE::RIDE_FERRIS;

	return S_OK;
}

HRESULT CFerrisArm::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_FerrisArm");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_FerrisArm Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_FerrisArm", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_FerrisArm");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_FerrisArm Clone Failed", E_FAIL);
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

void CFerrisArm::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));

	ENGINE::SafeDelete(m_pSeat);

	ENGINE::SafeDeleteArray(m_pConvertVtx);

}

CFerrisArm * CFerrisArm::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleZ, CFerrisBody* Parent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFerrisArm* pInstance = new CFerrisArm(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pBody = Parent;
	pInstance->m_tInfo.vSize = vSize;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0);

	pInstance->m_fAngleZ = fAngleZ;

	return pInstance;
}

void CFerrisArm::RideFerrisSeat()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	//pTransform->SetWorldMatrix
}
