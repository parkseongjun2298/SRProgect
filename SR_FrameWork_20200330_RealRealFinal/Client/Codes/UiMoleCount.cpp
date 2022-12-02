#include "stdafx.h"
#include "UiMoleCount.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"

CUiMoleCount::CUiMoleCount(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance()),
	m_iCount(0)
{
}


CUiMoleCount::~CUiMoleCount()
{
	Release();
}

int CUiMoleCount::Update()
{
	CUiPlate::Update();

	return NO_EVENT;
}

void CUiMoleCount::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));

}

void CUiMoleCount::Render()
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

	m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);
	m_mapComponent[L"Buffer"]->Render();

	Render_Count();

	m_pFontMgr->Render(L"둥근모꼴", D3DXVECTOR3( 300.f, 40.f, 0.f), m_szTextureCount, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CUiMoleCount::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CUiMoleCount::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CUiMoleCount::Release()
{
	CUiPlate::Release();
}

void CUiMoleCount::Render_Count()
{
	ZeroMemory(&m_szTextureCount, sizeof(m_szTextureCount));
	swprintf_s(m_szTextureCount, L"%d", m_iCount);
}

void CUiMoleCount::Notify_Count(int iCount)
{
	m_iCount = iCount;
}

CUiMoleCount * CUiMoleCount::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiMoleCount* pInstance = new CUiMoleCount(pGraphicDev);
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
