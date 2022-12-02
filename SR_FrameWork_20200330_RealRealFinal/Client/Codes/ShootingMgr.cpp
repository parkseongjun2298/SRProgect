#include "stdafx.h"
#include "ShootingMgr.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include"ShootingMonster.h"
#include"ShootingPlayer.h"
#include"ShootingCount.h"
#include"ShootingTime.h"
#include"ShootingStart.h"
#include"ShootingOver.h"
#include"CrossHair.h"
#include"ShootingGun.h"
#include"ShootingEffect.h"
#include "Transform.h"
#include "SoundMgr.h"
#include"CollisionMgr.h"

CShootingMgr::CShootingMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()), m_fTime(30.f), m_iScore(0), m_bStart(false)
{

}


CShootingMgr::~CShootingMgr()
{
	Release();
}

int CShootingMgr::Update()
{
	ENGINE::CGameObject::Update();
	m_pShootingCount->Update();
	m_pShootingTime->Update();
	m_pShootingStart->Update();
	m_pShootingOver->Update();
	m_pCrossHair->Update();
	m_pShootingGun->Update();
	m_pShootingEffect->Update();
	m_pPlayer->Update();

	list<CShootingMonster*>::iterator iter_begin = m_lMonster.begin();
	list<CShootingMonster*>::iterator iter_end = m_lMonster.end();

	Start_Game();
	for (; iter_begin != iter_end;)
	{
		int iEvent = (*iter_begin)->Update();

		if (DEAD_OBJ == iEvent)
		{
			if (m_fTime > 0.f)
			{
				m_iScore += (int)((*iter_begin)->Get_Score());
			}
			ENGINE::SafeDelete(*iter_begin);
			iter_begin = m_lMonster.erase(iter_begin);


		}
		else
			++iter_begin;
	}

	if (m_lMonster.empty() && m_fTime>0.f)
	{
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,0,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,5,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,10,10 }));

		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,0,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,5,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,10,10 }));

		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,0,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,5,10 }));
		m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,10,10 }));
	}




	return NO_EVENT;
}

void CShootingMgr::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	m_pShootingCount->LateUpdate();
	m_pShootingTime->LateUpdate();
	m_pShootingStart->LateUpdate();
	m_pShootingOver->LateUpdate();
	m_pPlayer->LateUpdate();
	m_pCrossHair->LateUpdate();
	m_pShootingGun->LateUpdate();
	m_pShootingEffect->LateUpdate();
	for (auto& iter : m_lMonster)
	{
		iter->LateUpdate();
	}
	CCollisionMgr::GetInstance()->Collision_BulletToMonster(m_pPlayer->Get_Bullet(), &m_lMonster);


}

void CShootingMgr::Render()
{

	m_pShootingCount->Render();
	m_pShootingTime->Render();
	m_pCrossHair->Render();
	m_pShootingGun->Render();

	m_pShootingStart->Render();
	m_pShootingOver->Render();
	m_pPlayer->Render();
	if (ENGINE::CKeyMgr::GetInstance()->KeyPressing(ENGINE::KEY_LBUTTON))
	{
		m_pShootingEffect->Render();
	}

	for (auto& iter : m_lMonster)
	{
		iter->Render();
	}


}

HRESULT CShootingMgr::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	//count
	m_pShootingCount = CShootingCount::Create(m_pGraphicDev, { -640.f, WINCY* 0.5f, 0.f }, { 416.f, 113.f, 0.f }, L"Texture_ShootingCount");
	m_pShootingCount->Notify_Count(m_iScore);
	//UiMoleTime
	m_pShootingTime = CShootingTime::Create(m_pGraphicDev, { 224.f, WINCY* 0.5f, 0.f }, { 416.f, 113.f, 0.f }, L"Texture_ShootingTime");
	m_pShootingTime->Notify_Time((int)m_fTime);
	//Start
	m_pShootingStart = CShootingStart::Create(m_pGraphicDev, { -200.f, WINCY* 0.5f, 0.f }, { 416.f, 113.f, 0.f }, L"Texture_ShootingStart");
	//Over
	m_pShootingOver = CShootingOver::Create(m_pGraphicDev, { -910.f, WINCY* 0.5f + 1000.f, 0.f }, { 1800.f, 1000.f, 0.f }, L"Texture_ShootingOver");
	m_pShootingOver->Notify_Count(m_iScore);
	//gun
	m_pShootingGun = CShootingGun::Create(m_pGraphicDev, { 350.f, WINCY* 0.5f - 420.f, 0.f }, { 300.f, 300.f, 0.f }, L"Texture_ShootingGun");
	//Effect
	m_pShootingEffect = CShootingEffect::Create(m_pGraphicDev, { 220.f, WINCY* 0.5f - 300.f , 0.f }, { 150.f, 150.f, 0.f }, L"Texture_ShootingEffect");

	//플레이어
	m_pPlayer = CShootingPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

	//몬스터
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,0,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,5,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 0,10,10 }));

	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,0,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,5,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ -10,10,10 }));

	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,0,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,5,10 }));
	m_lMonster.emplace_back(CShootingMonster::Create(m_pGraphicDev, D3DXVECTOR3{ 10,10,10 }));

	//cross
	m_pCrossHair = CCrossHair::Create(m_pGraphicDev, { 0.f - 50.f, 0.f + 50.f, 0.f }, { 100.f, 100.f, 0.f }, L"Texture_CrossHair");

	/*CSoundMgr::GetInstance()->PlayBGM(L"관람차소리.mp3");
	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);*/

	return S_OK;
}

HRESULT CShootingMgr::AddComponent()
{
	return S_OK;
}

void CShootingMgr::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pShootingCount);
	ENGINE::SafeDelete(m_pShootingTime);
	ENGINE::SafeDelete(m_pPlayer);
	ENGINE::SafeDelete(m_pShootingStart);
	ENGINE::SafeDelete(m_pShootingOver);
	ENGINE::SafeDelete(m_pCrossHair);
	ENGINE::SafeDelete(m_pShootingGun);
	ENGINE::SafeDelete(m_pShootingEffect);
	for_each(m_lMonster.begin(), m_lMonster.end(), ENGINE::SafeDelete<CShootingMonster*>);


}

CShootingMgr * CShootingMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShootingMgr* pInstance = new CShootingMgr(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);


	return pInstance;
}

void CShootingMgr::Start_Game()
{

	if (ENGINE::CKeyMgr::GetInstance()->KeyPressing(ENGINE::KEY_RBUTTON))
	{
		m_bStart = true;

	}


	if (m_bStart)
	{
		m_fTime -= 1 * m_pTimeMgr->GetDeltaTime();
		m_pShootingTime->Notify_Time((int)m_fTime);
		m_pShootingCount->Notify_Count(m_iScore);
		m_pShootingOver->Notify_Count(m_iScore);
		if (m_fTime <= 0)
			m_fTime = 0;

	}

}


