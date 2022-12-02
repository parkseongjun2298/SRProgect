#include "stdafx.h"
#include "GyroSwing.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

#include"GyroSwingBody.h"
#include"GyroSwingLeg.h"
#include"GyroSwingDevice.h"
#include"GyroSwingSpin.h"
#include"Queue.h"
#include "DeadObj.h"
#include "Customer.h"
CGyroSwing::CGyroSwing(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_bRideCheck(false)

{
}


CGyroSwing::~CGyroSwing()
{
	Release();
}

int CGyroSwing::Update()
{


	m_pBody->Update();
	for (auto& iter : m_vLeg)
	{
		iter->Update();
	}
	
	if (-1 == m_pDevice->Update())
		m_bRideCheck = false;
	m_pDevice->PlayGyroSwing();
	for (auto& iter : m_vSpin)
	{
		iter->Update();
	}
	if (m_pDevice->Get_FinCheck() == false && m_bRideCheck == false)
	{

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
				ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
				if (nullptr != pCustomer)
					m_vSpin[i]->PushSeat(j, dynamic_cast<CCustomer*>(pCustomer));

				
			}
		}
		m_bRideCheck = true;
	}




	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	float fRotateSpeed = 90.f * m_pTimeMgr->GetDeltaTime();
	pTransform->SetAngle(ENGINE::ANGLE_Y, pTransform->GetAngle(ENGINE::ANGLE_Y) + fRotateSpeed);


	return ENGINE::CGameObject::Update();
}

void CGyroSwing::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pBody->LateUpdate();
	for (auto& iter : m_vLeg)
	{
		iter->LateUpdate();
	}
	m_pDevice->LateUpdate();

	for (auto& iter : m_vSpin)
	{
		iter->LateUpdate();
	}
}

void CGyroSwing::Render()
{
	m_pBody->Render();
	for (auto& iter : m_vLeg)
	{
		iter->Render();
	}
	m_pDevice->Render();

	for (auto& iter : m_vSpin)
	{
		iter->Render();
	}
}

HRESULT CGyroSwing::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	//바디
	m_pBody = CGyroSwingBody::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize, 0);
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	//다리																													  
	m_vLeg.push_back(CGyroSwingLeg::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize + D3DXVECTOR3{ 0.f,0.5f,0.f }, 0, 0, 30));

	m_vLeg.push_back(CGyroSwingLeg::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize + D3DXVECTOR3{ 0.f,0.5f,0.f }, 0, 0, 330));

	//Device
	m_pDevice = CGyroSwingDevice::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize, 0);

	//spin

	m_vSpin.push_back(CGyroSwingSpin::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f, -20.f , -8.f }, m_tInfo.vSize, 90, m_pDevice));
	m_vSpin.push_back(CGyroSwingSpin::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f, -20.f, 8.f }, m_tInfo.vSize, 90, m_pDevice));

	m_vSpin.push_back(CGyroSwingSpin::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f, -20.f, 8.f }, m_tInfo.vSize, 0, m_pDevice));
	m_vSpin.push_back(CGyroSwingSpin::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f, -20.f, -8.f }, m_tInfo.vSize, 0, m_pDevice));


	m_eRideType = ENGINE::RIDE_GYROSWING;

	return  S_OK;;
}

HRESULT CGyroSwing::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;
	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));


	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"GyroSwing");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CGyroSwing::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pBody);
	for_each(m_vLeg.begin(), m_vLeg.end(), ENGINE::SafeDelete<CGyroSwingLeg*>);
	ENGINE::SafeDelete(m_pDevice);
	for_each(m_vSpin.begin(), m_vSpin.end(), ENGINE::SafeDelete<CGyroSwingSpin*>);

}

D3DXVECTOR3 CGyroSwing::GetPos()
{
	return m_tInfo.vPos;
}

ENGINE::SAVE_INFO CGyroSwing::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 6;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CGyroSwing * CGyroSwing::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGyroSwing* pInstance = new CGyroSwing(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;
	pInstance->m_fAngleY = fAngleY;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}
