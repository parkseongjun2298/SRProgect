


#include "stdafx.h"
#include "BumperCarField.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "Texture.h"
#include "PipeLine.h"
#include "RcTexLying.h"
#include "Frame.h"
//#include "RcTex.h"

#include "CollisionMgr.h"

#include "SuperCar.h"
#include "AutoCar.h"
#include "MyCar.h"

#include "SoundMgr.h"

CBumperCarField::CBumperCarField(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_fFindTargetTime(2.f), m_iAiCount(8)
{
	//m_vecBumperCar.reserve(2);
	m_vecBumperCar.reserve(8);
}


CBumperCarField::~CBumperCarField()
{
	Release();
}

int CBumperCarField::Update()
{
	CGameObject::Update();
	Check_DeadLine();
	Check_BumperCarHp();
	m_fBumperConvertTime += m_pTimeMgr->GetDeltaTime();
	m_fFindTargetTime += m_pTimeMgr->GetDeltaTime();

	if (m_fBumperConvertTime >= 60.f)
		m_fBumperConvertTime = 60.f;

	Find_Target();
	
	for (auto& iter : m_vecBumperCar)
	{
		iter->Update();
	}

	for (size_t i = 0; i < m_vecBumperCar.size(); ++i)
	{
		for (size_t j = 0; j < m_vecBumperCar.size(); ++j)
		{
			// j와 i가 같은 경우( 자기끼리의 충돌 판정 여부가 되므로, 예외처리)
			// j보다 i가 작은 경우도 해당됨 ( 이미 충돌 판정을 끝마친 상태에서 다시 충돌판정하면 상쇄효과가 나올 수 있음 ).
			if (j == i || j > i)
				continue;

			CCollisionMgr::GetInstance()->Collision_BumperCar(m_vecBumperCar[i], m_vecBumperCar[j]);
		}
	}


	return NO_EVENT;
}

void CBumperCarField::LateUpdate()
{
	CGameObject::LateUpdate();

	for (auto& iter : m_vecBumperCar)
	{
		iter->LateUpdate();
	}

}

void CBumperCarField::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 4; ++i)
	{
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// 투영스페이스 범위 밖에 정점들을 투영변환에서 제외시키기
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());

	}
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	m_mapComponent[L"Texture"]->Render(dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->GetCurrentFrame());
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	for (auto& iter : m_vecBumperCar)
	{
		iter->Render();
	}

}

HRESULT CBumperCarField::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);


	//MyCar
	CSuperCar* pMyCar = CMyCar::Create(m_pGraphicDev, { 31.f, 0.f, 14.f }, { 1.f, 1.f, 1.f }, 0.f);
	NULL_CHECK_MSG_RETURN(pMyCar, L"MyCar Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pMyCar);

	// AutoCar 00
	CSuperCar* pAutoCar = CAutoCar::Create(m_pGraphicDev, { -4.f, 0.f, 14.f }, { 1.f, 1.f, 1.f }, 90.f, 0);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar00 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 01
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 3.f, 0.f, -7.f }, { 1.f, 1.f, 1.f }, 45.f, 1);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar01 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 02
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 30.f, 0.f, -19.f }, { 1.f, 1.f, 1.f }, 0.f, 2);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar02 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 03
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 57.f, 0.f, -8.f }, { 1.f, 1.f, 1.f }, -45.f, 3);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar03 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 04
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 64.f, 0.f, 15.f }, { 1.f, 1.f, 1.f }, -90.f, 4);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar04 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 05
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 56.f, 0.f, 37.f }, { 1.f, 1.f, 1.f }, -135.f, 5);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar05 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 06
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 30.f, 0.f, 50.f }, { 1.f, 1.f, 1.f }, 180.f, 6);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar06 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	// AutoCar 07
	pAutoCar = CAutoCar::Create(m_pGraphicDev, { 3.f, 0.f, 37.f }, { 1.f, 1.f, 1.f }, 135.f, 7);
	NULL_CHECK_MSG_RETURN(pAutoCar, L"AutoCar07 Create Failed ", E_FAIL);
	m_vecBumperCar.push_back(pAutoCar);

	return S_OK;
}

HRESULT CBumperCarField::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_BumperCarField");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_BumperCarField Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_BumperCarField");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_BumperCarField Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_vRight = { 1.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };
	m_vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	// Frame
	pComponent = ENGINE::CFrame::Create(0.f, 8, 1.f, true);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollTerrain Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Frame", pComponent));

	return S_OK;
}

void CBumperCarField::Release()
{
	for_each(m_vecBumperCar.begin(), m_vecBumperCar.end(), ENGINE::SafeDelete<CSuperCar*>);
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CBumperCarField * CBumperCarField::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CBumperCarField* pInstance = new CBumperCarField(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}

// AI의 타켓찾기.
void CBumperCarField::Find_Target()
{
	if (m_vecBumperCar.empty())
		return;

	// 0은 플레이어이므로, 탐색할때 1부터 시작한다. (1부터 ~ n까지가 ai)
	int iSize = m_vecBumperCar.size();
	for (int i = 1; i < iSize; ++i)
	{
		float x1 = m_vecBumperCar[0]->GetPos().x - m_vecBumperCar[i]->GetPos().x;
		float z1 = m_vecBumperCar[0]->GetPos().z - m_vecBumperCar[i]->GetPos().z;
		float fDist1 = sqrtf(x1 * x1 + z1 * z1);
		// 일단 플레이어의 주소를 넘겨준후 비교한다 (이유 : 아예 비교대상이 없을 경우를 대비하여)
		m_vecBumperCar[i]->Set_Target(m_vecBumperCar[0]);
		m_vecBumperCar[i]->Set_Accel(true);

		for (int j = 1; j < iSize; ++j)
		{
			if (j == i)
				continue;

			float x2 = m_vecBumperCar[i]->GetPos().x - m_vecBumperCar[j]->GetPos().x;
			float z2 = m_vecBumperCar[i]->GetPos().x - m_vecBumperCar[j]->GetPos().z;
			float fDist2 = sqrtf(x2 * x2 + z2 * z2);

			// 비교하여 더 작을 경우 타켓을 교체해줌.
			if (fDist2 < fDist1 - 10)
			{
				//가까운적이 탈락하여 내려가는 적으로 y좌표가 낮은경우  다른적을 찾게끔
				if (m_vecBumperCar[j]->GetPos().y < 0.f)
					continue;

				m_vecBumperCar[i]->Set_Target(m_vecBumperCar[j]);
				fDist1 = fDist2;
			}
			//// 모두를 비교해도 맨처음의 구한 대상이 젤 가까운 경우 0(플레이어)의 주소를 넘겨준다.
			//else
			//{
			//	m_vecBumperCar[i]->Set_Target(m_vecBumperCar[0]);
			//}
		}
	}
}

void CBumperCarField::Check_DeadLine()
{
	for (size_t i = 0; i < m_vecBumperCar.size(); ++i)
	{
		D3DXVECTOR3 vPos = m_vecBumperCar[i]->GetPos();
		// x 판정
		if (vPos.x < -10.f || vPos.x > 70)
		{
			if (i == 0)
			{
				m_vecBumperCar[i]->Set_HpDecrease();
				m_vecBumperCar[i]->Set_Pos({ 31.f, 0.f, 14.f });
				CSoundMgr::GetInstance()->PlaySound(L"놉.mp3", CSoundMgr::CHANNELID::PLAYER);
			}
			else
			{
				if (m_vecBumperCar[i]->Get_Hp() > 0)
				{
					m_vecBumperCar[i]->Set_HpDecrease();
					CSoundMgr::GetInstance()->PlaySound(L"떨어지는소리.mp3", CSoundMgr::CHANNELID::MONSTER);
				}
			}
		}

		// z 판정
		else if (vPos.z < -24.f || vPos.z > 54)
		{
			if (i == 0)
			{
				m_vecBumperCar[i]->Set_HpDecrease();
				m_vecBumperCar[i]->Set_Pos({ 31.f, 0.f, 14.f });
				CSoundMgr::GetInstance()->PlaySound(L"놉.mp3", CSoundMgr::CHANNELID::PLAYER);
			}
			else
			{
				if (m_vecBumperCar[i]->Get_Hp() > 0)
				{
					m_vecBumperCar[i]->Set_HpDecrease();
					CSoundMgr::GetInstance()->PlaySound(L"떨어지는소리.mp3", CSoundMgr::CHANNELID::MONSTER);
				}
			}
		}

	}
}

void CBumperCarField::Check_BumperCarHp()
{
	for (size_t i = 1; i < m_vecBumperCar.size(); ++i)
	{
		if (m_vecBumperCar[i]->Get_Hp() <= 0)
			m_vecBumperCar[i]->Set_OutState(true);

		if (m_vecBumperCar[i]->GetPos().y <= -20.f)
		{
			ENGINE::SafeDelete(m_vecBumperCar[i]);
			m_vecBumperCar.erase(m_vecBumperCar.begin() + i);
			// 남아있는 적의 수 재정의.
			m_iAiCount = m_vecBumperCar.size() - 1;
		}
	}
}

int CBumperCarField::Get_PlayTime()
{
	return (int)(m_fBumperTotalTime - m_fBumperConvertTime);
}

int CBumperCarField::Get_PlayerHp()
{
	return m_vecBumperCar[0]->Get_Hp();
}

int CBumperCarField::Get_AiCount()
{
	return m_iAiCount;
}
