#include "stdafx.h"
#include "Tile.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "PipeLine.h"
#include "CollMouse.h"
#include "DeadObj.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CTile::CTile(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),	
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr)
{
	
}

CTile::~CTile()
{
	Release();
}

int CTile::Update()
{
	KeyInput();
	return ENGINE::CGameObject::Update();
}

void CTile::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();	
}

void CTile::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 4; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos, 
	//		&m_pOriginVtx[i].vPos, 
	//		&pTransform->GetWorldMatrix());

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());

	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if(m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	if (m_bDefault)
		m_mapComponent[L"Texture_TileDefault"]->Render(0);
	else
		m_mapComponent[L"Texture"]->Render((WORD)m_iTileIndex);
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CTile::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	return S_OK;
}

HRESULT CTile::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	if (m_iTileIndex >= m_pResourceMgr->GetInstance()->GetTextureSize(ENGINE::RESOURCE_DYNAMIC, m_wstrTileTag))
		return E_FAIL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Tile");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Tile Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Tile");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Tile Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_TileDefault");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_TileDefault Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_TileDefault", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = m_vPos;
	D3DXVECTOR3 vSize = { (float)TERRAIN_ITV, 1.f, (float)TERRAIN_ITV };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));	
	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	//// Collision Terrain
	//pComponent = CCollTerrain::Create(this);
	//NULL_CHECK_MSG_RETURN(pComponent, L"CollTerrain Create Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"CollTerrain", pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	//Collision Mouse
	pComponent = CCollMouse::Create(pObserver);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollMouse Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"CollMouse", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	return S_OK;
}

void CTile::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CTile::KeyInput()
{
	if (CUiMgr::GetInstance()->GetwstrSelect() != L"Delete")
		return;
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_RBUTTON))
	{
		CCollMouse* pCollMouse = dynamic_cast<CCollMouse*>(m_mapComponent[L"CollMouse"]);
		NULL_CHECK(pCollMouse);
	
		D3DXVECTOR3 vPickPos = {};
		bool		bPick = false;
	
		ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	
		bPick = pCollMouse->PickTile(&vPickPos, ENGINE::LAYER_OBJECT, L"Terrain", m_pOriginVtx, pTransform->GetWorldMatrix());
	
		if (bPick)
		{
			CSoundMgr::GetInstance()->PlaySound(L"뿅.mp3", CSoundMgr::CHANNELID::EFFECT);
			//cout << "삭제" << endl;
			//DeadObj추가해서 삭제
			dynamic_cast<ENGINE::CDeadObj*>(m_mapComponent[L"DeadObj"])->SetDead();
			//타일을 레이어로 옮기기
			if (m_bDefault)
			{
				list<ENGINE::CGameObject*>* Rides;
				float fDistCriteria = 9999.f;
				float fDist = 0.f;
				ENGINE::CGameObject* pRide = nullptr;

				//가까운 놀이기구 찾기
				wstring wstrRideNames[] = {L"Viking", L"SpinHorse", L"Ferris", L"GyroDrop", L"RollerCoaster", L"SpinTeaCup", L"GyroSwing"};
				for (auto RideName : wstrRideNames)
				{
					Rides = ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_OBJECT, RideName);
					if (Rides == nullptr)
						continue;
					for (auto Ride : *Rides)
					{
						fDist = VEC3::Length(m_vPos - Ride->GetPos());
						if (fDistCriteria > fDist)
						{
							fDistCriteria = fDist;
							pRide = Ride;
						}
					}
				}
				bool b = ENGINE::CTIleMgr::GetInstance()->GetTileIdx(m_vPos);
				ENGINE::CTIleMgr::GetInstance()->DeleteTile(m_vPos - Vec3(TERRAIN_ITV, 0.f, TERRAIN_ITV), 5);
				dynamic_cast<ENGINE::CDeadObj*>(pRide->GetComponent(L"DeadObj"))->SetDead();
			}
		}
	}
}

ENGINE::SAVE_INFO CTile::GetSaveInfo()
{
	ENGINE::SAVE_INFO tInfo = ENGINE::CGameObject::GetSaveInfo();
	tInfo.iIdx = m_iTileIndex;
	tInfo.fSize = m_fSize;
	lstrcpy(tInfo.tcTag, m_wstrTileTag.c_str());
	//tInfo.tcTag = m_wstrTileTag;
	tInfo.iRideTag = 13;
	tInfo.vPos = m_vPos;
	return tInfo;
}

CTile* CTile::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, wstring wstrTag, int iIndex, float fSize)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CTile* pInstance = new CTile(pGraphicDev);
	pInstance->m_vPos = vPos + Vec3(0.f, 0.01f, 0.f);
	pInstance->m_wstrTileTag = wstrTag;
	pInstance->m_iTileIndex = iIndex;
	pInstance->m_fSize = fSize;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	if (fSize != 1.f)
	{
		pInstance->SetCreatePosSize((vPos - Vec3(fSize / 2.f * TERRAIN_ITV + TERRAIN_ITV / 2, -0.01f, fSize / 2.f * TERRAIN_ITV + TERRAIN_ITV / 2)),
			Vec3((float)TERRAIN_ITV, 1.f, (float)TERRAIN_ITV) * fSize + Vec3((float)TERRAIN_ITV, 1.f, (float)TERRAIN_ITV), 0.f);
		pInstance->m_bDefault = true;
	}
	else
		pInstance->m_bDefault = false;
	return pInstance;
}
