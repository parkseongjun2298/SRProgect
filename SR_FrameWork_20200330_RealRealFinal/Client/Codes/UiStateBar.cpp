#include "stdafx.h"
#include "UiStateBar.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CollMouse.h"
#include "PipeLine.h"

CUiStateBar::CUiStateBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance()),
	m_vPaintPos(0.f, 0.f, 0.f),
	m_iCoin(99000), m_iCustomer(0)
{
}


CUiStateBar::~CUiStateBar()
{
	Release();
}

int CUiStateBar::Update()
{
	CUiPlate::Update();

	Move_StateBar();

	return 0;
}

void CUiStateBar::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, m_vPaintPos);
}

void CUiStateBar::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

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

	//// 알파 블렌딩
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);
	m_mapComponent[L"Buffer"]->Render();

	Render_State();
	m_pFontMgr->Render(L"둥근모꼴", D3DXVECTOR3(WINCX - 500.f, 10.f - m_vPaintPos.y, 0.f), m_szTexturePopulation, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pFontMgr->Render(L"둥근모꼴", D3DXVECTOR3(WINCX - 215.f, 10.f - m_vPaintPos.y, 0.f), m_szTextureCoin, D3DCOLOR_ARGB(255, 255, 255, 255));

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT CUiStateBar::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CUiStateBar::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CUiStateBar::Release()
{
	CUiPlate::Release();
}

void CUiStateBar::Render_State()
{
	ZeroMemory(&m_szTextureCoin, sizeof(m_szTextureCoin));
	ZeroMemory(&m_szTexturePopulation, sizeof(m_szTexturePopulation));

	swprintf_s(m_szTextureCoin, L"$ :  %d", m_iCoin);
	swprintf_s(m_szTexturePopulation, L"손님 :  %d", m_iCustomer);
}

void CUiStateBar::Move_StateBar()
{
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_F3))
	{
		m_bIsMove = !m_bIsMove;
	}

	if (m_bIsMove && m_vPaintPos.y < 50.f)
		m_vPaintPos.y += 200.f * m_pTimeMgr->GetDeltaTime();
	if (!m_bIsMove && m_vPaintPos.y > 0.f)
		m_vPaintPos.y -= 200.f * m_pTimeMgr->GetDeltaTime();

}

CUiStateBar * CUiStateBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiStateBar* pInstance = new CUiStateBar(pGraphicDev);
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
