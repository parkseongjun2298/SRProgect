#include "stdafx.h"
#include "SpinHorseHorse.h"
#include "CameraObserver.h"
#include "SpinHorse.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CubeCol.h"
#include "Seat.h"
#include "Customer.h"

CSpinHorseHorse::CSpinHorseHorse(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pOriginVtx(nullptr), m_pConvertVtx(nullptr), m_pParent(nullptr)
{
}


CSpinHorseHorse::~CSpinHorseHorse()
{
	Release();
}

int CSpinHorseHorse::Update()
{
	ENGINE::CGameObject::Update();
	HorseMove();

	Matrix matParentMatrix = MATRIX::Identity();
	//Matrix matWorld = pTransform->GetWorldMatrix() *matParentMatrix;

	//cout << matWorld._41 << ", " << matWorld._42 << ", " << matWorld._43 << endl;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CSpinHorseHorse::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);
	pTransform->SetParentWorldMatrix(m_pParent->GetWorldMatrix());
}

void CSpinHorseHorse::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	D3DXMATRIX matWorld;
	matWorld = pTransform->GetWorldMatrix() /**m_pParent->GetWorldMatrix()*/;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 8; ++i)
	//{
	//	// 로컬 -> 뷰 스페이스
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(matWorld * pObserver->GetViewMatrix()));
	//	
	//	// 투영 변환전 예외 처리
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	// 뷰 -> 투영 스페이스
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Buffer"]->Render();

	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(matWorld);
}

HRESULT CSpinHorseHorse::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_fSpeed = 1.f;

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_COL*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_COL[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_COL) * 8);

	m_eRideType = ENGINE::RIDE_SPINHORSE;
	return S_OK;
}

void CSpinHorseHorse::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

HRESULT CSpinHorseHorse::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_SpinHorseHorse");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_SpinHorseHorse Clone Add Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));


	//// Texture
	//pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_SpinHorseHorse");
	//NULL_CHECK_MSG_RETURN(pComponent, L"Texture_SpinHorseHorse Clone Add Failed", E_FAIL);
	//m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	m_tInfo.vLook = { 0.f, 0.f, 1.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(m_tInfo.vLook, m_tInfo.vPos, m_tInfo.vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorseHorse Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"SpinHorseHorse Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	m_pSubjectMgr->Subscribe(L"Camera", pObserver);

	// Seat
	pComponent = CSeat::Create(m_pGraphicDev, 1);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, 0.f, 1.f)); // 로컬
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));
	
	return S_OK;
}

CSpinHorseHorse * CSpinHorseHorse::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, CSpinHorse * pParent, float fAngle)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CSpinHorseHorse* pInstance = new CSpinHorseHorse(pGraphicDev);
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pParent = pParent;
	pInstance->m_fAngle = fAngle;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSpinHorseHorse::HorseMove()
{
	// 0보다 작을 시
	if (m_tInfo.vPos.y < 1.f)
	{
		m_fSpeed = -m_fSpeed;	// 스피드를 역스피드 ex) 10 -> -10
		m_tInfo.vPos.y = 1.f;
	}
	else if (m_tInfo.vPos.y > 4.f)
	{
		m_fSpeed = -m_fSpeed;
		m_tInfo.vPos.y = 4.f;
	}
	m_tInfo.vPos.y += m_fSpeed * m_pTimeMgr->GetDeltaTime();

	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetAngle(ENGINE::ANGLE_Y, m_fAngle);

	dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"])->SetPos(m_tInfo.vPos);
}

void CSpinHorseHorse::PushSeat(int iIdx, CCustomer * pCustomer)
{
	pCustomer->RideSetting(Vec3(0.f, 0.f, -1.f));
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(iIdx, pCustomer);
}

void CSpinHorseHorse::ClearSeat(D3DXVECTOR3 vPos)
{
	vPos.y = 0.f;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(vPos);
}

