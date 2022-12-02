#include "stdafx.h"
#include "ShootingPlayer.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "PipeLine.h"
#include"ShootingBullet.h"
#include"FpsCamera.h"


#include "SoundMgr.h"
CShootingPlayer::CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr),
	m_pCameraMgr(CCameraMgr::GetInstance())
{

}

CShootingPlayer::~CShootingPlayer()
{
	Release();
}

int CShootingPlayer::Update()
{
	ENGINE::CGameObject::Update();

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(pObserver->GetCameraPos());
	m_vLook = pObserver->GetCameraLook();
	m_vRight = VEC3::CrossProduct(Vec3(0.f, 1.f, 0.f), m_vLook, true);
	m_vUp = VEC3::CrossProduct(m_vLook, m_vRight, true);

	KeyInput();

	for (auto& iter : m_lBullet)
	{
		iter->Update();
	}




	list<CShootingBullet*>::iterator iter_begin = m_lBullet.begin();
	list<CShootingBullet*>::iterator iter_end = m_lBullet.end();
	for (; iter_begin != iter_end;)
	{
		int iEvent = (*iter_begin)->Update();

		if (DEAD_OBJ == iEvent)
		{
			ENGINE::SafeDelete(*iter_begin);
			iter_begin = m_lBullet.erase(iter_begin);
		}
		else
			++iter_begin;
	}
	return NO_EVENT;
}

void CShootingPlayer::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetWorldMatrixForRightUpLookPos(m_vRight, m_vUp, m_vLook, pTransform->GetPos());

	for (auto& iter : m_lBullet)
	{
		iter->LateUpdate();
	}

}

void CShootingPlayer::Render()
{
	for (auto& iter : m_lBullet)
	{
		iter->Render();
	}

	//ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	//NULL_CHECK(pTransform);
	//
	//CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	//NULL_CHECK(pObserver);
	//
	//for (int i = 0; i < 4; ++i)
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
	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if(m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);
	//
	////m_mapComponent[L"Texture"]->Render(0);
	//m_mapComponent[L"Buffer"]->Render();
}

HRESULT CShootingPlayer::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	//NULL_CHECK_RETURN(pBuffer, E_FAIL);
	//
	//m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	//NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);
	//
	//m_pConvertVtx = new ENGINE::VTX_TEX[4];
	//memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	return S_OK;
}

HRESULT CShootingPlayer::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	//// Buffer
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_STATIC, L"Buffer_Player");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Player Clone Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Buffer", pComponent));
	//
	//// Texture
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_STATIC, L"Texture_Player");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Player Clone Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Texture", pComponent));

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

	//// Collision Terrain
	//pComponent = CCollTerrain::Create(this);
	//NULL_CHECK_MSG_RETURN(pComponent, L"CollTerrain Create Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"CollTerrain", pComponent));

	return S_OK;
}

void CShootingPlayer::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	for_each(m_lBullet.begin(), m_lBullet.end(), ENGINE::SafeDelete<CShootingBullet*>);

}

void CShootingPlayer::KeyInput()
{
	if (ENGINE::CKeyMgr::GetInstance()->KeyDown(ENGINE::KEY_LBUTTON))
	{
		m_lBullet.emplace_back(CShootingBullet::Create(m_pGraphicDev, this->GetPos(), m_vLook));
		dynamic_cast<CFpsCamera*>(m_pCameraMgr->GetCurrentCamera())->CameraRebound(3.f);


		CSoundMgr::GetInstance()->PlaySound(L"총소리.mp3", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::EFFECT, 0.05f);


	}
}

CShootingPlayer* CShootingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShootingPlayer* pInstance = new CShootingPlayer(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
