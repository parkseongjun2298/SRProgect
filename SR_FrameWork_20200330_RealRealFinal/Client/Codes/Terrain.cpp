#include "stdafx.h"
#include "Terrain.h"
#include "TerrainTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "CollMouse.h"
#include "Tile.h"
#include "TileMgr.h"

#include "Viking.h"
#include "SpinHorse.h"
#include "GyroDrop.h"
#include "RollerCoaster.h"
#include "SpinTeaCup.h"
#include "FerrisWheel.h"
#include "GyroSwing.h"

#include "ShopMgr.h"
#include "CottenCandyMgr.h"
#include "ChurroMgr.h"
#include "FlowerMgr.h"
#include "RedFlowerMgr.h"
#include "YellowFlowerMgr.h"

#include "SoundMgr.h"
#include "UiStateBar.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pUiMgr(CUiMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr)
{

}

CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::Update()
{
	ENGINE::CGameObject::Update();
	KeyInput();

	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
}

void CTerrain::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());


	//for (int i = 0; i < TERRAIN_CNTX * TERRAIN_CNTZ; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&pTransform->GetWorldMatrix());
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());
	//
	//	// ±ÙÆò¸é°ú ¿øÆò¸é ¿ÜºÎ¿¡ Á¸ÀçÇÏ´Â Á¤Á¡µéÀº Åõ¿µº¯È¯¿¡¼­ Á¦¿Ü ½ÃÅ²´Ù.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	//m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CTerrain::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[TERRAIN_CNTX * TERRAIN_CNTZ];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * TERRAIN_CNTX * TERRAIN_CNTZ);

	return S_OK;
}

HRESULT CTerrain::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Terrain");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Terrain Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Terrain");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Terrain Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));
	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	//Collision Mouse
	pComponent = CCollMouse::Create(pObserver);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollMouse Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"CollMouse", pComponent));

	return S_OK;
}

void CTerrain::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CTerrain::KeyInput()
{
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_LBUTTON))
	{
		CCollMouse* pCollMouse = dynamic_cast<CCollMouse*>(m_mapComponent[L"CollMouse"]);
		NULL_CHECK(pCollMouse);

		D3DXVECTOR3 vPickPos = {};
		bool		bPick = false;

		bPick = pCollMouse->PickTerrain(&vPickPos, ENGINE::LAYER_OBJECT, L"Terrain");
		vPickPos.x = (float)((int)vPickPos.x / TERRAIN_ITV) * TERRAIN_ITV;
		vPickPos.y += 0.0001f;
		vPickPos.z = (float)((int)vPickPos.z / TERRAIN_ITV) * TERRAIN_ITV;

		if (bPick)
		{
			//list<CGameObject*>* pLstTile = ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_OBJECT, L"Tile");
			//if (pLstTile != nullptr)
			//{
			//	if (pLstTile->size() != 0)
			//	{
			//		auto iter_find = find_if(pLstTile->begin(), pLstTile->end(), [&](CGameObject* Tile) {
			//			return (dynamic_cast<ENGINE::CTransform*>(Tile->GetComponent(L"Transform"))->GetPos() == vPickPos);
			//		});
			//		if (iter_find != pLstTile->end())
			//		{
			//			cout << "ÀÌ¹Ì Å¸ÀÏÀÌ ÀÖ½À´Ï´Ù." << endl;
			//			return;
			//		}
			//	}
			//}
			if (m_pUiMgr->GetwstrSelect() == L"Texture_Tile")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos, 0, ENGINE::TILE_TILE)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(1);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::TILE_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile", 
					CTile::Create(m_pGraphicDev, vPickPos, m_pUiMgr->GetwstrSelect(), m_pUiMgr->GetiSelect()));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);

				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-10);
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_VikingUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(3);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::VIKING_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Viking",
					CViking::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 13.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_SpinHorseUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(2);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::SPINHORSE_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"SpinHorse",
					CSpinHorse::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_GyroDropUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(4);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::GYRODROP_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"GyroDrop",
					CGyroDrop::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 20.f, 0.f), { 1.f, 1.f, 1.f, }));// , 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_RollerCoasterUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(5);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::ROLLERCOASTER_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"RollerCoaster",
					CRollerCoaster::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_TeaCupUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(6);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::SPINTEACUP_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"SpinTeaCup",
					CSpinTeaCup::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 3.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_FerrisUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(7);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::FERRIS_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Ferris",
					CFerrisWheel::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 43.f, 0.f), { 1.f, 1.f, 1.f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_GyroSwingUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5, ENGINE::TILE_RIDES)))
					return;
				CEventMgr::GetInstance()->SetQuestClear(7);
				CEventMgr::GetInstance()->NotifyQuestCondition(QUEST_CONDITION::FERRIS_CREATE);
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"GyroSwing",
					CGyroSwing::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 18.f, 0.f), { 0.5f, 0.5f, 0.5f, }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 5.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_ShopUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 2, ENGINE::TILE_RIDES)))
					return;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Shop",
					CShopMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 2.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_CottonCandyShopUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 2, ENGINE::TILE_RIDES)))
					return;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"CottonCandyShop",
					CCottenCandyMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 2.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_ChurrosShopUi")
			{
				if (FAILED(ENGINE::CTIleMgr::GetInstance()->SetTile(vPickPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 2, ENGINE::TILE_RIDES)))
					return;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"ChurrosShop",
					CChurroMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Tile",
					CTile::Create(m_pGraphicDev, vPickPos, L"Texture_Tile", m_pUiMgr->GetiSelect(), 2.f));
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-1000);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_PinkFlowerUi")
			{
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"PinkFlower",
					CFlowerMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-10);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_YellowFlowerUi")
			{
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"YellowFlower",
					CYellowFlowerMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-10);
			}
			else if (m_pUiMgr->GetwstrSelect() == L"Texture_RedFlowerUi")
			{
				ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"RedFlower",
					CRedFlowerMgr::Create(m_pGraphicDev, vPickPos + Vec3(0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, 0.f));
				//cout << vPickPos.x << ", " << vPickPos.y << ", " << vPickPos.z << endl;
				CSoundMgr::GetInstance()->PlaySound(L"»Ð.mp3", CSoundMgr::CHANNELID::EFFECT);
				dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(-10);
			}
		}
	}
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CTerrain* pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
