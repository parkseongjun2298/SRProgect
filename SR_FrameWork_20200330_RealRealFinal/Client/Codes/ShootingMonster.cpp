#include "stdafx.h"
#include "ShootingMonster.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "PipeLine.h"

#include "EffectCountShot.h"

CShootingMonster::CShootingMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr), m_fAngle(0), m_fSpeed(4.f), m_fAngle2(0), m_Score(0), m_bDead(false)
{

}

CShootingMonster::~CShootingMonster()
{
	Release();
}

int CShootingMonster::Update()
{
	if (m_bDead)
		return DEAD_OBJ;
	ENGINE::CGameObject::Update();

	/*CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(pObserver->GetCameraPos());
	m_vLook = pObserver->GetCameraLook();
	m_vRight = VEC3::CrossProduct(Vec3(0.f, 1.f, 0.f), m_vLook, true);
	m_vUp = VEC3::CrossProduct(m_vLook, m_vRight, true);*/

	MoveMonster();

	return NO_EVENT;
}

void CShootingMonster::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	/*ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetWorldMatrixForRightUpLookPos(m_vRight, m_vUp, m_vLook, pTransform->GetPos());*/


}

void CShootingMonster::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 4; ++i)
	{
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&pTransform->GetWorldMatrix());

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetViewMatrix());

		// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();


	MonEffectRender();
}

HRESULT CShootingMonster::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);


	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	//Effect
	m_pEffectCount = CEffectCountShot::Create(m_pGraphicDev, pTransform->GetPos() + D3DXVECTOR3(0.f, 1.f, 0.f), { 0.9f, 0.68f, 1.f });
	NULL_CHECK_MSG_RETURN(m_pEffectCount, L"EffectCount Create Failed", E_FAIL);

	return S_OK;
}

HRESULT CShootingMonster::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Monster");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Monster Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_STATIC, L"Texture_Moster");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Moster Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };

	srand(unsigned(time(NULL)));
	float Temp, A(2);
	Temp = rand() % (int)A + 1.f;
	m_tInfo.vSize = { Temp ,Temp ,Temp };

	if (m_tInfo.vSize.x == 2)
	{
		m_Score = 5.f;
		m_bIsAngry = false;
	}
		if (m_tInfo.vSize.x == 1)
		{
			m_Score = 10.f;
			m_bIsAngry = true;
		}
	pComponent = ENGINE::CTransform::Create(vLook, m_tInfo.vPos, m_tInfo.vSize);
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

void CShootingMonster::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	ENGINE::SafeDelete(m_pEffectCount);
}

void CShootingMonster::MonEffectUpdate()
{
	// 렌더중이라면 취소.
	if (m_pEffectCount->Get_RenderState())
		return;

	/*ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);*/

	//m_pEffectCount->Set_RenderState(true);
	//m_pEffectCount->Set_Pos(Get_Pose() + D3DXVECTOR3(0.f, 0.5f, 0.f));
}

void CShootingMonster::MonEffectRender()
{
	if (!m_pEffectCount->Get_RenderState())
		return;

	if(m_bIsAngry)
		m_pEffectCount->Set_Minus(true);
	else
		m_pEffectCount->Set_Minus(false);

	m_pEffectCount->Update();
	m_pEffectCount->LateUpdate();
	m_pEffectCount->Render();
}

int CShootingMonster::MoveMonster()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	m_fTime += ENGINE::CTimeMgr::GetInstance()->GetDeltaTime();

	m_pEffectCount->Set_RenderState(true);
	m_pEffectCount->Set_Pos(pTransform->GetPos() + D3DXVECTOR3(0.f, 2.f, 0.f));
	if (m_bDead)
	{
		MonEffectUpdate();
	}

	if (m_fTime >= 2)
	{
		D3DXVECTOR3 Temp = { 0,0,0 };
		Temp.x = m_fSpeed * ENGINE::CTimeMgr::GetInstance()->GetDeltaTime();
		pTransform->MoveShift(Temp);

		//cout << pTransform->GetPos().x << endl;
		if (pTransform->GetPos().x >= 20)
		{
			m_fSpeed = -2;
		}
		if (pTransform->GetPos().x <= -20)
		{
			m_fSpeed = 2;
		}
	}

	return NO_EVENT;
}


CShootingMonster* CShootingMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShootingMonster* pInstance = new CShootingMonster(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
