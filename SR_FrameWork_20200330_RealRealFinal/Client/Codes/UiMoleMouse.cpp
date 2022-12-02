#include "stdafx.h"
#include "UiMoleMouse.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"
#include "MoleMousePos.h"

CUiMoleMouse::CUiMoleMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev)
{
}


CUiMoleMouse::~CUiMoleMouse()
{
	Release();
}

int CUiMoleMouse::Update()
{
	CUiPlate::Update();
	Check_Frame();

	return NO_EVENT;
}

void CUiMoleMouse::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, m_pMoleMouse->GetPos());
}

void CUiMoleMouse::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXMATRIX matWorld, matMine, matParent;
	matMine = pTransform->GetWorldMatrix();
	matParent = m_pMoleMouse->Get_MouseWorldMatrix();
	matWorld = pTransform->GetWorldMatrix() * m_pMoleMouse->Get_MouseWorldMatrix();


	for (int i = 0; i < 4; ++i)
	{
		// ���� -> ���彺���̽� ��ȯ.
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matMine);

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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 230); // ���� ���� �� 1�� ����.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ������ ���� �κ��� ������.

	m_mapComponent[L"Texture"]->Render((WORD)m_fFrameIndex);
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CUiMoleMouse::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CUiMoleMouse::AddComponent()
{
	CUiPlate::AddComponent();

	ENGINE::CComponent* pComponent = nullptr;

	return S_OK;
}

void CUiMoleMouse::Release()
{
	CUiPlate::Release();
}

void CUiMoleMouse::Check_Frame()
{
	if (!m_bFrameStart)
		return;

	m_fFrameIndex += 6.f * m_pTimeMgr->GetDeltaTime() * 6.f;

	if (m_fFrameIndex >= 6.f)
	{
		m_fFrameIndex = 0.f;
		m_bFrameStart = false;
	}
}

void CUiMoleMouse::Set_FrameStart(bool bCheck)
{
	m_bFrameStart = bCheck;
}


CUiMoleMouse * CUiMoleMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, CMoleMousePos* pMoleMouse, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CUiMoleMouse* pInstance = new CUiMoleMouse(pGraphicDev);
	pInstance->m_vPos = D3DXVECTOR3(vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z);
	pInstance->m_vSize = vSize;
	pInstance->m_wstrTextureTag = wstrTextureTag;
	pInstance->m_iTextureIndex = iIndex;
	pInstance->m_pMoleMouse = pMoleMouse;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
