#include "stdafx.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "Subject.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev),
	m_pManagement(ENGINE::CManagement::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fTargetDist(0.f),
	m_fAngle(30.f)
{
}


CStaticCamera::~CStaticCamera()
{
	Release();
}

int CStaticCamera::Update()
{
	KeyInput();
	
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(
		m_pManagement->GetComponent(ENGINE::LAYER_OBJECT, L"Player", L"Transform"));
	NULL_CHECK_RETURN(pTransform, NO_EVENT);

	// Eye
	m_vEye = pTransform->GetDir() * -1.f;
	m_vEye *= m_fTargetDist;

	// 플레이어의 Right 벡터 얻어오기.
	D3DXVECTOR3 vRight = {};
	memcpy(&vRight, &pTransform->GetWorldMatrix().m[0][0], sizeof(D3DXVECTOR3));

	// 임의의 축 회전
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fAngle));
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRot);

	m_vEye += pTransform->GetPos();

	// 바라볼 대상의 위치
	m_vAt = pTransform->GetPos();

	ENGINE::CCamera::Update();

	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERAPOS, &m_vEye);
	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERALOOK, &VEC3::Normalize(m_vAt - m_vEye));
	m_pSubjectMgr->Notify(L"Camera", DATA_VIEW, &m_matView);
	m_pSubjectMgr->Notify(L"Camera", DATA_PROJECTION, &m_matProj);

	return NO_EVENT;
}

HRESULT CStaticCamera::Initialize()
{
	m_fFovY = 90.f;
	m_fAspect = (float)WINCX / WINCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;

	m_fTargetDist = 10.f;

	HRESULT hr = m_pSubjectMgr->AddSubject(L"Camera", ENGINE::CSubject::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Camera Subject Add Failed", E_FAIL);

	return S_OK;
}

HRESULT CStaticCamera::AddComponent()
{
	return S_OK;
}

void CStaticCamera::Release()
{
}

void CStaticCamera::KeyInput()
{
	float fMoveSpeed = 3.f * m_pTimeMgr->GetDeltaTime();
	float fAngleSpeed = 45.f * m_pTimeMgr->GetDeltaTime();

	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_ADD))
		m_fTargetDist -= fMoveSpeed;
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_MIN))
		m_fTargetDist += fMoveSpeed;
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_NUMPAD8))
		m_fAngle += fAngleSpeed;
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_NUMPAD2))
		m_fAngle -= fAngleSpeed;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CStaticCamera* pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
