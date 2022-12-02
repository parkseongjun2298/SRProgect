#include "stdafx.h"
#include "GyroSwingSpin.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"GyroSwingDevice.h"
#include "Seat.h"
#include "Customer.h"
CGyroSwingSpin::CGyroSwingSpin(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
}


CGyroSwingSpin::~CGyroSwingSpin()
{
	Release();
}

int CGyroSwingSpin::Update()
{
	ENGINE::CGameObject::Update();


	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	float fRotateSpeed = 90.f * m_pTimeMgr->GetDeltaTime();
	pTransform->SetAngle(ENGINE::ANGLE_Y, pTransform->GetAngle(ENGINE::ANGLE_Y) + fRotateSpeed);

	if (m_MParent->Get_FinCheck())
	{
		Vec3 vPos = pTransform->GetPos();
		vPos.y = 0.f;
		dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(vPos);
	}


	Matrix matParentMatrix = MATRIX::Identity();
	//Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CGyroSwingSpin::LateUpdate()
{
	//ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	Vec3 vSize = pTransform->GetSize();
	Vec3 vPos = pTransform->GetPos();
	float fAngle[3] = { pTransform->GetAngle(ENGINE::ANGLE_X), pTransform->GetAngle(ENGINE::ANGLE_Y), pTransform->GetAngle(ENGINE::ANGLE_Z) };

	ENGINE::CTransform* pTransformBody = dynamic_cast<ENGINE::CTransform*>(m_MParent->GetComponent(L"Transform"));

	Matrix matScale, matRotateX, matRotateY, matRotateZ, matTrans, matWorld;
	matScale = MATRIX::Scaling(vSize);
	matRotateX = MATRIX::RotateX(D3DXToRadian(fAngle[0]));
	matRotateY = MATRIX::RotateY(D3DXToRadian(fAngle[1]));
	matRotateZ = MATRIX::RotateZ(fAngle[2]);
	matTrans = MATRIX::Translation(vPos);

	matWorld = matScale * matRotateX  * matRotateZ * matTrans * matRotateY;

	pTransform->SetWorldMatrix(matWorld);

	//ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetParentWorldMatrix(m_MParent->GetWorldMatrix());

}

void CGyroSwingSpin::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix();// *m_MParent->GetWorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	Matrix matParezntMatrix = dynamic_cast<ENGINE::CTransform*>(m_MParent->GetComponent(L"Transform"))->GetWorldMatrix();
	//	Matrix matMatrix = pTransform->GetWorldMatrix();
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(pTransform->GetWorldMatrix()));

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
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroSwingSpin"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_GyroSwingSpin"]->Render();

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(matWorld);

}

HRESULT CGyroSwingSpin::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//왼쪽
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroSwingSpin"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	m_eRideType = ENGINE::RIDE_GYROSWING;

	return S_OK;
}

HRESULT CGyroSwingSpin::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_GyroSwingSpin");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_GyroSwingSpin Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_GyroSwingSpin", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_GyroSwingSpin");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_GyroSwingSpin Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f,0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, m_tInfo.vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));


	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	// Seat
	pComponent = CSeat::Create(m_pGraphicDev, 5);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, 0.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(1, Vec3(4.f, 0.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(2, Vec3(8.f, 0.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(3, Vec3(-4.f, 0.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(4, Vec3(-8.f, 0.f, 0.f)); // 로컬



	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));

	return S_OK;
}

void CGyroSwingSpin::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CGyroSwingSpin * CGyroSwingSpin::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, CGyroSwingDevice* Parent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGyroSwingSpin* pInstance = new CGyroSwingSpin(pGraphicDev);
	pInstance->m_MParent = Parent;
	pInstance->m_tInfo.vPos = vPos;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}

void CGyroSwingSpin::PushSeat(int iIdx, CCustomer * pCustomer)
{
	pCustomer->RideSetting(Vec3(0.f, 0.f, -1.f));
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(iIdx, pCustomer);
}


