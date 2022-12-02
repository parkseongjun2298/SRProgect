#include "stdafx.h"
#include "Mole.h"
#include "Transform.h"
#include "CubeTex.h"
#include "PipeLine.h"
#include "CameraObserver.h"
#include "CollMouse.h"

#include "EffectCount.h"
#include "SoundMgr.h"

CMole::CMole(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_fSitHeight(0.f), m_fStandHeight(0.f), m_fStandTime(0.f),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr)
{
}

CMole::~CMole()
{
	Release();
}

void CMole::SetMolePlay(bool bCheck)
{
	m_bIsPlay = bCheck;
}

void CMole::SetMoleHit(bool bCheck)
{
	m_bIsHit = bCheck;
}

void CMole::SetMoleAngry(bool bCheck)
{
	m_bIsAngry = bCheck;
}

bool CMole::GetMolePlay()
{
	return m_bIsPlay;
}

bool CMole::GetMoleHit()
{
	return m_bIsHit;
}

bool CMole::GetMoleAngry()
{
	return m_bIsAngry;
}


int CMole::Update()
{
	ENGINE::CGameObject::Update();
	MolePlay();

	return NO_EVENT;
}

void CMole::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CMole::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	
	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 8; ++i)
	{
		// ���� -> �佺���̽�
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// ���������̽� ��ȯ�� ����ó��
		if (1.f > m_pConvertVtx[i].vPos.z || 1000.f < m_pConvertVtx[i].vPos.z)
			continue;

		// �� -> ���������̽�
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// ȭ�� �δ������� �Ǵ��Ͽ� �ؽ��ĸ� �ٸ��� ������.
	if (m_bIsAngry)
		m_mapComponent[L"TextureAngry"]->Render();
	else
		m_mapComponent[L"Texture"]->Render();
	// Texture�� ���� �����ϰ� Buffer�� �����ؾ��Ѵ�. -> ���۰� �׸��°����� ��ؽ��ķ� �׷��������� �����˾ƾ��ϱ� ����
	m_mapComponent[L"Buffer"]->Render();

	MoleEffectRender();
}

HRESULT CMole::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	// Effect
	m_pEffectCount = CEffectCount::Create(m_pGraphicDev, pTransform->GetPos() + D3DXVECTOR3(0.f, 1.f, 0.f), { 0.9f, 0.68f, 1.f });
	NULL_CHECK_MSG_RETURN(m_pEffectCount, L"EffectCount Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CMole::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;
	
	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Mole");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Mole Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Mole");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Mole Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_MoleAngry");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_MoleAngry Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"TextureAngry", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = m_vPos;
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Mole Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Mole Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	// CollMouse
	pComponent = CCollMouse::Create(pObserver);
	NULL_CHECK_MSG_RETURN(pComponent, L"Mole CollMouse Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"CollMouse", pComponent));

	return S_OK;
}

void CMole::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	ENGINE::SafeDelete(m_pEffectCount);
}

void CMole::MolePlay()
{
	if (!m_bIsPlay)
		return;


	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	float m_fStandSpeed = 3.f * m_pTimeMgr->GetDeltaTime() * 2.f;
	float m_fSitSpeed = 3.f * m_pTimeMgr->GetDeltaTime() * 2.f;


	////// �ö󰡴� ��� //////
	// �ö�� ���̰� 3���� �����鼭, 
	// �������°� �ƴ϶�� ��� �ö󰡶�.
	if (2.f >= m_fStandHeight && !m_bIsHit)
	{
		if (!m_bIsSound)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
			CSoundMgr::GetInstance()->PlaySound(L"�δ����Ҹ�.mp3", CSoundMgr::CHANNELID::MONSTER);
			m_bIsSound = true;
		}

		m_fStandHeight += m_fStandSpeed;
		m_vPos.y += m_fStandSpeed;
		pTransform->SetPos(m_vPos);
	}
	// �ö�� ���̰� 3���� ũ�ų� �������°� �ƴ϶��, 
	// ���̻� �ö����ʰ� �ö�ͼ� �带 �ð��� ����.
	else
	{
		m_fStandTime += m_pTimeMgr->GetDeltaTime();
	}


	////// �������� ��� //////
	// �ö�� �ִ� �ð��� 2�ʰ� �Ѱų�, �������¶��, ��������
	if (2.f <= m_fStandTime || m_bIsHit)
	{
		// ��ŷ�Ǿ� ���������� ����Ʈ ����.
		if (m_bIsHit)
			MoleEffectUpdate();

		//// ������ �ٽ�.
		// ������ ���̰� �ö�� ���̺��� ������, ��� ��������
		if (m_fStandHeight >= m_fSitHeight)
		{
			//cout << "stand : " << m_fStandHeight << " , sit : " << m_fSitHeight << endl;
			m_fSitHeight += m_fSitSpeed;
			m_vPos.y -= m_fSitSpeed;

			// y��ǥ�� �� �������� �� ����ó��
			if (0.f >= m_vPos.y)
				m_vPos.y = 0.f;

			pTransform->SetPos(m_vPos);
		}
		// ������ ���̰� �ö�� ���̸� ���� ���, ��� �ʱ�ȭ.
		else 
		{
			//cout << "===========================" << endl;
			m_bIsPlay = false;
			m_bIsHit = false;
			m_bIsAngry = false;
			m_fSitHeight = 0.f;
			m_fStandHeight = 0.f;
			m_fStandTime = 0.f;
			m_bIsSound = false;

			m_pEffectCount->Set_Minus(false);
			m_pEffectCount->Set_RenderState(false);
		}
	}

}

void CMole::MoleHit()
{
	CCollMouse* pCollMouse = dynamic_cast<CCollMouse*>(m_mapComponent[L"CollMouse"]);
	NULL_CHECK(pCollMouse);

	m_bIsHit = pCollMouse->PickMole(this);
}

void CMole::MoleEffectUpdate()
{
	// �������̶�� ���.
	if (m_pEffectCount->Get_RenderState())
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	m_pEffectCount->Set_RenderState(true);
	m_pEffectCount->Set_Pos(pTransform->GetPos() + D3DXVECTOR3(0.f, 0.5f, 0.f));
}

void CMole::MoleEffectRender()
{
	if (!m_pEffectCount->Get_RenderState())
		return;

	if (m_bIsAngry)
		m_pEffectCount->Set_Minus(true);

	m_pEffectCount->Update();
	m_pEffectCount->LateUpdate();
	m_pEffectCount->Render();
}


CMole * CMole::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CMole* pInstance = new CMole(pGraphicDev);
	pInstance->m_vPos = vPos;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}
