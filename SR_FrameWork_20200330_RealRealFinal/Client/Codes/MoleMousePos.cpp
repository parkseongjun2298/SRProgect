#include "stdafx.h"
#include "MoleMousePos.h"
#include "Transform.h"

#include "UiMoleMouse.h"

CMoleMousePos::CMoleMousePos(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev)
{
}


CMoleMousePos::~CMoleMousePos()
{
	Release();
}

int CMoleMousePos::Update()
{
	CGameObject::Update();
	Update_MousePos();
	Attack_Hammer();

	m_pUiMouse->Update();
	return 0;
}

void CMoleMousePos::LateUpdate()
{
	CGameObject::LateUpdate();

	m_pUiMouse->LateUpdate();
}

void CMoleMousePos::Render()
{
	m_pUiMouse->Render();
}

HRESULT CMoleMousePos::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pUiMouse = CUiMoleMouse::Create(m_pGraphicDev, { -39.f, 40.5f, 0.f }, { 78.5f, 80.f, 1.f }, L"Texture_UiMoleMouse", this, 0);
	NULL_CHECK_RETURN(m_pUiMouse, E_FAIL);
	return S_OK;
}

HRESULT CMoleMousePos::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Mole Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));
	return S_OK;
}

void CMoleMousePos::Release()
{
	ENGINE::SafeDelete(m_pUiMouse);
}

void CMoleMousePos::Update_MousePos()
{
	// 마우스 좌표 추출 완료.
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	// 뷰포트영역의 좌표이므로 월드까지 내려야함.

	D3DXVECTOR3 vMousePos = { (float)pt.x - (WINCX / 2.f), (WINCY / 2.f) - (float)pt.y, 0.f };


	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetPos(vMousePos);
}

void CMoleMousePos::Attack_Hammer()
{
	if (!m_bIsHammer)
		return;

	m_pUiMouse->Set_FrameStart(m_bIsHammer);
	m_bIsHammer = false;
}


D3DXMATRIX CMoleMousePos::Get_MouseWorldMatrix()
{
	return dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->GetWorldMatrix();
}

CMoleMousePos * CMoleMousePos::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CMoleMousePos* pInstance = new CMoleMousePos(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}
