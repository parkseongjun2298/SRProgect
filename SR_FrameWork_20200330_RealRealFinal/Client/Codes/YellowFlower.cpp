#include "stdafx.h"
#include "YellowFlower.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "PipeLine.h"

CYellowFlower::CYellowFlower(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),	
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr)
{
	
}

CYellowFlower::~CYellowFlower()
{
	Release();
}

int CYellowFlower::Update()
{
	ENGINE::CGameObject::Update();

	return NO_EVENT;
}

void CYellowFlower::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	IsBillboarding();
}

void CYellowFlower::Render()
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
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);


	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 알파 블렌딩
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_mapComponent[L"Texture"]->Render(0);
	//m_mapComponent[L"Buffer"]->Render();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	/*Z Read(Z Test)
		- A가 그려질 때 기존에 그려진 B의 Z와 비교를 하여
		- A의 Z 값이 B의 Z 값보다 작을 때 A를 그린다.

		SetRenderState(D3DRS_ZENABLE, TRUE);

	Z Write
		- 자신의 Z 값을 Z 버퍼에 기록한다.

		SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	3D 상에서 알파블렌딩 처리를 하기 위해서는

		Step1.
		불투명한 대상들 먼저 그린다. (이 대상들 끼리는 그리기 순서 상관 X)

		Step2.
		투명하거나 반투명한 대상을 그린다. (이 대상들 끼리는 정렬이 되어있어야 돼)
		* 정렬 순서 : 카메라로부터 멀리있는 대상부터 먼저 그려나간다.*/
}

HRESULT CYellowFlower::Initialize()
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

HRESULT CYellowFlower::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_YellowFlower");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_YellowFlower Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_YellowFlower");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_YellowFlower Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	
	
	pComponent = ENGINE::CTransform::Create(vLook, m_vPos, m_vSize);
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

	return S_OK;
}

void CYellowFlower::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}



void CYellowFlower::IsBillboarding()
{
	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	// 카메라가 회전한 반대로 자전을 먹이면 빌보드 완성.
	// 빌보드: 항상 카메라 정면을 바라보게 만드는 기법.
	// ex) 풀, 나무, 나뭇잎, 이펙트 등.

	//// 전체 축 빌보드
	//D3DXMATRIX matBill;
	//D3DXMatrixIdentity(&matBill);

	//// 포지션을 제외한 모든 원소를 얻어옴.
	//memcpy(&matBill.m[0][0], &pObserver->GetViewMatrix().m[0][0], sizeof(D3DXVECTOR3));
	//memcpy(&matBill.m[1][0], &pObserver->GetViewMatrix().m[1][0], sizeof(D3DXVECTOR3));
	//memcpy(&matBill.m[2][0], &pObserver->GetViewMatrix().m[2][0], sizeof(D3DXVECTOR3));

	//D3DXMatrixInverse(&matBill, nullptr, &matBill);

	//ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	//NULL_CHECK(pTransform);

	//D3DXMATRIX matWorld = pTransform->GetWorldMatrix();

	//// 스 * 자 * 이 * 공 * 부
	//pTransform->SetWorldMatrix(matBill * matWorld);

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Y축 빌보드
	D3DXMATRIX matBillY;
	D3DXMatrixIdentity(&matBillY);

	// 뷰행렬의 Y축 회전 정보 얻어옴.
	matBillY._11 = pObserver->GetViewMatrix()._11;
	matBillY._13 = pObserver->GetViewMatrix()._13;
	matBillY._31 = pObserver->GetViewMatrix()._31;
	matBillY._33 = pObserver->GetViewMatrix()._33;	

	D3DXMatrixInverse(&matBillY, nullptr, &matBillY);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXMATRIX matWorld = pTransform->GetWorldMatrix();

	// 스 * 자 * 이 * 공 * 부
	pTransform->SetWorldMatrix(matBillY * matWorld);
}

CYellowFlower* CYellowFlower::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CYellowFlower* pInstance = new CYellowFlower(pGraphicDev);
	pInstance->m_vPos = vPos;
	pInstance->m_vSize = vSize;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
