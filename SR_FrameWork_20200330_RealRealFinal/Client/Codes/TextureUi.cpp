#include "stdafx.h"
#include "TextureUi.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "PipeLine.h"

CTextureUi::CTextureUi(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance())
{
}


CTextureUi::~CTextureUi()
{
	Release();
}

int CTextureUi::Update()
{
	CUiPlate::Update();
	return 0;
}

void CTextureUi::LateUpdate()
{
	CUiPlate::LateUpdate();
}

void CTextureUi::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 10); // ���� ���� �� 1�� ����.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ������ ���� �κ��� ������.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	Render_Talk();
	m_pFontMgr->Render(L"����", D3DXVECTOR3(40.f , WINCY - 150.f, 0.f), m_szTexture, D3DCOLOR_ARGB(255, 0, 0, 0));


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//// ���� ����
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_mapComponent[L"Texture"]->Render(0);
	//m_mapComponent[L"Buffer"]->Render();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CTextureUi::Initialize()
{
	CUiPlate::Initialize();

	m_wstrTalk = L"�ȳ��ϼ���. ���� �� ���̰��� \n\n��� �������ִ� ����Դϴ�.";
	m_listTalk.push_back(m_wstrTalk);
	m_wstrTalk = L"���ݺ���, Ʃ�丮���� �����ϰڽ��ϴ�. \n\n���� ���ʻ�� Ui�� �����ּ���.";
	m_listTalk.push_back(m_wstrTalk);

	m_iIndex = 0;
	//wsprintf(m_szTexture, L"", temp);
	return S_OK;
}

HRESULT CTextureUi::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CTextureUi::Release()
{
	CUiPlate::Release();
}

void CTextureUi::KeyInput()
{
}

CTextureUi * CTextureUi::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CTextureUi* pInstance = new CTextureUi(pGraphicDev);
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

void CTextureUi::Render_Talk()
{
	m_fTime += m_pTimeMgr->GetDeltaTime();

	wstring wstrTalk = m_listTalk.front();


	if (m_iIndex <= wstrTalk.size())
	{
		if (m_fTime >= 0.1f)
		{
			m_szTexture[m_iIndex] = wstrTalk[m_iIndex];
			++m_iIndex;
			m_fTime = 0.f;
		}
	}
	//if (m_iIndex <= m_wstrTalk.size())
	//{
	//	if (m_fTime >= 0.1f)
	//	{
	//		m_szTexture[m_iIndex] = m_wstrTalk[m_iIndex];
	//		++m_iIndex;
	//		m_fTime = 0.f;
	//	}
	//}

	if (m_pKeyMgr->KeyDown(ENGINE::KEY_SPACE))
	{
		Update_Talk();
		//// �ʱ�ȭ ( ������ �����ִ� �ؽ��ĵ��� ��� ���������� ����)
		//ZeroMemory(&m_szTexture, sizeof(m_szTexture));

		////m_wstrTalk = L"���ݺ���, Ʃ�丮���� �����ϰڽ��ϴ�. \n\n���� ���ʻ�� Ui�� �����ּ���.";
		//m_iIndex = 0;
	}
}

void CTextureUi::Update_Talk()
{
	// �ʱ�ȭ ( ������ �����ִ� �ؽ��ĵ��� ��� ���������� ����)
	ZeroMemory(&m_szTexture, sizeof(m_szTexture));
	m_listTalk.pop_front();
	m_iIndex = 0;
}

void CTextureUi::SetTexture(wstring wstrTalk)
{
	m_listTalk.push_back(wstrTalk);
}
