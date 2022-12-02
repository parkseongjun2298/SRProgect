
#include "stdafx.h"
#include "ShootingTime.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"

CShootingTime::CShootingTime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance()),
	m_iTime(0)
{
}


CShootingTime::~CShootingTime()
{
	Release();
}

int CShootingTime::Update()
{
	CUiPlate::Update();

	return NO_EVENT;
}

void CShootingTime::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));
}

void CShootingTime::Render()
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

	Render_Time();

	m_pFontMgr->Render(L"둥근모꼴", D3DXVECTOR3(WINCX - 120.f, 40.f, 0.f), m_szTextureTime, D3DCOLOR_ARGB(255, 0, 255, 0));

}

HRESULT CShootingTime::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CShootingTime::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CShootingTime::Release()
{
	CUiPlate::Release();
}

void CShootingTime::Render_Time()
{
	ZeroMemory(&m_szTextureTime, sizeof(m_szTextureTime));
	swprintf_s(m_szTextureTime, L"%d", m_iTime);
}

void CShootingTime::Notify_Time(int iTime)
{
	m_iTime = iTime;
}

CShootingTime * CShootingTime::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShootingTime* pInstance = new CShootingTime(pGraphicDev);
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
