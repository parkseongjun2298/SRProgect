#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "Monster.h"
#include "Grass.h"
#include "SkyBox.h"
#include "SceneSelector.h"
#include "SoundMgr.h"
#include "UiStateBar.h"
//Flower
#include"FlowerMgr.h"
#include"YellowFlowerMgr.h"
#include"RedFlowerMgr.h"
//Rides
#include "Viking.h"
#include "SpinHorse.h"
#include "FerrisWheel.h"
#include "GyroDrop.h"
#include "Rail.h"
#include "Train.h"
#include "SpinTeaCup.h"
#include "GyroSwing.h"
#include "RollerCoaster.h"
//Customer
#include "Customer.h"

#include"LimitTileMgr.h"
#include "Tile.h"
//Shop
#include"ShopMgr.h"
#include"ChurroMgr.h"
#include"CottenCandyMgr.h"

#include "SoundMgr.h"
CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pEventMgr(CEventMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_bBuildingUiRender(false),
	m_fCustomerCreateTime(false)
{
}


CStage::~CStage()
{
	Release();
}

int CStage::Update()
{
	::ShowCursor(TRUE);
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_O))
		SaveObject();
	if (m_pEventMgr->GetQuestClear(2))
	{
		m_fCustomerCreateTime += ENGINE::CTimeMgr::GetInstance()->GetDeltaTime();
		if (m_fCustomerCreateTime >= 1.f)
		{
			if(m_mapLayer[ENGINE::LAYER_OBJECT]->GetObjectLst(L"Customer")->size() < 100)
				m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Customer", CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
			m_fCustomerCreateTime = 0.f;
		}
	}
	
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_P))
		LoadObject();
	//cout << "=======================" << endl;
	//cout << "Stage" << m_mapLayer[ENGINE::LAYER_OBJECT]->GetObjectLst(L"Customer")->size() << endl;
	int iEvent = NO_EVENT;
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_F1))
		CCameraMgr::GetInstance()->SetCameraIdx(0);
	if(ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_F2))
		CCameraMgr::GetInstance()->SetCameraIdx(1);
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_C))
	{
		m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Customer", CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	}

	// shoot
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_NUMPAD5))
	{
		CCameraMgr::GetInstance()->SetCameraIdx(0);
		ENGINE::CManagement::GetInstance()->SceneChange(
			CSceneSelector(SCENE_STAGE, SCENE_SHOOTINGGAME), ENGINE::SCENE_SUB);
		return CHANGE_SCENE;
	}
	// mole
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_NUMPAD4))
	{
		CCameraMgr::GetInstance()->SetCameraIdx(0);
		ENGINE::CManagement::GetInstance()->SceneChange(
			CSceneSelector(SCENE_STAGE, SCENE_MOLECATCHGAME), ENGINE::SCENE_SUB);
		return CHANGE_SCENE;
	}
	// bumper
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_NUMPAD6))
	{
		CCameraMgr::GetInstance()->SetCameraIdx(0);
		ENGINE::CManagement::GetInstance()->SceneChange(
			CSceneSelector(SCENE_STAGE, SCENE_BUMPERCARGAME), ENGINE::SCENE_SUB);
		return CHANGE_SCENE;
	}

	if (m_bBuildingUiRender)
	{
		m_bBuildingUiRender = m_pUiMgr->Update();
	}
	else
	{
		if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_TAB))
		{
			CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::PUSH_TAB);
			m_bBuildingUiRender = true;
			CUiMgr::GetInstance()->SetOpen(m_bBuildingUiRender);
		}
		CCameraMgr::GetInstance()->Update();
		iEvent = ENGINE::CScene::Update();	
	}
	m_pEventMgr->Update();

	return iEvent;
}

void CStage::LateUpdate()
{
	if (m_bBuildingUiRender)
	{
		m_pUiMgr->LateUpdate();
	}
	else
	{
		ENGINE::CScene::LateUpdate();
	}
	m_pEventMgr->LateUpdate();
	CSoundMgr::GetInstance()->Update();
}

void CStage::Render()
{
	if (m_bBuildingUiRender)
	{
		m_pUiMgr->Render();
	}
	//POINT pt;
	//GetCursorPos(&pt);
	//ScreenToClient(g_hWnd, &pt);
	//system("cls");
	//cout << pt.x << ", " << pt.y << endl;


	ENGINE::CScene::Render();
	m_pEventMgr->Render();

}



void CStage::Release()
{
	m_pResourceMgr->DeleteDynamicResources();
	m_pResourceMgr->DeleteStaticResources();
}

HRESULT CStage::Add_Environment_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_ENVIRONMENT, ENGINE::CLayer::Create(m_pGraphicDev)));

	//// Camera
	CCameraMgr::GetInstance()->SetGraphicDev(m_pGraphicDev);
	//HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"Camera", CDynamicCamera::Create(m_pGraphicDev));
	//FAILED_CHECK_MSG_RETURN(hr, L"Camera Create Failed", E_FAIL);

	// SkyBox
	HRESULT hr = m_mapLayer[ENGINE::LAYER_ENVIRONMENT]->AddObject(L"SkyBox", CSkyBox::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"SkyBox Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CStage::Add_Object_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_OBJECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	// Terrain
	HRESULT hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	m_mapLayer[ENGINE::LAYER_OBJECT]->CreateMap(L"Customer");
	
	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Player", pPlayer);
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);

	m_pEventMgr->Initialize(pPlayer, m_pGraphicDev);

	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Customer", CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	//FAILED_CHECK_MSG_RETURN(hr, L"Customer Create Failed", E_FAIL);


	////ÀÚÀÌ·Î½ºÀ®
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"GyroSwing", CGyroSwing::Create(m_pGraphicDev, { 30.f, 18.f, 40.f }, { 0.5f, 0.5f, 0.5f }, 90.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"GyroSwing Create Failed", E_FAIL);
	

	//LimitTile
	hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"LimitTile", CLimitTileMgr::Create(m_pGraphicDev, { 0.f, 10.f, 0.f }, { 20.f, 160.f, 1.f }, 0.f));
	FAILED_CHECK_MSG_RETURN(hr, L"LimitTile Create Failed", E_FAIL);

	////Shop
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Shop", CShopMgr::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"Shop Create Failed", E_FAIL);

	////Shop
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"CottenCandyShop", CCottenCandyMgr::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"CottenCandyShop Create Failed", E_FAIL);

	////Shop
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"ChurroShop", CChurroMgr::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"ChurroShop Create Failed", E_FAIL);
	//// Flower
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Flower", CFlowerMgr::Create(m_pGraphicDev, { 0.f,0.f ,0.f }, { 1.f,1.f ,1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"Flower Create Failed", E_FAIL);
	//// Flower
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"RedFlower", CRedFlowerMgr::Create(m_pGraphicDev, { 0.f,0.f ,0.f }, { 1.f,1.f ,1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"Flower Create Failed", E_FAIL);
	//// Flower
	//hr = m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"YellowFlower", CYellowFlowerMgr::Create(m_pGraphicDev, { 0.f,0.f ,0.f }, { 1.f,1.f ,1.f }, 0.f));
	//FAILED_CHECK_MSG_RETURN(hr, L"Flower Create Failed", E_FAIL);
	
	return S_OK;
}

HRESULT CStage::Add_Effect_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_EFFECT, ENGINE::CLayer::Create(m_pGraphicDev)));

	return S_OK;
}

HRESULT CStage::Add_UI_Layer()
{
	m_mapLayer.insert(make_pair(ENGINE::LAYER_UI, ENGINE::CLayer::Create(m_pGraphicDev)));

	// UiStateBar
	HRESULT hr = m_mapLayer[ENGINE::LAYER_UI]->AddObject(L"UiStateBar", CUiStateBar::Create(m_pGraphicDev, { -640.f, WINCY* 0.5f, 0.f }, { 1280.f, 50.f, 0.f }, L"Texture_UiStateBar"));
	FAILED_CHECK_MSG_RETURN(hr, L"UiStateBar Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CStage::SaveObject()
{
	HANDLE hFile = CreateFile(L"../Data/ObjData.bin", GENERIC_WRITE, 0, nullptr, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;

	map<wstring, list<ENGINE::CGameObject*>>* mapObjLst = m_mapLayer[ENGINE::LAYER_OBJECT]->GetMapObjectLst();
	for (auto& Pair : *mapObjLst)
	{
		if (Pair.first == L"Customer" || Pair.first == L"RollerCoaster" || Pair.first == L"LimitTile" || Pair.first == L"Player" || Pair.first == L"Terrain")
			continue;
		for (auto& Obj : Pair.second)
		{
			WriteFile(hFile, &Obj->GetSaveInfo(), sizeof(ENGINE::SAVE_INFO), &dwBytes, nullptr);
		}
	}

	CloseHandle(hFile);

	hFile = CreateFile(L"../Data/RollerCoasterData.bin", GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwBytes = 0;

	for (auto& Pair : *mapObjLst)
	{
		if (Pair.first == L"RollerCoaster")
		{
			for (auto& Obj : Pair.second)
			{
				WriteFile(hFile, &dynamic_cast<CRollerCoaster*>(Obj)->GetRollerInfo(), sizeof(ENGINE::ROLLER_INFO), &dwBytes, nullptr);
			}
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CStage::LoadObject()
{
	for (auto& Pair : *m_mapLayer[ENGINE::LAYER_OBJECT]->GetMapObjectLst())
	{
		if (Pair.first == L"Player" || Pair.first == L"LimitTile" || Pair.first == L"Terrain")
			continue;
		for (auto& Obj : Pair.second)
			ENGINE::SafeDelete(Obj);
		Pair.second.clear();
	}
	HANDLE hFile = CreateFile(L"../Data/ObjData.bin", GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;
	ENGINE::SAVE_INFO tInfo;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(ENGINE::SAVE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;
		wstring wstrRideTag;
		switch (tInfo.iRideTag)
		{
		case 0:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Viking", CViking::Create(m_pGraphicDev, 
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 1:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"SpinHorse", CSpinHorse::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 2:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Ferris", CFerrisWheel::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 3:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"GyroDrop", CGyroDrop::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 4:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"RollerCoaster", CRollerCoaster::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 5:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"SpinTeaCup", CSpinTeaCup::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 6:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"GyroSwing", CGyroSwing::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 7:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Shop", CShopMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 8:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"ChurrosShop", CChurroMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 9:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"CottonCandyShop", CCottenCandyMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 10:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"PinkFlower", CFlowerMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 11:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"RedFlower", CRedFlowerMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 12:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"CYellowFlower", CYellowFlowerMgr::Create(m_pGraphicDev,
				tInfo.vPos, tInfo.vSize, tInfo.fAngleY));
			break;
		case 13:
			m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"Tile", CTile::Create(m_pGraphicDev, tInfo.vPos, tInfo.tcTag, tInfo.iIdx, tInfo.fSize));
			break;
		default:
			break;
		}
	}
	CloseHandle(hFile);

	hFile = CreateFile(L"../Data/RollerCoasterData.bin", GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwBytes = 0;
	ENGINE::ROLLER_INFO tRollerInfo;
	ReadFile(hFile, &tRollerInfo, sizeof(ENGINE::ROLLER_INFO), &dwBytes, nullptr);

	m_mapLayer[ENGINE::LAYER_OBJECT]->AddObject(L"RollerCoaster", CRollerCoaster::Create(m_pGraphicDev,
		tRollerInfo.vPos, tRollerInfo.vSize, tRollerInfo.fAngleY, true, tRollerInfo));

	CloseHandle(hFile);


	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CStage* pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CStage::Initialize()
{
	srand(unsigned(time(nullptr)));

	m_eSceneType = ENGINE::SCENE_TYPE::SCENE_MAIN;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE); // Á¶¸í off	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	HRESULT hr = E_FAIL;

	//Default Buffer & Texture
	{
		// Player Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_STATIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_Player");
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Player Add Failed", E_FAIL);

		//// UiBase Buffer
		//hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		//	ENGINE::BUFFER_RCTEX, L"Buffer_UiBase");
		//FAILED_CHECK_MSG_RETURN(hr, L"Buffer_UiBase Add Failed", E_FAIL);

		// SkyBox Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_CUBETEX, L"Buffer_SkyBox", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SkyBox Add Failed", E_FAIL);

		// Monster Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_CUBECOL, L"Buffer_Monster", { 1.f, 0.f, 1.f }, { 2.f, 2.f, 2.f }, 0xffff00ff, ENGINE::PIVOT_TOP);
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Monster Add Failed", E_FAIL);

		// Terrain Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_TERRAINTEX, L"Buffer_Terrain", {}, {}, 0xffff0000, ENGINE::PIVOT_TYPE::PIVOT_MIDDLE, TERRAIN_CNTX, TERRAIN_CNTZ, TERRAIN_ITV);
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Terrain Add Failed", E_FAIL);

		// Tile Buffer
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEXLYING, L"Buffer_Tile", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 0.f, 1.f), ENGINE::PIVOT_MIDDLE);
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Tile Add Failed", E_FAIL);

		// Player Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_STATIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_Player", L"../Texture/Player%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_Player Add Failed", E_FAIL);

		// Ui Plate
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_UiBase", L"../Texture/Ui/UiBase%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"UiPlate Add Failed", E_FAIL);

		// TalkUi Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_UiTalk", L"../Texture/Ui/TalkBar%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"TalkUi Add Failed", E_FAIL);

		// UiNpc Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_UiNpc", L"../Texture/NPC/vurivuri%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_UiNpc Add Failed", E_FAIL);

		// UiStateBar Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_UiStateBar", L"../Texture/Ui/UiStateBar%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"UiStateBar Add Failed", E_FAIL);

		// SkyBox Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_CUBE, L"Texture_SkyBox", L"../Texture/SkyBox%d.dds", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_SkyBox Add Failed", E_FAIL);

		// Grass Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_Grass", L"../Texture/BillboardGrass%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_Grass Add Failed", E_FAIL);

		// Terrain Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_Terrain", L"../Texture/Terrain/Grass%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_Terrain Add Failed", E_FAIL);

		// Tile Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_Tile", L"../Texture/Terrain/Tile/Tile%d.png", 3);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_Tile Add Failed", E_FAIL);

		// Tile Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_TileDefault", L"../Texture/Terrain/Tile/Default/Tile%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_TileDefault Add Failed", E_FAIL);
	}

	////Ui Texture////
	{
		//VIKING_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_VikingUi", L"../Texture/Ui/Rides/Viking%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_VikingUi Add Failed", E_FAIL);

		//GYRODROP_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_GyroDropUi", L"../Texture/Ui/Rides/GyroDrop%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroDropUi Add Failed", E_FAIL);

		//SPINHORSE_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_SpinHorseUi", L"../Texture/Ui/Rides/SpinHorse%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_SpinHorseUi Add Failed", E_FAIL);

		//ROLLERCOASTER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_RollerCoasterUi", L"../Texture/Ui/Rides/RollerCoaster%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_RollerCoasterUi Add Failed", E_FAIL);

		//TEACUP_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_TeaCupUi", L"../Texture/Ui/Rides/TeaCup%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_TeaCupUi Add Failed", E_FAIL);

		//FERRIS_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_FerrisUi", L"../Texture/Ui/Rides/Ferris%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_FerrisUi Add Failed", E_FAIL);

		//FERRIS_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_GyroSwingUi", L"../Texture/Ui/Rides/GyroSwing%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroSwingUi Add Failed", E_FAIL);

		//FLOWER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_PinkFlowerUi", L"../Texture/Ui/Flower/Pink/PinkFlower%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_PinkFlowerUi Add Failed", E_FAIL);
		//FLOWER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_RedFlowerUi", L"../Texture/Ui/Flower/Red/RedFlower%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_RedFlowerUi Add Failed", E_FAIL);
		//FLOWER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_YellowFlowerUi", L"../Texture/Ui/Flower/Yellow/YellowFlower%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_YellowFlowerUi Add Failed", E_FAIL);

		//SHOP_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_ShopUi", L"../Texture/Ui/Shop/Shop/Shop%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_ShopUi Add Failed", E_FAIL);
		//FLOWER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_CottonCandyShopUi", L"../Texture/Ui/Shop/CottonCandyShop/CottonCandyShop%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_CottonCandyShopUi Add Failed", E_FAIL);
		//FLOWER_UI
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_ChurrosShopUi", L"../Texture/Ui/Shop/ChurrosShop/ChurrosShop%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_ChurrosShopUi Add Failed", E_FAIL);
	}

	//// Rides /////
	//// Buffer
	{
		// Viking Head Buffer
		{
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_VikingHead", { 0.f, 0.f, 0.f }, { 0.4f, 10.f, 0.4f }, 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_VikingHead Add Failed", E_FAIL);
			// Viking Bottom Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_VikingBottom", { 0.f, -10.f, 0.f }, { 10.f, 2.f, 4.f }, 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_VikingBottom Add Failed", E_FAIL);
			// Viking Left Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_VikingLeft", { -6.f, -9.5f, 0.f }, { 2.f, 3.f, 4.f }, 0xffff0000, ENGINE::PIVOT_LEFT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_VikingLeft Add Failed", E_FAIL);
			// Viking Right Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_VikingRight", { 6.f, -9.5f, 0.f }, { 2.f, 3.f, 4.f }, 0xffff0000, ENGINE::PIVOT_RIGHT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_VikingRight Add Failed", E_FAIL);
			// VikingLegL Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_VikingLegL", { 0.f, 10.f, 0.f }, { 2.f, 20.f, 2.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_VikingLegL Add Failed", E_FAIL);
		}
		// SpinHorse Buffer
		{
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_SpinHorse", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.5f, 9.f, 1.5f), D3DCOLOR_ARGB(255, 16, 255, 16), ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SpinHorse Add Failed", E_FAIL);

			// SpinHorseHorse Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBECOL, L"Buffer_SpinHorseHorse", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 3.f), D3DCOLOR_ARGB(255, 136, 0, 21), ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SpinHorseHorse Add Failed", E_FAIL);

			// SpinHorsePillar Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBECOL, L"Buffer_SpinHorsePillar", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.2f, 9.f, 0.2f), D3DCOLOR_ARGB(255, 128, 128, 128), ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SpinHorsePillar Add Failed", E_FAIL);

			// SpinHorsePlane Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_SpinHorsePlane", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(10.f, 1.f, 10.f), D3DCOLOR_ARGB(255, 128, 0, 128), ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SpinHorsePlane Add Failed", E_FAIL);
		}
		// GyroDrop Buffer
		{
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroDrop", { 0.f, 0.f, 0.f }, { 2.f, 40.f, 2.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroDrop Add Failed", E_FAIL);

			// GyroDropDevice Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroDevice", { 0.f, 0.f, 0.f }, { 20.f, 3.f, 2.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroDevice Add Failed", E_FAIL);
		}
		// FerrisBody Buffer
		{
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_FerrisBody", { 0.f, 0.f, 0.f }, { 1.5f, 1.5f, 1.5f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_FerrisBody Add Failed", E_FAIL);

			// FerrisArm Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_FerrisArm", { 0.f, 0.f, 0.f }, { 1.f, 30.f, 1.f }, 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_FerrisArm Add Failed", E_FAIL);

			// FerrisSeat Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_FerrisSeat", { 0.f, -5.f, 0.f }, { 3.f, 3.f, 3.f }, 0xffff0000, ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_FerrisSeat Add Failed", E_FAIL);

			// FerrisLeg Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_FerrisLeg", { 0.f, 0.f, 0.f }, { 1.f, 30.f, 1.f }, 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_FerrisLeg Add Failed", E_FAIL);
		}
		// RollerCoster Buffer
		{
			//Rail
			//Left
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBECOL, L"Buffer_RailLeft", { -1.f, 0.f, 0.f }, { 0.3f, 0.3f, 2.5f }, 0x8888888, ENGINE::PIVOT_FRONT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_RailLeft Add Failed", E_FAIL);

			//Right
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBECOL, L"Buffer_RailRight", { 1.f, 0.f, 0.f }, { 0.3f, 0.3f, 2.5f }, 0x8888888, ENGINE::PIVOT_FRONT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_RailRight Add Failed", E_FAIL);

			//Middle
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBECOL, L"Buffer_RailMiddle", { 0.f, 0.f, 1.f }, { 2.f, 0.3f, 0.3f }, 0x8888888, ENGINE::PIVOT_FRONT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_RailMiddle Add Failed", E_FAIL);


			//Buffer_RailTexture_Rail
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_RCTEXLYING, L"Buffer_Rail", { 0.f, 0.f, 1.f }, { 2.f, 0.f, 2.5f }, 0x8888888, ENGINE::PIVOT_FRONT);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Rail Add Failed", E_FAIL);

			//Train
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_Train", { 0.f, 0.f, 0.f }, { 2.f, 1.f, 3.f }, 0x8888888, ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Train Add Failed", E_FAIL);
		}
		// SpinTeaCup Buffer
		{
			// SpinTeaCup Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_SpinTeaCup", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(16.f, 1.f, 16.f), 0xffff0000, ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_SpinTeaCup Add Failed", E_FAIL);

			// SpinTeaCup TeaCup Buffer
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_TeaCup", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"BUffer_TeaCup Add Failed", E_FAIL);
		}
		//GyroSwing Buffer
		{
			//Body
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroSwingBody", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.5f, 1.5f, 1.5f), 0xffff0000, ENGINE::PIVOT_MIDDLE);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroSwingBody Add Failed", E_FAIL);
			//Leg
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroSwingLeg", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.5f, 20.f, 1.5f), 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroSwingLeg Add Failed", E_FAIL);
			//Device
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroSwingDevice", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.5f, 20.f, 1.5f), 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroSwingDevice Add Failed", E_FAIL);

			//Spin
			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_GyroSwingSpin", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(30.5f, 1.5f, 1.5f), 0xffff0000, ENGINE::PIVOT_TOP);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_GyroSwingSpin Add Failed", E_FAIL);

		}
		//LimitTile Buffer
		{

			hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::BUFFER_CUBETEX, L"Buffer_LimitTile", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f), 0xffff0000, ENGINE::PIVOT_BOTTOM);
			FAILED_CHECK_MSG_RETURN(hr, L"Buffer_LimitTile Add Failed", E_FAIL);
		}
		//LimitTile Texture
		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_CUBE, L"Texture_LimitTile", L"../Texture/Limit%d.dds", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_LimitTile Add Failed", E_FAIL);
	}

	//// Texture
	{
		// Viking Texture
		{
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_Viking", L"../Texture/Wood%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Viking Add Failed", E_FAIL);
			// VikingLeg Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_VikingLeg", L"../Texture/Viking/WoodLeg%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_VikingLeg Add Failed", E_FAIL);
		}
		// SpinHorse Texture
		{
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_SpinHorse", L"../Texture/SpinHorse/block%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_SpinHorse Add Failed", E_FAIL);

			// SpinHorsePlane Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_SpinHorsePlane", L"../Texture/SpinHorse/SpinHorsePlane%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_SpinHorsePlane Add Failed", E_FAIL);
		}
		//GyroDrop
		{
			// GyroBody Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_GyroBody", L"../Texture/GyroDrop/GyroBody%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroBody Add Failed", E_FAIL);
			// GyroDevice Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_GyroDevice", L"../Texture/GyroDrop/GyroDevice%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroDevice Add Failed", E_FAIL);
		}
		//Ferris
		{
			// FerrisSeat Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_FerrisSeat", L"../Texture/FerrisWheel/FerrisSeat%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_FerrisSeat Add Failed", E_FAIL);
			// FerrisArm Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_FerrisArm", L"../Texture/FerrisWheel/FerrisArm%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_FerrisArm Add Failed", E_FAIL);
			// FerrisLeg Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_FerrisLeg", L"../Texture/FerrisWheel/FerrisLeg%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_FerrisLeg Add Failed", E_FAIL);
		}
		// SpinTeaCup Texture
		{
			// SpinTeaCup Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_SpinTeaCup", L"../Texture/SpinTeaCup/CoffeePlane%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_SpinTeaCup Add Failed", E_FAIL);

			// TeaCup Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_TeaCup", L"../Texture/SpinTeaCup/Coffee%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_TeaCup Add Failed", E_FAIL);
		}
		//RollerCoaster Texture
		{
			//Train
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_Train", L"../Texture/RollerCoaster/Train%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Train Add Failed", E_FAIL);

			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Rail", L"../Texture/RollerCoaster/Rail%d.png", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Rail Add Failed", E_FAIL);
		}
		//GyroSwing
		{
			// GyroSwingDevice Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_GyroSwingDevice", L"../Texture/GyroSwing/GyroSwingDevice%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroSwingDevice Add Failed", E_FAIL);

			// GyroSwingLeg Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_GyroSwingLeg", L"../Texture/GyroSwing/GyroSwingLeg%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroSwingLeg Add Failed", E_FAIL);

			// GyroSwingSpin Texture
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_CUBE, L"Texture_GyroSwingSpin", L"../Texture/GyroSwing/GyroSwingSpin%d.dds", 1);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_GyroSwingSpin Add Failed", E_FAIL);

		}
	}

	//Customer
	{
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_Customer", { 0.f, 1.f, 0.f }, { 2.f, 2.f, 2.f });
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Customer Add Failed", E_FAIL);

		//Customer1
		{
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Run1", L"../Texture/Customer/Normal/Run/Customer%d.png", 8);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Run1 Add Failed", E_FAIL);

			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Idle1", L"../Texture/Customer/Normal/Idle/Customer%d.png", 4);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Idle1 Add Failed", E_FAIL);

			//////////////////////////////////////////////////////////////////////////
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Run2", L"../Texture/Customer/Hero/Run/CharRun%d.png", 8);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Run2 Add Failed", E_FAIL);

			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Idle2", L"../Texture/Customer/Hero/Idle/CharIdle%d.png", 4);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Idle2 Add Failed", E_FAIL);

			//////////////////////////////////////////////////////////////////////////
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Run3", L"../Texture/Customer/Girl/Run/CharRun%d.png", 8);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Run3 Add Failed", E_FAIL);

			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Idle3", L"../Texture/Customer/Girl/Idle/CharIdle%d.png", 4);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Idle3 Add Failed", E_FAIL);

			//////////////////////////////////////////////////////////////////////////
			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Run4", L"../Texture/Customer/Man/Run/CharRun%d.png", 8);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Run4 Add Failed", E_FAIL);

			hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
				ENGINE::TEXTURE_NORMAL, L"Texture_Customer_Idle4", L"../Texture/Customer/Man/Idle/CharIdle%d.png", 4);
			FAILED_CHECK_MSG_RETURN(hr, L"Texture_Customer_Idle4 Add Failed", E_FAIL);
		}
		//CustomerGage
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_CustomerGage", { -1.35f, 0.f, 0.f }, { 3.5f, 0.7f, 1.f }, 4294967295UL, ENGINE::PIVOT_LEFT);
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_CustomerGage Add Failed", E_FAIL);

		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_CustomerGage", L"../Texture/Customer/Ui/HeartInner%d.png", 1);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_CustomerGage Add Failed", E_FAIL);

		//CustomerUi
		hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::BUFFER_RCTEX, L"Buffer_CustomerUi", { 0.f, 0.f, 0.f }, { 5.f, 1.f, 1.f });
		FAILED_CHECK_MSG_RETURN(hr, L"Buffer_CustomerUi Add Failed", E_FAIL);

		hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
			ENGINE::TEXTURE_NORMAL, L"Texture_CustomerUi", L"../Texture/Customer/Ui/HeartBar%d.png", 11);
		FAILED_CHECK_MSG_RETURN(hr, L"Texture_CustomerUi Add Failed", E_FAIL);
	}
	//shop
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_Shop", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Shop Add Failed", E_FAIL);

	//shop Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_Shop", L"../Texture/Shop/Shop%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Shop Add Failed", E_FAIL);

	//Cottenshop
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_CottenCandyShop", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_CottenCandyShop Add Failed", E_FAIL);

	//shop Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_CottenCandyShop", L"../Texture/CottenCandyShop/CottenCandyShop%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_CottenCandyShop Add Failed", E_FAIL);

	//Churroshop
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_CUBETEX, L"Buffer_ChurroShop", { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0xffff0000, ENGINE::PIVOT_MIDDLE);
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_ChurroShop Add Failed", E_FAIL);

	//shop Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_CUBE, L"Texture_ChurroShop", L"../Texture/ChurroShop/ChurroShop%d.dds", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_ChurroShop Add Failed", E_FAIL);

	// Flower Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_Flower");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_Flower Add Failed", E_FAIL);
	// Flower Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_Flower", L"../Texture/Flower/²É%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_Flower Add Failed", E_FAIL);

	// Flower Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_YellowFlower");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_YellowFlower Add Failed", E_FAIL);
	// Flower Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_YellowFlower", L"../Texture/Flower/³ë¶õ²É%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_YellowFlower Add Failed", E_FAIL);

	// Flower Buffer
	hr = m_pResourceMgr->AddBuffer(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::BUFFER_RCTEX, L"Buffer_RedFlower");
	FAILED_CHECK_MSG_RETURN(hr, L"Buffer_RedFlower Add Failed", E_FAIL);
	// Flower Texture
	hr = m_pResourceMgr->AddTexture(m_pGraphicDev, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEXTURE_NORMAL, L"Texture_RedFlower", L"../Texture/Flower/»¡°£²É%d.png", 1);
	FAILED_CHECK_MSG_RETURN(hr, L"Texture_RedFlower Add Failed", E_FAIL);

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::CScene::Initialize(), E_FAIL);


	m_pUiMgr->SetGraphicDev(m_pGraphicDev);
	CSoundMgr::GetInstance()->PlayBGM(L"³îÀÌµ¿»êºê±Ý.mp3");
	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.2f);
	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);

	return S_OK;
}