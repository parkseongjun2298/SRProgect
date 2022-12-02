#include "stdafx.h"
#include "CustomerGage.h"
#include "RcTex.h"
#include "Texture.h"

#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

CCustomerGage::CCustomerGage(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr)
{
}


CCustomerGage::~CCustomerGage()
{
	Release();
}

int CCustomerGage::Update()
{
	ENGINE::CGameObject::Update();
	return NO_EVENT;
}

void CCustomerGage::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	IsBillboarding();
}

void CCustomerGage::Render()
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
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());
	//
	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
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

	m_mapComponent[L"Texture"]->Render();
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CCustomerGage::Initialize()
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

HRESULT CCustomerGage::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CustomerGage");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_CustomerGage Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_CustomerGage");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_CustomerGage Clone Failed", E_FAIL);
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

	return S_OK;
}

void CCustomerGage::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CCustomerGage::Set_Size(D3DXVECTOR3 vSize)
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	pTransform->SetSize(vSize);
}

void CCustomerGage::Set_Pos(D3DXVECTOR3 vPos)
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	m_vUiPos = vPos - D3DXVECTOR3(0.f, 0.f, 0.1f);
	pTransform->SetPos(vPos - D3DXVECTOR3(0.f, 0.f, 0.1f));
}

void CCustomerGage::IsBillboarding()
{
	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

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

	if (pTransform->GetWorldMatrix()._33 < 0)
	{
		pTransform->SetPos(m_vUiPos + D3DXVECTOR3(0.f, 0.f, 0.2f));
		pTransform->LateUpdate();
		pTransform->SetWorldMatrix(matBillY * pTransform->GetWorldMatrix());
	}

	//// 카메라가 회전한 반대로 자전을 먹이면 빌보드 완성.
	//// 빌보드: 항상 카메라 정면을 바라보게 만드는 기법.
	//// ex) 풀, 나무, 나뭇잎, 이펙트 등.

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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

CCustomerGage * CCustomerGage::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CCustomerGage* pInstance = new CCustomerGage(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}
