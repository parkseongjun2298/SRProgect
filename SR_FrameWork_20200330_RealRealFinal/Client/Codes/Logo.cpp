#include "stdafx.h"
#include "Logo.h"
#include "SceneSelector.h"
#include "UiPlate.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev),
	m_pManagement(ENGINE::CManagement::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiPlate(nullptr)
{
}


CLogo::~CLogo()
{
	Release();
}

int CLogo::Update()
{
	//m_pUiPlate->Update();
	int iEvent = ENGINE::CScene::Update();

	if (m_pKeyMgr->KeyDown(ENGINE::KEY_RETURN))
	{
		m_pManagement->SceneChange(CSceneSelector(SCENE_LOGO, SCENE_STAGE), ENGINE::SCENE_MAIN);
		return CHANGE_SCENE;
	}

	return iEvent;
}

void CLogo::LateUpdate()
{
	//m_pUiPlate->LateUpdate();
	ENGINE::CScene::LateUpdate();
}

void CLogo::Render()
{
	//m_pUiPlate->Render();
	m_pTexture->Render(0);
	m_pBuffer->Render();
	ENGINE::CScene::Render();
}

HRESULT CLogo::Initialize()
{
	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_MAIN;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE); // Α¶Έν off	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	ENGINE::CResourceMgr::GetInstance()->ChangeSceneType(ENGINE::SCENE_TYPE::SCENE_MAIN);

	HRESULT hr = ENGINE::CResourceMgr::GetInstance()->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_Logo", L"../Texture/Logo/Logo%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Logo Add Failed", E_FAIL);
	
	// UiBase Buffer
	hr = ENGINE::CResourceMgr::GetInstance()->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_UiBase");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_UiBase Add Failed", E_FAIL);

	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_MAIN;
	m_pResourceMgr->ChangeSceneType(m_eSceneType);
	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);	

	return S_OK;
}

void CLogo::Release()
{
	ENGINE::SafeDelete(m_pUiPlate);
}

HRESULT CLogo::Add_Environment_Layer()
{
	return S_OK;
}

HRESULT CLogo::Add_Object_Layer()
{
	return S_OK;
}

HRESULT CLogo::Add_Effect_Layer()
{
	//m_pUiPlate = CUiPlate::Create(m_pGraphicDev, { -WINCX * 0.5f, WINCY * 0.5f, 1.f }, { WINCX, WINCY, 1.f }, L"Texture_Logo", 0, false);
	m_pBuffer = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_UiBase");
	NULL_CHECK_MSG_RETURN(m_pBuffer, L"Buffer_UiBase Clone Failed", E_FAIL);


	// Texture
	m_pTexture = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Logo");
	NULL_CHECK_MSG_RETURN(m_pTexture, L"Texture_Logo Clone Failed", E_FAIL);

	return S_OK;
}

HRESULT CLogo::Add_UI_Layer()
{
	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CLogo* pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
