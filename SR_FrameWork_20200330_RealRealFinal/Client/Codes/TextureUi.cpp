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

		// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
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

	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 10); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	Render_Talk();
	m_pFontMgr->Render(L"바탕", D3DXVECTOR3(40.f , WINCY - 150.f, 0.f), m_szTexture, D3DCOLOR_ARGB(255, 0, 0, 0));


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//// 알파 블렌딩
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

	m_wstrTalk = L"안녕하세요. 저는 이 놀이공원 \n\n운영을 지원해주는 사람입니다.";
	m_listTalk.push_back(m_wstrTalk);
	m_wstrTalk = L"지금부터, 튜토리얼을 시작하겠습니다. \n\n먼저 왼쪽상단 Ui를 눌러주세요.";
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
		//// 초기화 ( 기존에 적혀있던 텍스쳐들을 모두 지워버리는 역할)
		//ZeroMemory(&m_szTexture, sizeof(m_szTexture));

		////m_wstrTalk = L"지금부터, 튜토리얼을 시작하겠습니다. \n\n먼저 왼쪽상단 Ui를 눌러주세요.";
		//m_iIndex = 0;
	}
}

void CTextureUi::Update_Talk()
{
	// 초기화 ( 기존에 적혀있던 텍스쳐들을 모두 지워버리는 역할)
	ZeroMemory(&m_szTexture, sizeof(m_szTexture));
	m_listTalk.pop_front();
	m_iIndex = 0;
}

void CTextureUi::SetTexture(wstring wstrTalk)
{
	m_listTalk.push_back(wstrTalk);
}
