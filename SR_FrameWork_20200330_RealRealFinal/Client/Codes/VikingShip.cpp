#include "stdafx.h"
#include "VikingShip.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "SoundMgr.h"
#include "Seat.h"
#include "Customer.h"

#define VIKING_PLAYING_TIME 60.f

CVikingShip::CVikingShip(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fAngle(0), m_fSpeed(1.f), m_fAngle2(0),
	m_fPlayTime(0), m_fAngleTime(0), m_fValue(0),
	m_bDirCheck(false)
{
	m_pOriginVtx[4] = { 0, };
	m_pConvertVtx[4] = { 0, };
}

CVikingShip::~CVikingShip()
{
	Release();
}

int CVikingShip::Update()
{
	MoveViking();
	ENGINE::CGameObject::Update();


	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	if (pTransform->GetAngle(ENGINE::ANGLE_Z) >= 165)
	{
		pTransform->SetAngle(ENGINE::ANGLE_Z, 165);
	}

	if (pTransform->GetAngle(ENGINE::ANGLE_Z) <= -165)
	{
		pTransform->SetAngle(ENGINE::ANGLE_Z, -165);
	}

	Matrix matParentMatrix = MATRIX::Identity();
	Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CVikingShip::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	Vec3 vSize = pTransform->GetSize();
	Vec3 vPos = pTransform->GetPos();
	float fAngle[3] = { pTransform->GetAngle(ENGINE::ANGLE_X), pTransform->GetAngle(ENGINE::ANGLE_Y), pTransform->GetAngle(ENGINE::ANGLE_Z) };

	Matrix matScale, matRotateX, matRotateY, matRotateZ, matTrans, matWorld;
	matScale = MATRIX::Scaling(vSize);
	matRotateX = MATRIX::RotateX(D3DXToRadian(fAngle[0]));
	matRotateY = MATRIX::RotateY(D3DXToRadian(fAngle[1]));
	matRotateZ = MATRIX::RotateZ(D3DXToRadian(fAngle[2]));
	matTrans = MATRIX::Translation(vPos);

	matWorld = matScale * matRotateY * matRotateX * matRotateZ * matTrans;

	pTransform->SetWorldMatrix(matWorld);
}

void CVikingShip::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int j = 0; j < 4; ++j)
	//{
	//	for (int i = 0; i < 8; ++i)
	//	{
	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[j][i].vPos,
	//			&m_pOriginVtx[j][i].vPos,
	//			&pTransform->GetWorldMatrix());

	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[j][i].vPos,
	//			&m_pConvertVtx[j][i].vPos,
	//			&pObserver->GetViewMatrix());

	//		// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//		if (m_pConvertVtx[j][i].vPos.z < 1.f || m_pConvertVtx[j][i].vPos.z > 1000.f)
	//			continue;

	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[j][i].vPos,
	//			&m_pConvertVtx[j][i].vPos,
	//			&pObserver->GetProjMatrix());
	//	}
	//}
	m_pGraphicDev->SetTexture(0, nullptr);

	/*dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingHead"])->SetConvertVtx(m_pConvertVtx[0]);
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingBottom"])->SetConvertVtx(m_pConvertVtx[1]);
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingLeft"])->SetConvertVtx(m_pConvertVtx[2]);
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingRight"])->SetConvertVtx(m_pConvertVtx[3]);*/

	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_VikingHead"]->Render();
	m_mapComponent[L"Buffer_VikingBottom"]->Render();
	m_mapComponent[L"Buffer_VikingLeft"]->Render();
	m_mapComponent[L"Buffer_VikingRight"]->Render();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &MATRIX::Identity());

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(pTransform->GetWorldMatrix());
}

HRESULT CVikingShip::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//헤드
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingHead"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx[0] = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx[0], E_FAIL);

	m_pConvertVtx[0] = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx[0], m_pOriginVtx[0], sizeof(ENGINE::VTX_CUBETEX) * 8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////


	//바텀
	ENGINE::CVIBuffer* pBuffer2 = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingBottom"]);
	NULL_CHECK_RETURN(pBuffer2, E_FAIL);

	m_pOriginVtx[1] = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer2->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx[1], E_FAIL);

	m_pConvertVtx[1] = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx[1], m_pOriginVtx[1], sizeof(ENGINE::VTX_CUBETEX) * 8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//LEFT
	ENGINE::CVIBuffer* pBuffer3 = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingLeft"]);
	NULL_CHECK_RETURN(pBuffer3, E_FAIL);

	m_pOriginVtx[2] = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer3->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx[2], E_FAIL);

	m_pConvertVtx[2] = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx[2], m_pOriginVtx[2], sizeof(ENGINE::VTX_CUBETEX) * 8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//RIGHT
	ENGINE::CVIBuffer* pBuffer4 = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_VikingRight"]);
	NULL_CHECK_RETURN(pBuffer4, E_FAIL);

	m_pOriginVtx[3] = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer4->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx[3], E_FAIL);

	m_pConvertVtx[3] = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx[3], m_pOriginVtx[3], sizeof(ENGINE::VTX_CUBETEX) * 8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_eRideType = ENGINE::RIDE_VIKING;

	return S_OK;
}

HRESULT CVikingShip::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingHead");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingHead Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_VikingHead", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingBottom");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingBottom Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_VikingBottom", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingLeft");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingLeft Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_VikingLeft", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_VikingRight");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_VikingRight Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_VikingRight", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Viking");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Viking Clone Failed", E_FAIL);
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

	// Seat
	pComponent = CSeat::Create(m_pGraphicDev, 10);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(2.f, -10.f, -1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(1, Vec3(1.f, -10.f, -1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(2, Vec3(0.f, -10.f, -1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(3, Vec3(-1.f, -10.f, -1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(4, Vec3(-2.f, -10.f, -1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(5, Vec3(2.f, -10.f, 1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(6, Vec3(1.f, -10.f, 1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(7, Vec3(0.f, -10.f, 1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(8, Vec3(-1.f, -10.f, 1.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(9, Vec3(-2.f, -10.f, 1.f)); // 로컬


	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(1, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(2, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(3, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(4, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(5, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(6, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(7, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(8, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(9, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));

	return S_OK;
}

void CVikingShip::Release()
{

	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	for (int i = 0; i < 4; ++i)
	{
		ENGINE::SafeDeleteArray(m_pConvertVtx[i]); //여기고쳐야함

	}




}

CVikingShip* CVikingShip::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CVikingShip* pInstance = new CVikingShip(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);
	//if (dynamic_cast<ENGINE::CTransform*>(pInstance->GetComponent(L"Transform"))->GetAngle(ENGINE::ANGLE_Y) == 0.f ||
	//	dynamic_cast<ENGINE::CTransform*>(pInstance->GetComponent(L"Transform"))->GetAngle(ENGINE::ANGLE_Y) == 180.f)
	//	pInstance->m_eAxis = ENGINE::ANGLE_Z;
	//else
	//	pInstance->m_eAxis = ENGINE::ANGLE_X;

	return pInstance;
}

void CVikingShip::MoveViking()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	if (m_bDirCheck)
	{

		if (m_fPlayTime >= VIKING_PLAYING_TIME)
		{
			pTransform->SetAngle(ENGINE::ANGLE_X, pTransform->GetAngle(ENGINE::ANGLE_X) + m_fAngle * m_pTimeMgr->GetDeltaTime());
			if (fabsf(pTransform->GetAngle(ENGINE::ANGLE_X)) < 0.1f)
			{
				m_fPlayTime = 0.f;
				m_bDirCheck = false;
				m_fAngle = 0.f;
				pTransform->SetAngle(ENGINE::ANGLE_X, 0.f);
				Vec3 vPos = pTransform->GetPos();
				vPos.y = 0.f;
				dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(vPos);
				return;
			}
		}
		else
		{
			m_fPlayTime += m_pTimeMgr->GetDeltaTime();
			float fCosTime = cosf(D3DX_PI * m_fPlayTime);	//-1, 1사이의 값

			if (m_fPlayTime <= VIKING_PLAYING_TIME / 2.f)
				m_fAngle = fCosTime * (m_fPlayTime * m_pTimeMgr->GetDeltaTime() * 10.f);/// 7.5f);
			else
				m_fAngle = fCosTime * ((VIKING_PLAYING_TIME - m_fPlayTime) * m_pTimeMgr->GetDeltaTime() * 10.f); /// 7.5f);
			pTransform->SetAngle(ENGINE::ANGLE_X, pTransform->GetAngle(ENGINE::ANGLE_X) + m_fAngle);
			if (m_fPlayTime >= VIKING_PLAYING_TIME)
			{
				m_fAngle = -pTransform->GetAngle(ENGINE::ANGLE_X);
			}
		}
		/////////////////////////////////////////////////////////////////////////
		//if (m_fPlayTime >= 50)
		//{
		//	float fCorrection = 0.f - m_fAngle;
		//	fCorrection /= 10.f;
		//	m_fAngle += fCorrection;
		//	if (fCorrection <= 0.1f)
		//	{
		//		m_fAngle = 0.f;
		//		m_bDirCheck = false;
		//	}
		//
		//
		//}
		//else
		//{
		//	m_fPlayTime += m_pTimeMgr->GetDeltaTime();
		//
		//	if (m_fPlayTime <= 25.f)
		//		m_fValue += 0.08f * m_pTimeMgr->GetDeltaTime();
		//	else
		//		m_fValue -= 0.08f * m_pTimeMgr->GetDeltaTime();
		//	
		//	
		//	//if (fabs(m_fValue) <= -0.001)
		//	//	m_fValue = 0;
		//	
		//	m_fAngleTime = cosf(D3DX_PI * m_fPlayTime);
		//	m_fAngle += m_fAngleTime * m_fValue;
		//}
		//pTransform->SetAngle(ENGINE::ANGLE_X, m_fAngle);
		//
		////cout << "타임:" << m_fPlayTime << endl;
		//cout << "앵글:" << m_fAngle << endl;

	}
}

void CVikingShip::PlayViking()
{
	if (!m_bDirCheck)
	{
		m_bDirCheck = true;
		//CSoundMgr::GetInstance()->PlaySound(L"바이킹소리.mp3", CSoundMgr::CHANNELID::RIDES);
		//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.2f);
	}
}

void CVikingShip::PushSeat(int iIdx, CCustomer * pCustomer)
{
	pCustomer->RideSetting(Vec3(1.f, 0.f, 0.f));
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(iIdx, pCustomer);
}
