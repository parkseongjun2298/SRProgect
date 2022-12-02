#include "stdafx.h"
#include "SubStageExample.h"
#include "Player.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "Monster.h"
#include "Grass.h"
#include "SkyBox.h"
#include "SceneSelector.h"
#include "SoundMgr.h"

CSubStageExample::CSubStageExample(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev), 
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance())
{
}


CSubStageExample::~CSubStageExample()
{
	Release();
}

int CSubStageExample::Update()
{
	int iEvent = NO_EVENT;

	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_RETURN))
	{
		ENGINE::CManagement::GetInstance()->SceneReturn();
		return CHANGE_SCENE;
	}
	
	//if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_RETURN))
	//{
	//	ENGINE::CManagement::GetInstance()->SceneReturn();
	//	return CHANGE_SCENE;
	//}

	iEvent = ENGINE::CScene::Update();	
	CCameraMgr::GetInstance()->Update();

	return iEvent;
}

void CSubStageExample::LateUpdate()
{
	ENGINE::CScene::LateUpdate();
}

void CSubStageExample::Render()
{
	//POINT pt;
	//GetCursorPos(&pt);
	//ScreenToClient(g_hWnd, &pt);
	//system("cls");
	//cout << pt.x << ", " << pt.y << endl;


	ENGINE::CScene::Render();
}

HRESULT CSubStageExample::Initialize()
{
	srand(unsigned(time(nullptr)));

	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_SUB;
	m_pResourceMgr->ChangeSceneType(m_eSceneType);


	// Player Buffer
	HRESULT hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_STATIC, 
		ENGINE::BUFFER_RCTEX, L"Buffer_Player");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Player Add Failed", E_FAIL);

	// UiBase Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_UiBase");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_UiBase Add Failed", E_FAIL);

	// SkyBox Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_SkyBox");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SkyBox Add Failed", E_FAIL);

	// Terrain Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_TERRAINTEX, L"Buffer_Terrain", {}, {}, 0xffff0000, ENGINE::PIVOT_TYPE::PIVOT_MIDDLE, TERRAIN_CNTX, TERRAIN_CNTZ, TERRAIN_ITV);
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Terrain Add Failed", E_FAIL);

	// Player Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_STATIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_Player", L"../Texture/Player%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Player Add Failed", E_FAIL);

	// SkyBox Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_SkyBox", L"../Texture/SkyBox%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_SkyBox Add Failed", E_FAIL);

	// Terrain Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_Terrain", L"../Texture/Terrain/Grass%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Terrain Add Failed", E_FAIL);		


	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);

	//CSoundMgr::GetInstance()->PlayBGM(L"놀이동산브금.mp3");
	//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.2f);
	//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);

	return S_OK;
}

void CSubStageExample::Release()
{
	m_pResourceMgr->DeleteDynamicResources();
	m_pResourceMgr->DeleteStaticResources();
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_MAIN, true);
	m_pResourceMgr->ChangeSceneType(ENGINE::SCENE_TYPE::SCENE_MAIN);
}

HRESULT CSubStageExample::Add_Environment_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_ENVIRONMENT, ENGINE::CLayer::Create(m_pGraphicDev)));

	//// Camera
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_SUB, false);
	m_pCameraMgr->PushCamera(CDynamicCamera::Create(m_pGraphicDev));

	// SkyBox
	HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CSkyBox::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"SkyBox Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CSubStageExample::Add_Object_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_OBJECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	HRESULT hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Player", pPlayer);
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);

	//// Terrain
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	//FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CSubStageExample::Add_Effect_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_EFFECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

HRESULT CSubStageExample::Add_UI_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_UI, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

CSubStageExample* CSubStageExample::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CSubStageExample* pInstance = new CSubStageExample(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}