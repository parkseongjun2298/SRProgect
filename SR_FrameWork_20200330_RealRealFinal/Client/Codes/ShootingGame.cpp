#include "stdafx.h"
#include "ShootingGame.h"

#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "FpsCamera.h"
#include "SceneSelector.h"
#include "SoundMgr.h"
#include "ShootingSkyBox.h"
#include"ShootingMgr.h"

CShootingGame::CShootingGame(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance())
{
}


CShootingGame::~CShootingGame()
{
	Release();
}

int CShootingGame::Update()
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




	return iEvent;
}

void CShootingGame::LateUpdate()
{
	ENGINE::CScene::LateUpdate();
	CSoundMgr::GetInstance()->Update();
}

void CShootingGame::Render()
{
	//POINT pt;
	//GetCursorPos(&pt);
	//ScreenToClient(g_hWnd, &pt);
	//system("cls");
	//cout << pt.x << ", " << pt.y << endl;


	ENGINE::CScene::Render();

}

HRESULT CShootingGame::Initialize()
{
	srand(unsigned(time(nullptr)));

	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_SUB;
	m_pResourceMgr->ChangeSceneType(m_eSceneType);

	HRESULT hr = E_FAIL;

	// UiBase Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_UiBase");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_UiBase Add Failed", E_FAIL);

	// SkyBox Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_ShootingSkyBox");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_ShootingSkyBox Add Failed", E_FAIL);


	// SkyBox Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_ShootingSkyBox", L"../Texture/ShootingGame/ShootingBackGround%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingSkyBox Add Failed", E_FAIL);

	// Monster Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_Monster", { 0.f,0.f,0.f }, { 1.f,1.f,1.f });
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Monster Add Failed", E_FAIL);

	// Bullet
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_Bullet");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Bullet Add Failed", E_FAIL);



	// Moster Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_STATIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_Moster", L"../Texture/ShootingGame/사격판%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Moster Add Failed", E_FAIL);

	//Bullet Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_Bullet", L"../Texture/ShootingGame/Bullet%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Bullet Add Failed", E_FAIL);

	// UiCount Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingCount", L"../Texture/ShootingGame/Count%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingCount Add Failed", E_FAIL);

	// UiTime Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingTime", L"../Texture/ShootingGame/Timer%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingTime Add Failed", E_FAIL);

	// UiStart Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingStart", L"../Texture/ShootingGame/게임시작%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingStart Add Failed", E_FAIL);

	// UiOver Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingOver", L"../Texture/ShootingGame/게임오버%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingOver Add Failed", E_FAIL);

	// Crosshair Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_CrossHair", L"../Texture/ShootingGame/Crosshair%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_CrossHair Add Failed", E_FAIL);

	// Gun Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingGun", L"../Texture/ShootingGame/총%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingGun Add Failed", E_FAIL);
	//Effect
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_ShootingEffect", L"../Texture/ShootingGame/이펙트%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShootingEffect Add Failed", E_FAIL);


	// Effect
	{
		// Effect Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_Effect", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Effect Add Failed", E_FAIL);

		// EffectPlus Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_EffectPlusFive", L"../Texture/ShootingGame/plusFive.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_EffectPlusFive Add Failed", E_FAIL);

		// 10 Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_EffectPlusTen", L"../Texture/ShootingGame/plusTen.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_EffectPlusTen Add Failed", E_FAIL);
	}


	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);




	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::BGM);
	CSoundMgr::GetInstance()->PlayBGM(L"사격bgm.mp3");
	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::MINIGAMEBGM, 0.2f);

	return S_OK;
}

void CShootingGame::Release()
{
	m_pResourceMgr->DeleteDynamicResources();
	m_pResourceMgr->DeleteStaticResources();
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_MAIN, true);
	m_pResourceMgr->ChangeSceneType(ENGINE::SCENE_TYPE::SCENE_MAIN);
}

HRESULT CShootingGame::Add_Environment_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_ENVIRONMENT, ENGINE::CLayer::Create(m_pGraphicDev)));

	//// Camera
	m_pCameraMgr->ChangeSceneCamera(ENGINE::SCENE_TYPE::SCENE_SUB, false);
	m_pCameraMgr->PushCamera(CFpsCamera::Create(m_pGraphicDev));

	// SkyBox
	HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CShootingSkyBox::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"CShootingSkyBox Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CShootingGame::Add_Object_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_OBJECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	// Player
	CShootingMgr* pPlayer = CShootingMgr::Create(m_pGraphicDev, D3DXVECTOR3{ 0.f,0.f,0.f }, D3DXVECTOR3{ 1.f,1.f,1.f });
	HRESULT hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Player", pPlayer);
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);




	//// Terrain
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	//FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CShootingGame::Add_Effect_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_EFFECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

HRESULT CShootingGame::Add_UI_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_UI, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

CShootingGame* CShootingGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CShootingGame* pInstance = new CShootingGame(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}