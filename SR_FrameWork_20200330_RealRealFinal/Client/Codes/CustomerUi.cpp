#include "stdafx.h"
#include "CustomerUi.h"

#include "RcTex.h"
#include "Texture.h"

#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

#include "Customer.h"
#include "CustomerGage.h"

CCustomerUi::CCustomerUi(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr),
	m_iHogamdoGage(0)
{
}


CCustomerUi::~CCustomerUi()
{
	Release();
}

int CCustomerUi::Update()
{
	ENGINE::CGameObject::Update();
	Update_CustomerPos();

	m_pCustomerGage->Update();

	return NO_EVENT;
}

void CCustomerUi::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	IsBillboarding();

	m_pCustomerGage->LateUpdate();
}

void CCustomerUi::Render()
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

	m_mapComponent[L"Texture"]->Render((WORD)m_iHogamdoGage);
	m_mapComponent[L"Buffer"]->Render();

	//m_pCustomerGage->Render();
}

HRESULT CCustomerUi::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	// CustomerGage
	m_pCustomerGage = CCustomerGage::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
	NULL_CHECK_RETURN(m_pCustomerGage, E_FAIL);

	return S_OK;
}

HRESULT CCustomerUi::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CustomerUi");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_CustomerUi Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_CustomerUi");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_CustomerUi Clone Failed", E_FAIL);
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

void CCustomerUi::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	ENGINE::SafeDelete(m_pCustomerGage);
}

void CCustomerUi::Update_CustomerPos()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	// 매 프레임마다 자신의 객체가 될 손님의 좌표를 받아서, 좌표를 이동시킴.
	pTransform->SetPos(m_pCustomer->GetPos() + D3DXVECTOR3(0.f, 2.f, 0.f));
	// 이동시킨 좌표를 게이지에 넣어줌.
	D3DXVECTOR3 vPos = m_pCustomer->GetPos() + D3DXVECTOR3(0.f, 2.f, 0.f);
	m_pCustomerGage->Set_Pos(pTransform->GetPos());

}

void CCustomerUi::IsBillboarding()
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
}

void CCustomerUi::Set_HogamdoGage(int iGage)
{
	if (m_iHogamdoGage >= 10)
		m_iHogamdoGage = 10;
	if (m_iHogamdoGage <= 0)
		m_iHogamdoGage = 0;
	m_iHogamdoGage += iGage;
}

CCustomerUi * CCustomerUi::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, CCustomer* pCustomer, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	NULL_CHECK_RETURN(pCustomer, nullptr);

	CCustomerUi* pInstance = new CCustomerUi(pGraphicDev);
	pInstance->m_pCustomer = pCustomer;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngle);
	return pInstance;
}
