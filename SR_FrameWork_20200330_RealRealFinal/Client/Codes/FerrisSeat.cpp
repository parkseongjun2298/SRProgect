#include "stdafx.h"
#include "FerrisSeat.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "FerrisBody.h"
#include"FerrisArm.h"
#include "Seat.h"
#include "Customer.h"
CFerrisSeat::CFerrisSeat(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
}


CFerrisSeat::~CFerrisSeat()
{
	Release();
}

int CFerrisSeat::Update()
{
	ENGINE::CGameObject::Update();
	//CheckDir();
	//MoveSeat();

	Matrix matParentMatrix = MATRIX::Identity();
	//Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CFerrisSeat::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	Vec3 vSize = pTransform->GetSize();
	Vec3 vPos = pTransform->GetPos();
	float fAngle[3] = { pTransform->GetAngle(ENGINE::ANGLE_X), pTransform->GetAngle(ENGINE::ANGLE_Y), pTransform->GetAngle(ENGINE::ANGLE_Z) };

	ENGINE::CTransform* pTransformBody = dynamic_cast<ENGINE::CTransform*>(m_pArm->Get_Body()->GetComponent(L"Transform"));

	Matrix matScale, matRotateX, matRotateY, matRotateZ, matTrans, matWorld;
	matScale = MATRIX::Scaling(vSize);
	matRotateX = MATRIX::RotateX(D3DXToRadian(fAngle[0]));
	matRotateY = MATRIX::RotateY(D3DXToRadian(fAngle[1]));
	matRotateZ = MATRIX::RotateZ(D3DXToRadian(fAngle[2] - pTransformBody->GetAngle(ENGINE::ANGLE_Z)));
	matTrans = MATRIX::Translation(vPos);

	matWorld = matScale * matRotateY * matRotateX * matRotateZ * matTrans;

	pTransform->SetWorldMatrix(matWorld);
	pTransform->SetParentWorldMatrix(m_pArm->Get_Body()->GetWorldMatrix());
}

void CFerrisSeat::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix();// *m_pArm->Get_Body()->Get_Matrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(matWorld));

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
//	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisSeat"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_FerrisSeat"]->Render();

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(matWorld);
}

HRESULT CFerrisSeat::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_FerrisSeat"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(m_vPos);

	m_eRideType = ENGINE::RIDE_FERRIS;

	return S_OK;
}

HRESULT CFerrisSeat::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_FerrisSeat");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_FerrisSeat Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_FerrisSeat", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_FerrisSeat");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_FerrisSeat Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	CheckDir();
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	//D3DXVECTOR3 vPos = { 5.f, 15.f, 0.f };
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
	pComponent = CSeat::Create(m_pGraphicDev, 1);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, -2.f, -1.f)); // 로컬
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));

	return S_OK;
}

void CFerrisSeat::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CFerrisSeat * CFerrisSeat::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, CFerrisArm* Parent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFerrisSeat* pInstance = new CFerrisSeat(pGraphicDev);
	pInstance->m_pArm = Parent;
	pInstance->m_vPos = vPos;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);
	return pInstance;
}

void CFerrisSeat::MoveSeat()
{
	ENGINE::CTransform* pTranform = dynamic_cast<ENGINE::CTransform*>(m_pArm->Get_Body()->GetComponent(L"Transform"));
	NULL_CHECK(pTranform);


	ENGINE::CTransform* pTranform2 = dynamic_cast<ENGINE::CTransform*>(this->GetComponent(L"Transform"));
	NULL_CHECK(pTranform2);
	if (pTranform2->GetAngle(ENGINE::ANGLE_Z) <= 180.f)
		pTranform2->SetAngle(ENGINE::ANGLE_Z, -pTranform->GetAngle(ENGINE::ANGLE_Z));
	else
		pTranform2->SetAngle(ENGINE::ANGLE_Z, 360.f - pTranform->GetAngle(ENGINE::ANGLE_Z));

	if (pTranform2->GetAngle(ENGINE::ANGLE_Z) > 360.f)
		pTranform2->SetAngle(ENGINE::ANGLE_Z, 360.f);
}

void CFerrisSeat::CheckDir()
{
	D3DXVECTOR3 Temp{ 1,-1,0 };
	ENGINE::CTransform* pTranform = dynamic_cast<ENGINE::CTransform*>(m_pArm->GetComponent(L"Transform"));
	NULL_CHECK(pTranform);
	m_vPos = pTranform->GetPos();
	D3DXVec3Normalize(&Temp, &Temp);
	m_tInfo.vPos = m_vPos + Temp * 4.f;
}

