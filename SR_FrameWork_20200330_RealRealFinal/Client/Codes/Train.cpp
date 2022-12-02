#include "stdafx.h"
#include "Train.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"
#include "Rail.h"
#include "Seat.h"
#include "Customer.h"

#include "Camera.h"
#include "FpsCamera.h"

CTrain::CTrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_iRailIndex(1), m_fSpeed(20.f),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_vRight({ 1.f, 0.f, 0.f }), m_vUp({ 0.f, 1.f, 0.f }), m_vLook({ 0.f, 0.f, 1.f }),
	m_fCreateTime(0.5f), m_iRailCount(0), m_fMoveValue(0.f)
{
	m_vecRail.reserve(100);
	ZeroMemory(m_bAxisRotate, sizeof(m_bAxisRotate));
}


CTrain::~CTrain()
{
	Release();
}

int CTrain::Update()
{
	if (m_bCreateMode)
	{
		m_fCreateTime += m_pTimeMgr->GetDeltaTime();
		if (m_fCreateTime < 0.1f)
			return NO_EVENT;
		CCameraMgr::GetInstance()->SetCameraIdx(1);
		dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->SetRail(m_pParent, m_vecRail[m_vecRail.size() - 2]);

		Matrix matRotate;
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_W) && m_bAxisRotate[0] == false)
		{

			m_bAxisRotate[0] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetRight(), D3DXToRadian(10.f));
			Vec3 vLook = m_vecRail.back()->GetLook();
			vLook = VEC3::TransformNormal(vLook, matRotate);

			m_vecRail.back()->SetRight(m_vecRail.back()->GetRight(), vLook);
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_S) && m_bAxisRotate[1] == false)
		{
			m_bAxisRotate[1] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetRight(), D3DXToRadian(-10.f));
			Vec3 vLook = m_vecRail.back()->GetLook();
			vLook = VEC3::TransformNormal(vLook, matRotate);

			m_vecRail.back()->SetRight(m_vecRail.back()->GetRight(), vLook);
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_A) && m_bAxisRotate[2] == false)
		{
			m_bAxisRotate[2] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetUp(), D3DXToRadian(-10.f));
			Vec3 vLook = m_vecRail.back()->GetLook();
			vLook = VEC3::TransformNormal(vLook, matRotate);

			m_vecRail.back()->SetLook(vLook, m_vecRail.back()->GetUp());
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_D) && m_bAxisRotate[3] == false)
		{
			m_bAxisRotate[3] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetUp(), D3DXToRadian(10.f));
			Vec3 vLook = m_vecRail.back()->GetLook();
			vLook = VEC3::TransformNormal(vLook, matRotate);

			m_vecRail.back()->SetLook(vLook, m_vecRail.back()->GetUp());
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_Z) && m_bAxisRotate[4] == false)
		{
			m_bAxisRotate[4] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetLook(), D3DXToRadian(10.f));
			Vec3 vUp = m_vecRail.back()->GetUp();
			vUp = VEC3::TransformNormal(vUp, matRotate);
		
			m_vecRail.back()->SetLook(m_vecRail.back()->GetLook(), vUp);
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_X) && m_bAxisRotate[5] == false)
		{
			m_bAxisRotate[5] = true;
			matRotate = MATRIX::RotateAxis(m_vecRail.back()->GetLook(), D3DXToRadian(-10.f));
			Vec3 vUp = m_vecRail.back()->GetUp();
			vUp = VEC3::TransformNormal(vUp, matRotate);
		
			m_vecRail.back()->SetLook(m_vecRail.back()->GetLook(), vUp);
		}
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_SPACE))
		{
			m_vecRail.push_back(CRail::Create(m_pGraphicDev, m_vecRail.back()->GetPos() + m_vecRail.back()->GetLook() * RAIL_ITV,
				m_vecRail.back()->GetLook(), m_vecRail.back()->GetUp(), { 1.f, 1.f, 1.f }, m_pParent));
			ZeroMemory(m_bAxisRotate, sizeof(m_bAxisRotate));
			m_fCreateTime = 0.f;
		}
		if (m_pKeyMgr->KeyDown(ENGINE::KEY_RETURN))
		{
			CCameraMgr::GetInstance()->SetCameraIdx(0);
			m_bCreateMode = false;
			::ShowCursor(TRUE);
		}
		


		return NO_EVENT;
	}
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK_RETURN(pTransform, NO_EVENT);

	ENGINE::CGameObject::Update();
	for (auto Rail : m_vecRail)
		Rail->Update();
	MoveRail();
	SettingSpeed();
	if (m_iRailCount == m_vecRail.size() - 1)
	{
		dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->ClearCustomer(m_pParent->GetPos());
		m_iRailCount = 0;
		return RAIL_END;
		m_fSpeed = 20.f;
	}

	KeyInput();

	Matrix matParentMatrix = m_pParent->GetWorldMatrix();
	Matrix matWorld = pTransform->GetWorldMatrix() * matParentMatrix;
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->KeyInput(this, matParentMatrix);

	return NO_EVENT;
}

void CTrain::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	for (auto Rail : m_vecRail)
		Rail->LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetWorldMatrixForRightUpLookPos(m_vRight, m_vUp, m_vLook, pTransform->GetPos());
}

void CTrain::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	Matrix matParentMatrix = m_pParent->GetWorldMatrix();
	Matrix matWorld = pTransform->GetWorldMatrix() * matParentMatrix;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&matWorld);

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

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_Train"])->SetConvertVtx(m_pConvertVtx);

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer_Train"]->Render();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->Render(matWorld);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (auto Rail : m_vecRail)
		Rail->Render();
}

HRESULT CTrain::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_Train"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);
	
	m_vecRail.push_back(CRail::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, m_vLook, m_vUp, { 1.f, 1.f, 1.f },
		m_pParent));
	m_vecRail.push_back(CRail::Create(m_pGraphicDev, { 0.f, 0.f, 3.f }, m_vLook, m_vUp, { 1.f, 1.f, 1.f },
		m_pParent));
	m_vecRail.push_back(CRail::Create(m_pGraphicDev, { 0.f, 0.f, 6.f }, m_vLook, m_vUp, { 1.f, 1.f, 1.f },
		m_pParent));
	m_vecRail.push_back(CRail::Create(m_pGraphicDev, { 0.f, 0.f, 9.f }, m_vLook, m_vUp, { 1.f, 1.f, 1.f },
		m_pParent));
	m_vecRail.push_back(CRail::Create(m_pGraphicDev, { 0.f, 0.f, 12.f }, m_vLook, m_vUp, { 1.f, 1.f, 1.f },
		m_pParent));
	m_bCreateMode = true;

	m_eRideType = ENGINE::RIDE_ROLLERCOASTER;

	return S_OK;
}

HRESULT CTrain::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Train");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Train Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_Train", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Train");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Train Clone Failed", E_FAIL);
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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Seat
	pComponent = CSeat::Create(m_pGraphicDev, 1);
	dynamic_cast<CSeat*>(pComponent)->SetSeatPos(0, Vec3(0.f, 0.f, 0.f));//(로컬)
	//dynamic_cast<CSeat*>(pComponent)->PushCustomer(0, CCustomer::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }));
	NULL_CHECK_MSG_RETURN(pComponent, L"Seat Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Seat", pComponent));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

void CTrain::Release()
{
	for (auto Rail : m_vecRail)
		ENGINE::SafeDelete(Rail);
	m_vecRail.clear();
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pConvertVtx);
}

void CTrain::KeyInput()
{
	
}

void CTrain::SetLook(Vec3 vLook, Vec3 vUp)
{
	m_vLook = vLook;
	m_vLook = VEC3::Normalize(m_vLook);
	m_vRight = VEC3::Normalize(VEC3::CrossProduct(vUp, m_vLook));
	m_vUp = VEC3::Normalize(VEC3::CrossProduct(m_vLook, m_vRight));
}

void CTrain::SetUp(Vec3 vUp, Vec3 vRight)
{
	m_vUp = vUp;
	m_vUp = VEC3::Normalize(m_vUp);
	m_vLook = VEC3::Normalize(VEC3::CrossProduct(vRight, m_vUp));
	m_vRight = VEC3::Normalize(VEC3::CrossProduct(m_vUp, m_vLook));
}

void CTrain::SetRight(Vec3 vRight, Vec3 vLook)
{
	m_vRight = vRight;
	m_vRight = VEC3::Normalize(m_vRight);
	m_vUp = VEC3::Normalize(VEC3::CrossProduct(vLook, m_vRight));
	m_vLook = VEC3::Normalize(VEC3::CrossProduct(m_vRight, m_vUp));
}

void CTrain::SetRailInfo(ENGINE::ROLLER_INFO tInfo)
{
	for (auto Rail : m_vecRail)
		ENGINE::SafeDelete(Rail);
	m_vecRail.clear();
	m_bCreateMode = false;

	for (int i = 0; i < 1000; ++i)
	{
		if (tInfo.tRailInfo[i].bExist == false)
			return;
		m_vecRail.push_back(CRail::Create(m_pGraphicDev, tInfo.tRailInfo[i].vPos, tInfo.tRailInfo[i].vLook, tInfo.tRailInfo[i].vUp, tInfo.tRailInfo[i].vSize,
			m_pParent));
	}
}

void CTrain::MoveRail()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	float MoveValue = m_fSpeed * m_pTimeMgr->GetDeltaTime();
	//if (MoveValue >= VEC3::Length(m_vecRail[m_iRailIndex]->GetEnd() - pTransform->GetPos()))
	//{
	//	int iIdxIncrease = (int)(MoveValue / RAIL_ITV);
	//	float fMoveAdd = 0.f;
	//	for (int i = 0; i < iIdxIncrease - 1; ++i)
	//	{
	//		fMoveAdd += VEC3::Length(m_vecRail[m_iRailIndex]->GetEnd() - pTransform->GetPos());
	//		pTransform->SetPos(m_vecRail[m_iRailIndex]->GetEnd());
	//		m_iRailIndex = (m_iRailIndex + 1) % m_vecRail.size();
	//		++m_iRailCount;
	//		Vec3 vDir = VEC3::Normalize(m_vecRail[m_iRailIndex]->GetEnd() - pTransform->GetPos());
	//		this->SetLook( vDir, m_vecRail[m_iRailIndex]->GetUp());
	//	}
	//	MoveValue -= fMoveAdd;
	//	pTransform->MoveShift(m_vLook * MoveValue);
	//}
	//else
	{
		m_fMoveValue += MoveValue;

		if (m_fMoveValue > RAIL_ITV)
		{
			m_fMoveValue -= RAIL_ITV;
			pTransform->SetPos(m_vecRail[m_iRailIndex]->GetEnd());
			m_iRailIndex = (m_iRailIndex + 1) % m_vecRail.size();
			++m_iRailCount;
			Vec3 vDir = VEC3::Normalize(m_vecRail[m_iRailIndex]->GetEnd() - pTransform->GetPos());
			this->SetLook(vDir, m_vecRail[m_iRailIndex]->GetUp());
			pTransform->MoveShift(m_vLook * m_fMoveValue);
			m_fMoveValue = 0.f;
		}
		else
		{
			pTransform->MoveShift(m_vLook * m_fSpeed * m_pTimeMgr->GetDeltaTime());

		}
	}
}

void CTrain::SettingSpeed()
{
	if (m_vLook.y < 0)
	{
		m_fSpeed -= m_vLook.y;
		if (m_fSpeed > 50.f)
			m_fSpeed = 50.f;
	}
	else if (m_vLook.y > 0)
	{
		m_fSpeed -= m_vLook.y * 0.5f;
		if (m_fSpeed < 20.f)
			m_fSpeed = 20.f;
	}
}

void CTrain::PushSeat(int iIdx, CCustomer * pCustomer)
{
	pCustomer->RideSetting();
	dynamic_cast<CSeat*>(m_mapComponent[L"Seat"])->PushCustomer(iIdx, pCustomer);
}

CTrain * CTrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize,
	ENGINE::CGameObject* pParent)
{
	CTrain* pInstance = new CTrain(pGraphicDev);
	pInstance->m_pParent = pParent;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0.f);
	return pInstance;
}
