#include "stdafx.h"
#include "TeaCup.h"
#include "Transform.h"
#include "CubeTex.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "SpinTeaCup.h"
#include "Seat.h"
#include "Customer.h"
#include "TeaCupPlane.h"

CTeaCup::CTeaCup(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr),
	m_fAngle(0.f)
{
}


CTeaCup::~CTeaCup()
{
	Release();
}

int CTeaCup::Update()
{
	ENGINE::CGameObject::Update();
	SpinCup();
	Matrix matParentMatrix = MATRIX::Identity();
	//Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CTeaCup::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetParentWorldMatrix(m_pParent->GetWorldMatrix() * dynamic_cast<CTeaCupPlane*>(m_pParent)->GetParent()->GetWorldMatrix());
}

void CTeaCup::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	ENGINE::CTransform* pParentTrans = dynamic_cast<ENGINE::CTransform*>(m_pParent->GetComponent(L"Transform"));
	NULL_CHECK(pTransform);

	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix();// *pParentTrans->GetWorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(matWorld * pObserver->GetViewMatrix()));

	//	// 투영스페이스 범위 밖에 정점들을 투영변환에서 제외시키기
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());

	//}
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(matWorld);
}

HRESULT CTeaCup::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	m_eRideType = ENGINE::RIDE_SPINTEACUP;

	return S_OK;
}

HRESULT CTeaCup::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_TeaCup");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_TeaCup Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_TeaCup");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_TeaCup Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"TeaCup Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"TeaCup Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	// Seat
	pComponent = CSeat::Create(m_pGraphicDev, 1);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, 0.f, 0.f)); // 로컬
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));

	return S_OK;
}

void CTeaCup::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CTeaCup* CTeaCup::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, ENGINE::CGameObject* pParent)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CTeaCup* pInstance = new CTeaCup(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pParent = pParent;
	pInstance->m_fAngle = (float)(rand() % 90);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTeaCup::SpinCup()
{
	if (m_bIsLeftSpin)
	{
		m_fAngle -= 20.f;
		m_bIsLeftSpin = false;
	}
	if (m_bIsRightSpin)
	{
		m_fAngle += 20.f;
		m_bIsRightSpin = false;
	}

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	float fAngleSpeed = m_fAngle * m_pTimeMgr->GetDeltaTime();

	pTransform->MoveAngle(ENGINE::ANGLE_Y, fAngleSpeed);

}

void CTeaCup::PushSeat(CCustomer * pCustomer)
{
	pCustomer->RideSetting();
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(0, pCustomer);
}

void CTeaCup::ClearSeat(D3DXVECTOR3 vPos)
{
	vPos.y = 0.f;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(vPos);
}
