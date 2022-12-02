#include "stdafx.h"
#include "SuperCar.h"
#include "CubeTex.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CameraObserver.h"

CSuperCar::CSuperCar(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr), m_pTarget(nullptr), 
	m_iTextureIndex(0)
{
}


CSuperCar::~CSuperCar()
{
	Release();
}

int CSuperCar::Update()
{
	ENGINE::CGameObject::Update();

	// 밖으로 나가 아웃상태가 아니면서,
	// 충돌하여 팅겨나가는 중이 아니라면
	if (!m_bIsOut && !ReBound())
	{
		// 가속중이라면 가속.
		if (m_bIsAccel)
			Accel();
		// 가속중이 아니라면 감속.
		else
			Decel();

	}
	// 단, 충돌하였을때
	// 아웃상태라면 스피드를 0으로 만들어준다.
	if (m_bIsOut)
	{
		OutAnimation();
		m_fSpeed = 0;
	}


	return NO_EVENT;
}

void CSuperCar::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	pTransform->SetWorldMatrixForRightUpLookPos(m_vRight, m_vUp, m_vLook, pTransform->GetPos());
}

void CSuperCar::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	for (int i = 0; i < 8; ++i)
	{
		D3DXMATRIX matWorld;
		matWorld = pTransform->GetWorldMatrix() * pObserver->GetViewMatrix();
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&matWorld);

		// 투영스페이스 범위 밖에 정점들을 투영변환에서 제외시키기
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());

	}
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(m_iTextureIndex);  // m_iTextureIndex : 나중에 자동차의 색깔들이 다르게 출력하기위해. (0 : 플레이어의 슈퍼카, 1~? : Ai의 슈퍼카 )
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CSuperCar::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_fMaxSpeed = 25.f;
	m_fAccelSpeed = 0.f;
	m_fSpeed = 0.f;
	m_fSize = 4.f;
	m_iHp = 1;

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	return S_OK;
}

HRESULT CSuperCar::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SuperCar");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SuperCar Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SuperCar");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SuperCar Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_vRight = { 1.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };
	m_vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SuperCar Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));
	return S_OK;
}

void CSuperCar::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CSuperCar::Accel()
{
	if (m_fMaxSpeed <= m_fSpeed)
		m_fSpeed = m_fMaxSpeed;

	m_fSpeed += m_fMaxSpeed * m_pTimeMgr->GetDeltaTime();
}

void CSuperCar::Decel()
{
	if (0.f >= m_fSpeed)
		m_fSpeed = 0.f;

	m_fSpeed -= m_fMaxSpeed * m_pTimeMgr->GetDeltaTime() / 1.5f;
}

bool CSuperCar::ReBound()
{
	if (D3DXVec3Length(&m_vReBoundDir) == 0)
		return false;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, false);

	m_fReBoundTime += m_fReBoundDistance * m_pTimeMgr->GetDeltaTime();
	float fReBoundSpeed = m_fReBoundDistance * m_pTimeMgr->GetDeltaTime();

	if (m_fReBoundDistance <= m_fReBoundTime)
	{
		m_vReBoundDir = { 0.f, 0.f, 0.f };
		m_fReBoundTime = 0.f;
		m_fReBoundDistance = 0.f;
	}
	else
	{
		pTransform->MoveShift(m_vReBoundDir * fReBoundSpeed);
		m_fSpeed = 0.f;
	}

	return true;
}

void CSuperCar::OutAnimation()
{
	if (!m_bIsOut)
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	float fAngleSpeed = 360.f * m_pTimeMgr->GetDeltaTime();
	float fFallSpeed = 10.f * m_pTimeMgr->GetDeltaTime();

	// 4. 구한 각도로 회전행렬 만들어서 vRight, vUp, vLook 재정의.
	D3DXMATRIX matRot = MATRIX::RotateY(D3DXToRadian(-fAngleSpeed));
	m_vLook = VEC3::TransformNormal(m_vLook, matRot);
	m_vRight = VEC3::CrossProduct({ 0.f, 1.f, 0.f }, m_vLook);
	m_vUp = { 0.f, 1.f ,0.f };
	pTransform->SetPos(pTransform->GetPos() + D3DXVECTOR3(0.f, -fFallSpeed, 0.f));
}


const D3DXVECTOR3& CSuperCar::Get_Look() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vLook;
}

const D3DXVECTOR3& CSuperCar::Get_Pos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->GetPos();
}

const float & CSuperCar::Get_Speed() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_fSpeed;
}

const float & CSuperCar::Get_Size() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_fSize;
}

const int & CSuperCar::Get_Hp() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_iHp;
}

void CSuperCar::Set_Target(CSuperCar * pTarget)
{
	m_pTarget = pTarget;
}

void CSuperCar::Set_TextureIndex(int iIndex)
{
	m_iTextureIndex = iIndex;
}

void CSuperCar::Set_ReBoundDir(D3DXVECTOR3 vDir)
{
	m_vReBoundDir = vDir;
}

void CSuperCar::Set_ReBoundDistance(float fDistance)
{
	m_fReBoundDistance = fDistance;
}

void CSuperCar::Set_Accel(bool bCheck)
{
	m_bIsAccel = bCheck;
}

void CSuperCar::Set_Hp(int iHp)
{
	m_iHp = iHp;
}

void CSuperCar::Set_HpDecrease()
{
	if (m_iHp > 0)
		--m_iHp;
}

void CSuperCar::Set_Pos(D3DXVECTOR3 vPos)
{
	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetPos(vPos);
}

void CSuperCar::Set_OutState(bool bCheck)
{
	m_bIsOut = bCheck;
}

CSuperCar * CSuperCar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CSuperCar* pInstance = new CSuperCar(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}
