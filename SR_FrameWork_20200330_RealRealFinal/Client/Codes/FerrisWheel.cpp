#include "stdafx.h"
#include "FerrisWheel.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "FerrisBody.h"
#include "FerrisArm.h"
#include "FerrisLeg.h"
#include "FerrisSeat.h"
#include "Transform.h"
#include "SoundMgr.h"
#include "Queue.h"
#include "DeadObj.h"
#include "Seat.h"
#include "Customer.h"

CFerrisWheel::CFerrisWheel(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
	ZeroMemory(&m_bRideCheck, sizeof(m_bRideCheck));
}


CFerrisWheel::~CFerrisWheel()
{
	Release();
}

int CFerrisWheel::Update()
{
	

	m_pBody->Update();
	for (auto& iter : m_pVecArm)
	{
		iter->Update();
	}
	for (auto& iter : m_pVecLeg)
	{
		iter->Update();
	}
	ENGINE::CTransform* pBodyTransform = dynamic_cast<ENGINE::CTransform*>(m_pBody->GetComponent(L"Transform"));
	for (int i = 0; i < 8; ++i)
	{
		//0-359														0-359
		if ((int)pBodyTransform->GetAngle(ENGINE::ANGLE_Z) % 360 >= (7 - i) * 45 
			&& (int)pBodyTransform->GetAngle(ENGINE::ANGLE_Z) % 360 < (7 - i) * 45 + 5
			&& !m_bRideCheck[i])
		{
			m_bRideCheck[i] = true;
			//내리고
			CFerrisSeat* pFerrisSeat = m_pVecArm[i]->GetFerrisSeat();
			Vec3 vPos = m_tInfo.vPos;
			vPos.y = 0.f;
			dynamic_cast<CSeat*>(pFerrisSeat->GetComponent(L"Seat"))->ClearCustomer(vPos);

			//태우고
			CQueue* pQueue = dynamic_cast<CQueue*>(m_mapComponent[L"Queue"]);
			ENGINE::CGameObject* pCustomer = pQueue->PullCustomer();
			if (nullptr != pCustomer)
			{
				dynamic_cast<CCustomer*>(pCustomer)->RideSetting(Vec3(0.f, 0.f, -1.f));
				dynamic_cast<CSeat*>(pFerrisSeat->GetComponent(L"Seat"))->PushCustomer(0, dynamic_cast<CCustomer*>(pCustomer));
			}
		}
		else if ((int)pBodyTransform->GetAngle(ENGINE::ANGLE_Z) % 360 >= (7 - i) * 45 + 5)
		{
			m_bRideCheck[i] = false;
		}
	}
	return ENGINE::CGameObject::Update();;
}

void CFerrisWheel::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pBody->LateUpdate();
	for (auto& iter : m_pVecArm)
	{
		iter->LateUpdate();
	}
	for (auto& iter : m_pVecLeg)
	{
		iter->LateUpdate();
	}
}

void CFerrisWheel::Render()
{
	m_pBody->Render();
	for (auto& iter : m_pVecArm)
	{
		iter->Render();
	}
	for (auto& iter : m_pVecLeg)
	{
		iter->Render();
	}
}

HRESULT CFerrisWheel::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);



	//바디
	m_pBody = CFerrisBody::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize, 0);
	NULL_CHECK_RETURN(m_pBody, E_FAIL);

	//팔 +모양
	Vec3 vSize = VEC3::Normalize(D3DXVECTOR3(10.f, 10.f, 0.f)) * 10.f;
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ 0,-10,0 } *3.f, m_tInfo.vSize, 0, 180, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ vSize.x,-vSize.x,0 } *3.f, m_tInfo.vSize, 0, 225, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ 10,0,0 } *3.f, m_tInfo.vSize, 0, 270.f, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ vSize.x,vSize.x,0 } *3.f, m_tInfo.vSize, 0, 315, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ 0,10,0 } *3.f, m_tInfo.vSize, 0, 0, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ -vSize.x,vSize.x,0 } *3.f, m_tInfo.vSize, 0, 45, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ -10,0,0 } *3.f, m_tInfo.vSize, 0, 90, m_pBody));
	m_pVecArm.push_back(CFerrisArm::Create(m_pGraphicDev, D3DXVECTOR3{ -vSize.x,-vSize.x,0 } *3.f, m_tInfo.vSize, 0, 135, m_pBody));




	//다리																													  
	m_pVecLeg.push_back(CFerrisLeg::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize + D3DXVECTOR3{ 0.f,0.5f,0.f }, 0, 20));
	m_pVecLeg.push_back(CFerrisLeg::Create(m_pGraphicDev, m_tInfo.vPos, m_tInfo.vSize + D3DXVECTOR3{ 0.f,0.5f,0.f }, 0, 340));


	//CSoundMgr::GetInstance()->PlayBGM(L"관람차소리.mp3");
	//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);

	m_eRideType = ENGINE::RIDE_FERRIS;

	return S_OK;
}

HRESULT CFerrisWheel::AddComponent()
{

	ENGINE::CComponent* pComponent = nullptr;
	// Queue
	pComponent = CQueue::Create(m_tInfo.vPos, L"Ferris");
	NULL_CHECK_MSG_RETURN(pComponent, L"Queue Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Queue", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CFerrisWheel::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pBody);
	for_each(m_pVecArm.begin(), m_pVecArm.end(), ENGINE::SafeDelete<CFerrisArm*>);
	for_each(m_pVecLeg.begin(), m_pVecLeg.end(), ENGINE::SafeDelete<CFerrisLeg*>);

}

D3DXVECTOR3 CFerrisWheel::GetPos()
{
	return m_tInfo.vPos;
}

ENGINE::SAVE_INFO CFerrisWheel::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo;
	tInfo.iRideTag = 2;
	tInfo.vPos = m_tInfo.vPos;
	tInfo.vSize = m_tInfo.vSize;
	tInfo.fAngleY = m_fAngleY;
	return tInfo;
}

CFerrisWheel * CFerrisWheel::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFerrisWheel* pInstance = new CFerrisWheel(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;
	pInstance->m_fAngleY=fAngle;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);


	return pInstance;
}
