#include "stdafx.h"
#include "AutoCar.h"
#include "CubeTex.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CameraObserver.h"

CAutoCar::CAutoCar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSuperCar(pGraphicDev),
	m_vTargetDir(0.f, 0.f, 0.f), m_fMoveTime(0.f),
	m_fMoveAngle(0.f), m_fDotAngle(0.f)
{
}


CAutoCar::~CAutoCar()
{
	Release();
}

int CAutoCar::Update()
{
	CSuperCar::Update();
	AutoMove2();

	return NO_EVENT;
}

void CAutoCar::LateUpdate()
{
	CSuperCar::LateUpdate();
}

void CAutoCar::Render()
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

		// ���������̽� ���� �ۿ� �������� ������ȯ���� ���ܽ�Ű��
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());

	}
	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(m_iTextureIndex);	// m_iTextureIndex : ���߿� �ڵ����� ������� �ٸ��� ����ϱ�����.
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CAutoCar::Initialize()
{
	CSuperCar::Initialize();
	//FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	////m_fSpeed = 10.f;

	//ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	//NULL_CHECK_RETURN(pBuffer, E_FAIL);

	//m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	//NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	//m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	//memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);
	D3DXMATRIX matRot = MATRIX::RotateY(D3DXToRadian(m_fMoveAngle));
	m_vLook = VEC3::TransformNormal(m_vLook, matRot);
	m_vRight = VEC3::CrossProduct({ 0.f, 1.f, 0.f }, m_vLook);
	m_vUp = { 0.f, 1.f ,0.f };

	return S_OK;
}

HRESULT CAutoCar::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;


	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_AutoCar");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_AutoCar Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_AutoCar");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_AutoCar Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_vRight = { 1.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };
	m_vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"AutoCar Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"AutoCar Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));
	return S_OK;
}

void CAutoCar::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Transform"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CAutoCar::AutoMove()
{
	if (m_pTarget == nullptr)
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);


	D3DXVECTOR3 vAutoPos = pTransform->GetPos();

	// 1. ����ī(Player)�� ��ġ�� �޾ƿ´�.
	D3DXVECTOR3 vTargetPos = dynamic_cast<ENGINE::CTransform*>(m_pTarget->GetComponent(L"Transform"))->GetPos();
	D3DXVECTOR3 vTargetLook = m_pTarget->Get_Look();

	//cout << vAutoPos.x << " , " << vAutoPos.y << " , " << vAutoPos.z << endl;

	// 2. �÷��̾�� ���ϴ� ������ ����.
	D3DXVECTOR3 vTargetDir = vTargetPos - pTransform->GetPos();
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);

	// 3. ����ī(Ai)�� Look�� �÷������� ������ �����Ͽ� ������ ����.
	float fDotAngle = VEC3::DotProduct(m_vLook, vTargetDir);	// cos�� -1 ~ 1 ���� ����,
	fDotAngle = acosf(fDotAngle);								// acosf()�� ����� ��μ� ������ �ٲ�.

	//// Test
	//D3DXVECTOR3 vCross = VEC3::CrossProduct(vTargetLook, m_vLook);
	//if (vCross.y < 0)
	//	fDotAngle = -fDotAngle;
	////cout << vCross.x << " , " << vCross.y << " , " << vCross.z << endl;

	// 4. ȸ������� ���� ��, vLook, vRight, vUp ������.
	D3DXMATRIX matRot = MATRIX::RotateY(D3DXToRadian(fDotAngle));
	m_vLook = VEC3::TransformNormal(m_vLook, matRot);
	m_vRight = VEC3::CrossProduct(D3DXVECTOR3(0.f, 1.f, 0.f), m_vLook);
	m_vUp = { 0.f, 1.f, 0.f };

	// 5. �����ǵ� ������ �������� �����δ�.
	pTransform->MoveShift(m_vLook * 15.f * m_pTimeMgr->GetDeltaTime());

}

void CAutoCar::AutoMove2()
{
	if (!m_bIsOut && !m_bIsAccel || m_pTarget == nullptr)
		return;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vAutoPos = pTransform->GetPos();

	// 1. Ÿ��(�� �ٸ� Ai�� ���� �ְ�, �÷��̾� �� ���� �ִ�.)�� ��ġ�� �޾ƿ´�.
	D3DXVECTOR3 vTargetPos = dynamic_cast<ENGINE::CTransform*>(m_pTarget->GetComponent(L"Transform"))->GetPos();
	D3DXVECTOR3 vTargetLook = m_pTarget->Get_Look();

	// 2. Ÿ������ ���� ���� ���Ѵ�. (��, �����϶� �� �ѹ� ������ �ް� ���ڴ�� ������ ��.)
	m_vTargetDir = vTargetPos - vAutoPos;
	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);


	// 3. Ÿ������ ���ư� ����� Look�� �����Ѵ�.
	D3DXVECTOR3 vCross;
	float fMoveAngle = 0.f;
	D3DXVec3Cross(&vCross, &m_vLook, &m_vTargetDir);
	if (vCross.y <= 0.1)
	{
		fMoveAngle = 90.f * m_pTimeMgr->GetDeltaTime();
	}
	else if (vCross.y >= 0.1)
	{
		fMoveAngle = -90.f * m_pTimeMgr->GetDeltaTime();
	}

	// 4. ���� ������ ȸ����� ���� vRight, vUp, vLook ������.
	D3DXMATRIX matRot = MATRIX::RotateY(D3DXToRadian(-fMoveAngle));
	m_vLook = VEC3::TransformNormal(m_vLook, matRot);
	m_vRight = VEC3::CrossProduct({ 0.f, 1.f, 0.f }, m_vLook);
	m_vUp = { 0.f, 1.f ,0.f };

	// 5. �����Ѵ�.
	pTransform->MoveShift(m_vLook * m_fSpeed * m_pTimeMgr->GetDeltaTime());
	m_fMoveTime += m_pTimeMgr->GetDeltaTime();

}

CAutoCar * CAutoCar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, int m_iTexIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CAutoCar* pInstance = new CAutoCar(pGraphicDev);
	pInstance->m_fMoveAngle = fAngleY;
	pInstance->m_iTextureIndex = m_iTexIndex;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}
