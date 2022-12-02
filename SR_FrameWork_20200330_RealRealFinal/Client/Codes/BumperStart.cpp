#include "stdafx.h"
#include "BumperStart.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"

CBumperStart::CBumperStart(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_bIsQuest(true)
{
}


CBumperStart::~CBumperStart()
{
	Release();
}

int CBumperStart::Update()
{
	CUiPlate::Update();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(D3DXVECTOR3{ 0.f,20,0.f });

	// ����Ʈ�� Ŭ���ߴٸ� ���ӽ������� ��ŸƮ����.
	if (!m_bIsQuest)
	{
		CountSize();
		StartSize();
	}

	return 0;
}

void CBumperStart::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));
}

void CBumperStart::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);


	for (int i = 0; i < 4; ++i)
	{
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&pTransform->GetWorldMatrix());

		//ENGINE::CPipeLine::MyTransformCoord(
		//	&m_pConvertVtx[i].vPos,
		//	&m_pConvertVtx[i].vPos,
		//	&pObserver->GetViewMatrix());

		// ������ ����� �ܺο� �����ϴ� �������� ������ȯ���� ���� ��Ų��.
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		D3DXMATRIX matOrtho;
		D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1000.f);

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&matOrtho);
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// ���� �׽���: ���� ���� �� ������ �Ѿ �κ��� �Ұ��Ѵ�(����������).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200); // ���� ���� �� 1�� ����.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ������ ���� �κ��� ������.

	// ��ó������ ����Ʈ ȭ�� ���.
	if (m_bIsQuest)
	{
		m_mapComponent[L"Texture_Quest"]->Render((WORD)m_iTextureIndex);
	}
	else
	{
		// 3,2,1�� ��� ������ ����!
		if (m_bIsStart)
			m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);
		else
			m_mapComponent[L"Texture_Count"]->Render((WORD)m_iTextureIndex);
	}

	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBumperStart::Initialize()
{
	CUiPlate::Initialize();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(D3DXVECTOR3{ 0.f,20,0.f });

	return S_OK;
}

HRESULT CBumperStart::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	if (m_iTextureIndex >= m_pResourceMgr->GetInstance()->GetTextureSize(ENGINE::RESOURCE_DYNAMIC, m_wstrTextureTag))
		return E_FAIL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_UiBase");
	if (pComponent == nullptr)
	{
		pComponent = nullptr;
	}
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_UiBase Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, m_wstrTextureTag);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_BumperCarCount");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Count", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_BumperCarQuest");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Quest", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, -1.f };
	D3DXVECTOR3 vPos = m_vPos;
	D3DXVECTOR3 vSize = m_vSize;
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	return S_OK;
}

void CBumperStart::Release()
{
	CUiPlate::Release();
}


void CBumperStart::CountSize()
{
	if (!m_bIsCount)
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	if (m_vSize.x >= 1.f && m_vSize.y >= 1.f)
	{
		m_vSize.x -= 156.f * m_pTimeMgr->GetDeltaTime();
		m_vSize.y -= 204.f * m_pTimeMgr->GetDeltaTime();
		pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
	}
	else
	{
		m_vSize.x = 156.f;
		m_vSize.y = 204.f;
		pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
		++m_iTextureIndex;
	}

	// 3�� ��ٴ°� 3 , 2 , 1�� ��� ��µǾ��ٴ� ����, ���� ������ ����ؾ���.
	if (m_iTextureIndex >= 3)
	{
		m_iTextureIndex = 0;
		m_bIsCount = false;
		m_bIsStart = true;
		m_vSize.x = 402.f;
		m_vSize.y = 204.f;
		pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
	}
}

void CBumperStart::StartSize()
{
	if (!m_bIsStart)
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	if (m_vSize.x >= 1.f && m_vSize.y >= 1.f)
	{
		m_vSize.x -= 402.f * m_pTimeMgr->GetDeltaTime() * 2.f;
		m_vSize.y -= 204.f * m_pTimeMgr->GetDeltaTime() * 2.f;
		pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
	}
	else
	{
		m_bIsStart = false;
		m_bIsGameStart = true;
	}
}

void CBumperStart::QuestSize()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	m_vSize.x = 156.f;
	m_vSize.y = 204.f;
	pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
}

void CBumperStart::Set_Count(bool bCheck)
{
	m_bIsCount = bCheck;
}

void CBumperStart::Set_Quest(bool bCheck)
{
	m_bIsQuest = bCheck;
}

bool CBumperStart::Get_GameStart()
{
	return m_bIsGameStart;
}

CBumperStart * CBumperStart::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CBumperStart* pInstance = new CBumperStart(pGraphicDev);
	pInstance->m_vPos = D3DXVECTOR3(vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z);
	pInstance->m_vSize = vSize;
	pInstance->m_wstrTextureTag = wstrTextureTag;
	pInstance->m_iTextureIndex = iIndex;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
