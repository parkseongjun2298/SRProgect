#include "stdafx.h"
#include "UiNpc.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CollMouse.h"
#include "PipeLine.h"

CUiNpc::CUiNpc(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev)
{
}


CUiNpc::~CUiNpc()
{
	Release();
}

int CUiNpc::Update()
{
	CUiPlate::Update();

	return 0;
}

void CUiNpc::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, { 0.f, 0.f, 0.f });
}

void CUiNpc::Render()
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

	//// ���� ����
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���� �׽���: ���� ���� �� ������ �Ѿ �κ��� �Ұ��Ѵ�(����������).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200); // ���� ���� �� 1�� ����. 1~255
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ������ ���� �κ��� ������.

	m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CUiNpc::Initialize()
{
	CUiPlate::Initialize();

	return S_OK;
}

HRESULT CUiNpc::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CUiNpc::Release()
{
	CUiPlate::Release();
}

CUiNpc * CUiNpc::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiNpc* pInstance = new CUiNpc(pGraphicDev);
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
