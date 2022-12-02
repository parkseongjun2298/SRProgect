#include "stdafx.h"
#include "GyroDrop.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"GyroDropBody.h"
#include"GyroDropDevice.h"
#include "SoundMgr.h"
#include"Queue.h"
#include "DeadObj.h"
#include "Customer.h"
CGyroDrop::CGyroDrop(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_bPlay(false), Temp(0.f)

{

}


CGyroDrop::~CGyroDrop()
{
	Release();
}

int CGyroDrop::Update()
{

	if (m_pVecGyroDevice.front()->GetPlayGyro() == false)
	{

		//CSoundMgr::GetInstance()->PlaySound(L"자이로드롭소리.mp3", CSoundMgr::CHANNELID::RIDES);
		//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 7; ++j)
			{
				CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
				ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
				if (nullptr != pCustomer)
					m_pVecGyroDevice[i]->PushSeat(j, dynamic_cast<CCustomer*>(pCustomer));

			}
		}

		//Temp += m_pTimeMgr->GetDeltaTime();
		//if (Temp >= 2.f)
		//{
		//	m_bPlay = true;
		//	Temp = 0.f;
		//}
		m_bPlay = true;
	}
	for (auto& iter : m_pVecGyroDevice)
	{
		if (m_bPlay)
			iter->PlayGyro();
		iter->Update();
	}
	m_bPlay = false;
	m_pGyroDropBody->Update();



	return ENGINE::CGameObject::Update();;
}

void CGyroDrop::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pGyroDropBody->LateUpdate();
	for (auto& iter : m_pVecGyroDevice)
	{
		iter->LateUpdate();
	}
}

void CGyroDrop::Render()
{

	m_pGyroDropBody->Render();
	for (auto& iter : m_pVecGyroDevice)
	{
		iter->Render();
	}
}

HRESULT CGyroDrop::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	m_pGyroDropBody = CGyroDropBody::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_RETURN(m_pGyroDropBody, E_FAIL);

	m_pVecGyroDevice.push_back(CGyroDropDevice::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f,-20.f,-10.f }, m_tInfo.vSize, 0, m_pGyroDropBody));
	m_pVecGyroDevice.push_back(CGyroDropDevice::Create(m_pGraphicDev, D3DXVECTOR3{ -10.f,-20.f,0.f }, m_tInfo.vSize, 90, m_pGyroDropBody));

	m_pVecGyroDevice.push_back(CGyroDropDevice::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f,-20.f,10.f }, m_tInfo.vSize, 0, m_pGyroDropBody));
	m_pVecGyroDevice.push_back(CGyroDropDevice::Create(m_pGraphicDev, D3DXVECTOR3{ 10.f,-20.f,0.f }, m_tInfo.vSize, 90, m_pGyroDropBody));


	m_eRideType = ENGINE::RIDE_GYRODROP;

	return S_OK;
}

HRESULT CGyroDrop::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;
	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"GyroDrop");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));


	return S_OK;
}

void CGyroDrop::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pGyroDropBody);
	for_each(m_pVecGyroDevice.begin(), m_pVecGyroDevice.end(), ENGINE::SafeDelete<CGyroDropDevice*>);

}

D3DXVECTOR3 CGyroDrop::GetPos()
{
	return m_tInfo.vPos;
}

ENGINE::SAVE_INFO CGyroDrop::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 3;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CGyroDrop * CGyroDrop::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGyroDrop* pInstance = new CGyroDrop(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;
	pInstance->m_fAngleY = fAngle;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0);


	return pInstance;
}
