#include "stdafx.h"
#include "UiTexture.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "PipeLine.h"

CUiTexture::CUiTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance())
{
}


CUiTexture::~CUiTexture()
{
	Release();
}

int CUiTexture::Update()
{
	CUiPlate::Update();

	if (m_pKeyMgr->KeyDown(ENGINE::KEY_SPACE))
	{
		return Update_Talk();
		//// 초기화 ( 기존에 적혀있던 텍스쳐들을 모두 지워버리는 역할)
		//ZeroMemory(&m_szTexture, sizeof(m_szTexture));

		////m_wstrTalk = L"지금부터, 튜토리얼을 시작하겠습니다. \n\n먼저 왼쪽상단 Ui를 눌러주세요.";
		//m_iIndex = 0;
	}

	return NO_EVENT;
}

void CUiTexture::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, {0.f, 0.f, 0.f});
}

void CUiTexture::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &MATRIX::Identity());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &MATRIX::Identity());

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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200); // 알파 기준 값 1로 설정. 1~255
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	Render_Talk();
	m_pFontMgr->Render(L"둥근모꼴", D3DXVECTOR3(40.f, WINCY - 150.f, 0.f), m_szTexture, D3DCOLOR_ARGB(255, 255, 255, 255));


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

HRESULT CUiTexture::Initialize()
{
	CUiPlate::Initialize();

	//m_wstrTalk = L"안녕하세요. 저는 이 놀이공원 \n\n운영을 지원해주는 사람입니다.";
	//m_listTalk.push_back(m_wstrTalk);
	//m_wstrTalk = L"지금부터, 튜토리얼을 시작하겠습니다. \n\n먼저 왼쪽상단 Ui를 눌러주세요.";
	//m_listTalk.push_back(m_wstrTalk);
	m_iIndex = 0;

	return S_OK;
}

HRESULT CUiTexture::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CUiTexture::Release()
{
	CUiPlate::Release();
	////wstring 어떻게 지워야할까요?!
	//for_each(m_listTalk.begin(), m_listTalk.end(), ENGINE::SafeDelete<wstring>);
}

void CUiTexture::KeyInput()
{
}

CUiTexture * CUiTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiTexture* pInstance = new CUiTexture(pGraphicDev);
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

void CUiTexture::Render_Talk()
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
}

int CUiTexture::Update_Talk()
{
	// 초기화 ( 기존에 적혀있던 텍스쳐들을 모두 지워버리는 역할)
	ZeroMemory(&m_szTexture, sizeof(m_szTexture));

	//// 출력된 내용 지우기.
	//wstring wstrTemp = m_listTalk.front();
	//ENGINE::SafeDelete(wstrTemp);

	if (m_listTalk.size() <= 1)
	{
		CEventMgr::GetInstance()->SetQuestClear(0);
		return DEAD_OBJ;
	}


	m_listTalk.pop_front();
	m_iIndex = 0;


	return NO_EVENT;
}

void CUiTexture::SetTexture(wstring wstrTalk)
{
	m_listTalk.push_back(wstrTalk);
}
