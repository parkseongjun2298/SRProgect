#include "stdafx.h"
#include "BumperCar.h"
#include "CubeTex.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CameraObserver.h"

#include "CollisionMgr.h"
#include "SuperCar.h"
#include "MyCar.h"
#include "AutoCar.h"

#include "BumperCarField.h"
#include "BumperCarMainField.h"

// Ui
#include "BumperLife.h"
#include "BumperStart.h"
#include "BumperEnd.h"
#include "SoundMgr.h"

CBumperCar::CBumperCar(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev)
{
}


CBumperCar::~CBumperCar()
{
	Release();
}

int CBumperCar::Update()
{
	KeyInput();
	Check_Life();
	Check_Ready();
	if (m_bIsStart)
	{
		if (!m_bIsGameOver)
		{
			Check_GameOver();
			m_pField->Update();
			m_pMainField->Update();
			for (auto& iter : m_listBumperLife)
				iter->Update();
		}
	}
	if (m_bIsReady && !m_bIsStart)
		m_pUiStart->Update();

	if (m_bIsGameOver)
		m_pUiEnd->Update();

	return NO_EVENT;
}

void CBumperCar::LateUpdate()
{
	//CGameObject::LateUpdate();
	m_pField->LateUpdate();
	m_pMainField->LateUpdate();

	for (auto& iter : m_listBumperLife)
		iter->LateUpdate();

	m_pUiStart->LateUpdate();

	if (m_bIsGameOver)
		m_pUiEnd->LateUpdate();
}

void CBumperCar::Render()
{
	m_pField->Render();
	m_pMainField->Render();

	for (auto& iter : m_listBumperLife)
		iter->Render();

	m_pUiStart->Render();

	if (m_bIsGameOver)
		m_pUiEnd->Render();
}

HRESULT CBumperCar::Initialize()
{
	m_pField = CBumperCarField::Create(m_pGraphicDev, { -10.f, 0.f, -25.f }, { 80.f, 1.f, 80.f }, 0.f);
	NULL_CHECK_RETURN(m_pField, E_FAIL);

	m_pMainField = CBumperCarMainField::Create(m_pGraphicDev, { -10.f, 0.f, -25.f }, { 80.f, 1.f, 80.f }, 0.f);
	NULL_CHECK_RETURN(m_pMainField, E_FAIL);

	// Life 1 (17.f, 15.f)원본사이즈
	CBumperLife* pLife = CBumperLife::Create(m_pGraphicDev, { -640.f, WINCY* 0.5f, 0.f }, { 68.f, 60.f, 0.f }, L"Texture_BumperCarLife");
	NULL_CHECK_RETURN(pLife, E_FAIL);
	m_listBumperLife.emplace_back(pLife);

	// Life 2
	pLife = CBumperLife::Create(m_pGraphicDev, { -558.f, WINCY* 0.5f, 0.f }, { 68.f, 60.f, 0.f }, L"Texture_BumperCarLife");
	NULL_CHECK_RETURN(pLife, E_FAIL);
	m_listBumperLife.emplace_back(pLife);

	// Life 3
	pLife = CBumperLife::Create(m_pGraphicDev, { -476.f, WINCY* 0.5f, 0.f }, { 68.f, 60.f, 0.f }, L"Texture_BumperCarLife");
	NULL_CHECK_RETURN(pLife, E_FAIL);
	m_listBumperLife.emplace_back(pLife);

	// StartUi (52.f, 68.f)
	m_pUiStart = CBumperStart::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 808.f, 634.f, 0.f }, L"Texture_BumperCarStart");
	NULL_CHECK_RETURN(m_pUiStart, E_FAIL);

	// EndUi
	m_pUiEnd = CBumperEnd::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 402.f, 204.f, 0.f }, L"Texture_BumperCarVictory");
	NULL_CHECK_RETURN(m_pUiEnd, E_FAIL);

	return S_OK;
}

HRESULT CBumperCar::AddComponent()
{
	return S_OK;
}

void CBumperCar::Release()
{
	ENGINE::SafeDelete(m_pField);
	ENGINE::SafeDelete(m_pMainField);
	ENGINE::SafeDelete(m_pUiStart);
	ENGINE::SafeDelete(m_pUiEnd);
	for_each(m_listBumperLife.begin(), m_listBumperLife.end(), ENGINE::SafeDelete<CBumperLife*>);

}

void CBumperCar::Check_Life()
{
	// 아무것도 없을때도 수행되는것을 방지하기위해 예외처리.
	if (m_listBumperLife.empty())
		return;

	// 플레이어의 Hp가 표시되는 하트의 개수보다 작을 경우 똑같이 갯수 맞춰줌.
	if ((size_t)m_pField->Get_PlayerHp() < m_listBumperLife.size())
	{
		ENGINE::SafeDelete(m_listBumperLife.back());
		m_listBumperLife.pop_back();
	}
}

void CBumperCar::KeyInput()
{
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_RBUTTON))
	{
		if (!m_bIsReady)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->PlaySound(L"카운트.mp3", CSoundMgr::CHANNELID::RIDES);
			m_pUiStart->Set_Count(true);
			m_pUiStart->Set_Quest(false);
			m_pUiStart->QuestSize();
		}

		m_bIsReady = true;
	}
}

void CBumperCar::Check_Ready()
{
	// 모든 시작연출이 끝났는 말로 게임을시작하면됨
	if (m_pUiStart->Get_GameStart())
		m_bIsStart = true;

	if (!m_bIsUiStart && m_bIsStart)
	{
		CSoundMgr::GetInstance()->PlayBGM(L"범퍼카BGM.mp3");
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.4f);
		m_bIsUiStart = true;
	}
}

void CBumperCar::Check_GameOver()
{
	if (m_pField->Get_PlayerHp() <= 0.f)
	{
		m_pUiEnd->Set_Fail(true);
		m_bIsGameOver = true;
		CSoundMgr::GetInstance()->StopAll();
		CSoundMgr::GetInstance()->PlaySound(L"범퍼카패배.mp3", CSoundMgr::CHANNELID::EFFECT);
	}
	if (m_pField->Get_AiCount() <= 0.f)
	{
		m_pUiEnd->Set_Victory(true);
		m_bIsGameOver = true;
		CSoundMgr::GetInstance()->StopAll();
		CSoundMgr::GetInstance()->PlaySound(L"범퍼카승리.mp3", CSoundMgr::CHANNELID::EFFECT);
	}
}

CBumperCar * CBumperCar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CBumperCar* pBumperCar = new CBumperCar(pGraphicDev);
	if (FAILED(pBumperCar->Initialize()))
	{
		ENGINE::SafeDelete(pBumperCar);
		return nullptr;
	}
	pBumperCar->SetCreatePosSize(vPos, vSize, fAngle);

	return pBumperCar;
}
