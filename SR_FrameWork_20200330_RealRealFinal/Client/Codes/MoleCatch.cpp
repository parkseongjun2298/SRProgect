#include "stdafx.h"
#include "MoleCatch.h"
#include "Mole.h"
#include "CubeTex.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CameraObserver.h"
#include "UiMoleCount.h"
#include "UiMoleTime.h"
#include "UiMoleStart.h"
#include "UiMoleEnd.h"

#include "MoleMousePos.h"
#include "SoundMgr.h"

CMoleCatch::CMoleCatch(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr),
	m_fConvertTime(0.f), m_fCoolTime(0.f),
	m_fPrevTime(0.f), m_fTotalTime(0.f), m_iCatchCount(0), m_fRemainingTime(0.f)
{
	m_vecMole.reserve(9);
}


CMoleCatch::~CMoleCatch()
{
	Release();
}

int CMoleCatch::Update()
{
	ENGINE::CGameObject::Update();
	
	MoleCatchPlay();
	KeyInput();

	// 플레이타임이 끝나면, GameOver씬을 출력.
	if (m_fConvertTime > 30.f && !m_bRenderEnd)
	{
		CSoundMgr::GetInstance()->PlaySound(L"종료.mp3", CSoundMgr::CHANNELID::UI);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::UI, 0.4f);
		m_bRenderEnd = true;
	}

	for (auto& pMole : m_vecMole)
	{
		pMole->Update();
	}
	m_pUiMoleCount->Update();
	m_pUiMoleTime->Update();
	m_pMoleMousePos->Update();
	m_pUiMoleStart->Update();

	if (m_bRenderEnd)
		m_pUiMoleEnd->Update();
	return NO_EVENT;
}

void CMoleCatch::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();

	for (auto& pMole : m_vecMole)
	{
		pMole->LateUpdate();
	}
	m_pUiMoleCount->LateUpdate();
	m_pUiMoleTime->LateUpdate();
	m_pMoleMousePos->LateUpdate();
	m_pUiMoleStart->LateUpdate();

	if (m_bRenderEnd)
		m_pUiMoleEnd->LateUpdate();
}

void CMoleCatch::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 8; ++i)
	{
		// 로컬 -> 뷰 스페이스
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// 투영스페이스 전 예외처리
		if (1.f > m_pConvertVtx[i].vPos.z || 1000.f < m_pConvertVtx[i].vPos.z)
			continue;

		// 뷰 -> 투영스페이스
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());
	}
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Buffer"]->Render();
	m_mapComponent[L"Texture"]->Render();

	for (auto& pMole : m_vecMole)
	{
		pMole->Render();
	}
	// 클릭을 해서 게임이 시작했을 경우 렌더하지않게 함.
	if (!m_bRenderStart)
		m_pUiMoleStart->Render();

	// 게임이 끝났을 시, 게임오버를 보여줌.
	if (m_bRenderEnd)
		m_pUiMoleEnd->Render();

	m_pUiMoleCount->Render();
	m_pUiMoleTime->Render();
	m_pMoleMousePos->Render();
}

HRESULT CMoleCatch::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	
	// 시간 및 멤버변수
	m_fTotalTime = 0.f;	// 플레이할 총 시간.(60초)
	m_fCoolTime = 1.f;		// 두더지가 올라올 주기타임(쿨타임).

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	D3DXVECTOR3 vPos = pTransform->GetPos();

	CMole* pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x - 2.f, vPos.y, vPos.z + 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole00 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x, vPos.y, vPos.z + 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole01 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x + 2.f, vPos.y, vPos.z + 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole02 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x - 2.f, vPos.y, vPos.z));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole03 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x, vPos.y, vPos.z));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole04 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x + 2.f, vPos.y, vPos.z));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole05 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x - 2.f, vPos.y, vPos.z - 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole06 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x, vPos.y, vPos.z - 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole07 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	pMole = CMole::Create(m_pGraphicDev, D3DXVECTOR3(vPos.x + 2.f, vPos.y, vPos.z - 2.f));
	NULL_CHECK_MSG_RETURN(pMole, L"Mole08 Create Failed", E_FAIL);
	m_vecMole.emplace_back(pMole);

	// UiMoleCount
	m_pUiMoleCount = CUiMoleCount::Create(m_pGraphicDev, { -640.f, WINCY* 0.5f, 0.f }, { 416.f, 113.f, 0.f }, L"Texture_UiMoleCount");
	NULL_CHECK_MSG_RETURN(m_pUiMoleCount, L"UiMoleCount Create Failed", E_FAIL);
	m_pUiMoleCount->Notify_Count(m_iCatchCount);

	// UiMoleTime
	m_pUiMoleTime = CUiMoleTime::Create(m_pGraphicDev, { 224.f, WINCY* 0.5f, 0.f }, { 416.f, 113.f, 0.f }, L"Texture_UiMoleTime");
	NULL_CHECK_MSG_RETURN(m_pUiMoleCount, L"UiMoleTime Create Failed", E_FAIL);
	m_pUiMoleTime->Notify_Time(30);

	// UiMoleMouse
	m_pMoleMousePos = CMoleMousePos::Create(m_pGraphicDev);
	NULL_CHECK_MSG_RETURN(m_pMoleMousePos, L"UiMoleMouse Create Failed", E_FAIL);

	// UiMoleStart
	m_pUiMoleStart = CUiMoleStart::Create(m_pGraphicDev, { -200.f, WINCY* 0.5f, 0.f }, { 796.f, 251.f, 0.f }, L"Texture_UiMoleStart");
	NULL_CHECK_MSG_RETURN(m_pUiMoleStart, L"UiMoleStart Create Failed", E_FAIL);

	// UiMoleEnd
	m_pUiMoleEnd = CUiMoleEnd::Create(m_pGraphicDev, { -200.f, WINCY* 0.5f, 0.f }, { 184.f, 56.f, 0.f }, L"Texture_UiMoleEnd");
	NULL_CHECK_MSG_RETURN(m_pUiMoleEnd, L"UiMoleEnd Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CMoleCatch::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_MoleCatch");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_MoleCatch Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_MoleCatch");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_MoleCatch Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = m_vPos;
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"MoleCatch Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"MoleCatch Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	return S_OK;
}

void CMoleCatch::Release()
{
	CSoundMgr::GetInstance()->StopAll();
	for_each(m_vecMole.begin(), m_vecMole.end(), ENGINE::SafeDelete<CMole*>);
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	ENGINE::SafeDelete(m_pUiMoleCount);
	ENGINE::SafeDelete(m_pUiMoleTime);
	ENGINE::SafeDelete(m_pMoleMousePos);
	ENGINE::SafeDelete(m_pUiMoleStart);
	ENGINE::SafeDelete(m_pUiMoleEnd);
}

CMoleCatch * CMoleCatch::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CMoleCatch* pInstance = new CMoleCatch(pGraphicDev);
	pInstance->m_vPos = vPos;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}

void CMoleCatch::MoleCatchPlay()
{
	// 플레이시간이 총플레이시간을 넘겼다면 중지.
	if (m_fConvertTime >= m_fTotalTime)
		return;

	m_fRemainingTime += m_pTimeMgr->GetDeltaTime();
	if (1.f <= m_fRemainingTime)
	{
		m_pUiMoleTime->Notify_Time((int)(m_fTotalTime - m_fConvertTime));
		m_fRemainingTime = 0.f;
	}

	m_fConvertTime += m_pTimeMgr->GetDeltaTime();	// 플레이시간 측정.
	
	// 쿨타임 시간보다, 현재시간 - 최근의올린 두더지 시간이 더 클경우 실행.
	if (m_fCoolTime < m_fConvertTime - m_fPrevTime)
	{

		// 랜덤하게 올라올 준비.
		int iRandomIndex = 0;
		int iCount = 0;
		while (true)
		{			
			iRandomIndex = rand() % 9;

			// 만약에 움직이지 않았다면 움직인다. 
			if (!m_vecMole[iRandomIndex]->GetMolePlay())
			{
				m_vecMole[iRandomIndex]->SetMolePlay(true);
				m_vecMole[iRandomIndex]->SetMoleAngry(MoleAngry());

				m_fPrevTime = m_fConvertTime;	// 두더지 올린 시간 저장.

				// 7초마다 쿨타임 감소.
				int iCool = (int)m_fConvertTime / 7;
				switch (iCool)
				{
				case 1:
					if (0.7f <= m_fCoolTime)
						m_fCoolTime -= 0.5f * m_pTimeMgr->GetDeltaTime();
					else
						m_fCoolTime = 0.7f;
					break;
				case 2:
					if (0.5f <= m_fCoolTime)
						m_fCoolTime -= 0.5f * m_pTimeMgr->GetDeltaTime();
					else
						m_fCoolTime = 0.5f;
					break;
				case 3:
					if (0.3f <= m_fCoolTime)
						m_fCoolTime -= 0.5f * m_pTimeMgr->GetDeltaTime();
					else
						m_fCoolTime = 0.3f;
					break;
				}

				break;
			}
			else
			{
				// 예외처리 : 전부 다 true일때도 빠져나올 수 있게끔.
				for (auto& pMole : m_vecMole)
				{
					if (pMole->GetMolePlay())
						iCount++;
				}
				// 카운트가 9개라는 말은 모두 올라오고있는 상태 혹은, 올라와있는 상태라는 말이다.
				// 즉, 올릴 것이 없으므로 while문 빠져나오기.
				if (iCount >= 9)
					break;
				else
					iCount = 0;
			}
		}
	}
}

bool CMoleCatch::MoleAngry()
{
	// 20퍼 확률.
	int iRandom = (rand() % 10 ); // 0 ~ 9

	if(iRandom >= 8.f)
		return true;
	return false;
}


void CMoleCatch::KeyInput()
{
	// 마우스 왼쪽클릭을 할 경우
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_LBUTTON))
	{
		//CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->PlaySound(L"퍽.mp3", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::EFFECT, 0.4f);

		// 모든 두더지들을 순회
		for (auto& pMole : m_vecMole)
		{
			m_pMoleMousePos->Set_AttHammer(true);

			// 올라와 있는 두더지는
			if (pMole->GetMolePlay())
			{
				// 올라와있는 두더지가 충돌판정이 되어있지않는 경우에만(충돌처리가 이미끝난 두더지는 할 필요가 x)
				if (!pMole->GetMoleHit())
				// 충돌판정여부를 조사한다.
					pMole->MoleHit();
			}

			// 두더지 잡을때 점수판단
			if (pMole->GetMoleHit())
			{
				// 화난두더지일 경우
				if (pMole->GetMoleAngry())
				{
					m_iCatchCount -= 3;
					if (0 > m_iCatchCount)
						m_iCatchCount = 0;
				}
				else // 일반두더지일 경우
					++m_iCatchCount;

				m_pUiMoleCount->Notify_Count(m_iCatchCount);
			}
		}
	}

	// 시작.
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_RBUTTON))
	{
		m_fConvertTime = 0.f;
		m_fTotalTime = 30.f;
		m_bRenderStart = true;
	}

}
