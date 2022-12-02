#include "stdafx.h"
#include "GyroDropDevice.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "Seat.h"
#include "Customer.h"

CGyroDropDevice::CGyroDropDevice(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()), m_fSpeed(0), m_fRebound(0)
	, m_bCheckTop(false), m_bRotateCheck(false), m_bCheckBot(false), m_bPlay(false)
{
	Temp = { 0.f,-30.f,0.f };
}


CGyroDropDevice::~CGyroDropDevice()
{
	Release();
}

int CGyroDropDevice::Update()
{
	ENGINE::CGameObject::Update();

	GyroStart(); 
	Matrix matParentMatrix = MATRIX::Identity();
	//Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CGyroDropDevice::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetParentWorldMatrix(m_pParent->GetWorldMatrix());
}

void CGyroDropDevice::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	Matrix matParent = m_pParent->GetWorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(pTransform->GetWorldMatrix()));// *matParent));

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
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroDevice"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_GyroDevice"]->Render();

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render((pTransform->GetWorldMatrix()));
}

HRESULT CGyroDropDevice::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//왼쪽
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroDevice"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	m_eRideType = ENGINE::RIDE_GYRODROP;

	return S_OK;
}

HRESULT CGyroDropDevice::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_GyroDevice");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_GyroDevice Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_GyroDevice", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_GyroDevice");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_GyroDevice Clone Failed", E_FAIL);
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
	pComponent = CSeat::Create(m_pGraphicDev, 7);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(1, Vec3(2.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(2, Vec3(4.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(3, Vec3(-2.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(4, Vec3(-4.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(5, Vec3(6.f, 2.f, 0.f)); // 로컬
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(6, Vec3(-6.f, 2.f, 0.f)); // 로컬

																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(1, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(2, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(3, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(4, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(5, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
																		   //dynamic_cast<CSeat*>(pComponent)->PushCustomer(6, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));

	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));

	return S_OK;
}

void CGyroDropDevice::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CGyroDropDevice * CGyroDropDevice::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, ENGINE::CGameObject* pBody)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGyroDropDevice* pInstance = new CGyroDropDevice(pGraphicDev);
	pInstance->m_pParent = pBody;
	pInstance->m_fAngle = fAngleY;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}

void CGyroDropDevice::GyroStart()
{
	if (!m_bPlay)
		return;
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	//올라가는부분
	if (!m_bCheckTop && !m_bCheckBot)
	{
		pTransform->MoveShift(D3DXVECTOR3{ 0.f,7.f,0.f } * m_pTimeMgr->GetDeltaTime());

	}
	if (pTransform->GetPos().y >= 14.f)
	{
		D3DXVECTOR3 vPos = pTransform->GetPos();
		vPos.y = 14.f;
		pTransform->SetPos(vPos);
		m_bCheckTop = true;
	}

	if (m_bCheckTop)
	{
		if (!m_bRotateCheck)
		{
			//자전
			//pTransform->MoveAngle(ENGINE::ANGLE_Y, m_fSpeed);
			m_fSpeed += 0.1f * m_pTimeMgr->GetInstance()->GetDeltaTime();
		}
		if (m_fSpeed >= 1)
		{
			m_bRotateCheck = true;
			m_fSpeed = 0;
		}
		//전체가같이돌아가야함

		//떨어지는부분
		if (m_bRotateCheck && !m_bCheckBot)
		{

			Temp.y -= 60.f * m_pTimeMgr->GetInstance()->GetDeltaTime();
			pTransform->MoveShift(Temp * m_pTimeMgr->GetDeltaTime());

			if (pTransform->GetPos().y <= -18.f)
			{
				D3DXVECTOR3 vPos = pTransform->GetPos();
				vPos.y = -18.f;
				pTransform->SetPos(vPos);
				m_bCheckBot = true;
				m_bCheckTop = false;
			}

		}
	}

	if (!m_bCheckTop && m_bCheckBot)
	{
		m_fRebound += 1 * m_pTimeMgr->GetInstance()->GetDeltaTime();
		if (m_fRebound < 0.5f)
			pTransform->MoveShift(D3DXVECTOR3{ 0.f,2.f,0.f } * m_pTimeMgr->GetDeltaTime());

		else
		{
			pTransform->MoveShift(D3DXVECTOR3{ 0.f,-2.f,0.f } * m_pTimeMgr->GetDeltaTime());

			if (pTransform->GetPos().y <= -18.f)
			{
				D3DXVECTOR3 vPos = pTransform->GetPos();
				vPos.y = -18.f;
				pTransform->SetPos(vPos);

				m_bCheckBot = false;
				m_bRotateCheck = false;
				m_fRebound = 0;
				m_bPlay = false;

				dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(vPos);
			}
		}
	}

	//	cout << pTransform->GetPos().y << endl;

}

void CGyroDropDevice::PlayGyro()
{
	m_bPlay = true;
}

void CGyroDropDevice::PushSeat(int iIdx, CCustomer * pCustomer)
{
	pCustomer->RideSetting(Vec3(0.f, 0.f, -1.f));
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(iIdx, pCustomer);
}
