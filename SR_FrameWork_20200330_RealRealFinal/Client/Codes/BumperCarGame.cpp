#include "stdafx.h"
#include "BumperCarGame.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "SceneSelector.h"
#include "SoundMgr.h"
#include "FpsCamera.h"

#include "CollisionMgr.h"
#include "BumperCar.h"

CBumperCarGame::CBumperCarGame(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_pCollisionMgr(CCollisionMgr::GetInstance())
{
}


CBumperCarGame::~CBumperCarGame()
{
	Release();
}

int CBumperCarGame::Update()
{
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

void CBumperCarGame::LateUpdate()
{
	ENGINE::CScene::LateUpdate();
	CSoundMgr::GetInstance()->Update();
}

void CBumperCarGame::Render()
{
	ENGINE::CScene::Render();
}

HRESULT CBumperCarGame::Initialize()
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
		ENGINE::BUFFER_CUBETEX, L"Buffer_SkyBox");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SkyBox Add Failed", E_FAIL);

	// SkyBox Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_SkyBox", L"../Texture/BumperCar/Space%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_SkyBox Add Failed", E_FAIL);

	//BumperCar
	// SuperCar Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_SuperCar", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(3.f, 3.f, 3.f));
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SuperCar Add Failed", E_FAIL);

	// AutoCar Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_AutoCar", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(3.f, 3.f, 3.f));
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_AutoCar Add Failed", E_FAIL);

	// BumperCarField Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEXLYING, L"Buffer_BumperCarField", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_BumperCarField Add Failed", E_FAIL);

	// SuperCar Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_SuperCar", L"../Texture/BumperCar/Car/SuperCar%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_SuperCar Add Failed", E_FAIL);

	// AutoCar Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_AutoCar", L"../Texture/BumperCar/Car/BumperCar%d.dds", 8);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_AutoCar Add Failed", E_FAIL);

	// BumperCarField Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarField", L"../Texture/BumperCar/BumperCarField/Field%d.png", 8);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarField Add Failed", E_FAIL);
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarMainField", L"../Texture/BumperCar/BumperCarField/MainField%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarMainField Add Failed", E_FAIL);

	// BumperCarLife Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarLife", L"../Texture/BumperCar/heart%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarLife Add Failed", E_FAIL);

	// BumperCarCount Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarCount", L"../Texture/BumperCar/Count%d.png", 3);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarCount Add Failed", E_FAIL);

	// BumperCarStart Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarStart", L"../Texture/BumperCar/Start%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarStart Add Failed", E_FAIL);

	// BumperCarVictory Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarVictory", L"../Texture/BumperCar/Victory%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarVictory Add Failed", E_FAIL);

	// BumperCarFail Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarFail", L"../Texture/BumperCar/Fail%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarFail Add Failed", E_FAIL);

	// BumperCarQuest Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_BumperCarQuest", L"../Texture/BumperCar/Quest%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_BumperCarQuest Add Failed", E_FAIL);

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);

	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::BGM);

	return S_OK;
}

void CBumperCarGame::Release()
{
	//m_pCollisionMgr->DestroyInstance();
	m_pResourceMgr->DeleteDynamicResources();
	m_pResourceMgr->DeleteStaticResources();
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_MAIN, true);
	m_pResourceMgr->ChangeSceneType(ENGINE::SCENE_TYPE::SCENE_MAIN);
}

HRESULT CBumperCarGame::Add_Environment_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_ENVIRONMENT, ENGINE::CLayer::Create(m_pGraphicDev)));

	//// Camera
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_SUB, false);
	//m_pCameraMgr->PushCamera(CDynamicCamera::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 20.f, 0.f), D3DXVECTOR3(0.f, 7.f, 5.f)));
	m_pCameraMgr->PushCamera(CDynamicCamera::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 40.f, 0.f), D3DXVECTOR3(0.f, 1.f, 1.f)));

	// SkyBox
	HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CSkyBox::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"SkyBox Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CBumperCarGame::Add_Object_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_OBJECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	// BumperCar
	CBumperCar* pBumperCar = CBumperCar::Create(m_pGraphicDev, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), 0.f);
	HRESULT hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"BumperCar", pBumperCar);
	FAILED_CHECK_MSG_RETURN(hr, L"BumperCar Create Failed ", E_FAIL);

	return S_OK;
}

HRESULT CBumperCarGame::Add_Effect_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_EFFECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

HRESULT CBumperCarGame::Add_UI_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_UI, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

CBumperCarGame * CBumperCarGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CBumperCarGame* pInstance = new CBumperCarGame(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
