#include "stdafx.h"
#include "MoleCatchGame.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "SceneSelector.h"
#include "SoundMgr.h"

#include "MoleCatch.h"
#include "ShootingSkyBox.h"

CMoleCatchGame::CMoleCatchGame(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance())
{
}


CMoleCatchGame::~CMoleCatchGame()
{
	Release();
}

int CMoleCatchGame::Update()
{
	::ShowCursor(FALSE);
	int iEvent = NO_EVENT;

	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_RETURN))
	{
		ENGINE::CManagement::GetInstance()->SceneReturn();

		CSoundMgr::GetInstance()->StopAll();
		CSoundMgr::GetInstance()->PlayBGM(L"놀이동산브금.mp3");
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.2f);

		return CHANGE_SCENE;

	}

	iEvent = ENGINE::CScene::Update();
	CCameraMgr::GetInstance()->Update();

	return NO_EVENT;
}

void CMoleCatchGame::LateUpdate()
{
	ENGINE::CScene::LateUpdate();
	CSoundMgr::GetInstance()->Update();
}

void CMoleCatchGame::Render()
{
	ENGINE::CScene::Render();
}

HRESULT CMoleCatchGame::Initialize()
{
	srand(unsigned(time(nullptr)));

	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_SUB;
	m_pResourceMgr->ChangeSceneType(m_eSceneType);

	// UiBase Buffer
	HRESULT hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_UiBase");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_UiBase Add Failed", E_FAIL);

	// SkyBox Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_ShootingSkyBox");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_ShootingSkyBox Add Failed", E_FAIL);

	// SkyBox Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_ShootingSkyBox", L"../Texture/MoleCatch/MoleSky%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingSkyBox Add Failed", E_FAIL);

	////MiniGame
	//Mole Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_Mole", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f));

	//MoleCatch Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_MoleCatch", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(6.f, 2.f, 6.f));

	// Mole Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_Mole", L"../Texture/MoleCatch/Mole%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Mole Add Failed", E_FAIL);
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_MoleAngry", L"../Texture/MoleCatch/MoleAngry%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_MoleAngry Add Failed", E_FAIL);

	// MoleCatch Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_MoleCatch", L"../Texture/MoleCatch/MoleCatch%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_MoleCatch Add Failed", E_FAIL);

	// UiMoleCount Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_UiMoleCount", L"../Texture/MoleCatch/Count%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiMoleCount Add Failed", E_FAIL);

	// UiMoleTime Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_UiMoleTime", L"../Texture/MoleCatch/Timer%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiMoleTime Add Failed", E_FAIL);

	// UiMoleMouse Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_UiMoleMouse", L"../Texture/MoleCatch/Hammer/Hammer%d.png", 6);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiMoleMouse Add Failed", E_FAIL);

	// UiMoleStart Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_UiMoleStart", L"../Texture/MoleCatch/Start%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiMoleStart Add Failed", E_FAIL);

	// UiMoleEnd Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_UiMoleEnd", L"../Texture/MoleCatch/GameOver%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiMoleEnd Add Failed", E_FAIL);

	// Effect
	{
		// Effect Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_Effect", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_EffectPlus Add Failed", E_FAIL);

		// EffectPlus Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_EffectPlus", L"../Texture/MoleCatch/plus%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_EffectPlus Add Failed", E_FAIL);

		// EffectMius Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_EffectMinus", L"../Texture/MoleCatch/minus%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_EffectMinus Add Failed", E_FAIL);
	}

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);

	return S_OK;
}

void CMoleCatchGame::Release()
{
	m_pResourceMgr->DeleteDynamicResources();
	m_pResourceMgr->DeleteStaticResources();
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_MAIN, true);
	m_pResourceMgr->ChangeSceneType(ENGINE::SCENE_TYPE::SCENE_MAIN);
}

HRESULT CMoleCatchGame::Add_Environment_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_ENVIRONMENT, ENGINE::CLayer::Create(m_pGraphicDev)));

	//// Camera
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_SUB, false);
	m_pCameraMgr->PushCamera(CDynamicCamera::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 5.f, 0.f), D3DXVECTOR3(0.f, 0.f, 7.f)));

	//// SkyBox
	//HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CSkyBox::Create(m_pGraphicDev));
	//FAILED_CHECK_MSG_RETURN(hr, L"SkyBox Create Failed", E_FAIL);

	HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CShootingSkyBox::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"SkyBox Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CMoleCatchGame::Add_Object_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_OBJECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	// MoleCatch
	CMoleCatch* pMole = CMoleCatch::Create(m_pGraphicDev, D3DXVECTOR3(30.f, 0.f, 15.f), D3DXVECTOR3(1.f, 1.f, 1.f), 0.f);
	HRESULT hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"MoleCatch", pMole);
	FAILED_CHECK_MSG_RETURN(hr, L"MoleCatch Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CMoleCatchGame::Add_Effect_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_EFFECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

HRESULT CMoleCatchGame::Add_UI_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_UI, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

CMoleCatchGame * CMoleCatchGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CMoleCatchGame* pInstance = new CMoleCatchGame(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
