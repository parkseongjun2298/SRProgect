#include "stdafx.h"
#include "DynamicCamera.h"
#include "Transform.h"
#include "Subject.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev),
	m_pManagement(ENGINE::CManagement::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fTargetDist(0.f)
{
}


CDynamicCamera::~CDynamicCamera()
{
	Release();
}

int CDynamicCamera::Update()
{
	::ShowCursor(TRUE);
	KeyInput();
	ENGINE::CCamera::Update();

	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERAPOS, &m_vEye);
	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERALOOK, &VEC3::Normalize(m_vAt - m_vEye));
	m_pSubjectMgr->Notify(L"Camera", DATA_VIEW, &m_matView);
	m_pSubjectMgr->Notify(L"Camera", DATA_PROJECTION, &m_matProj);

	return NO_EVENT;
}

HRESULT CDynamicCamera::Initialize()
{
	m_fFovY = 90.f;
	m_fAspect = (float)WINCX / WINCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;

	//m_vEye = { 0.f, 20.f, 0.f };
	//m_vAt = { 0.f, 0.f, 10.f };
	m_vLook = m_vAt - m_vEye;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_fTargetDist = D3DXVec3Length(&(m_vEye + m_vAt));


	HRESULT hr = m_pSubjectMgr->AddSubject(L"Camera", ENGINE::CSubject::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Camera Subject Add Failed", E_FAIL);

	return S_OK;
}

HRESULT CDynamicCamera::AddComponent()
{
	return S_OK;
}

void CDynamicCamera::Release()
{
}

void CDynamicCamera::KeyInput()
{
	float fMoveSpeed = 30.f * m_pTimeMgr->GetDeltaTime();

	D3DXVECTOR3 vRight, vLook = m_vAt - m_vEye;
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_W))
	{
		m_vAt += vLook * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_S))
	{
		m_vAt -= vLook * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_A))
	{
		m_vAt -= vRight * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_D))
	{
		m_vAt += vRight * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_ADD))
		m_fTargetDist -= fMoveSpeed;
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_MIN))
		m_fTargetDist += fMoveSpeed;

	//CameraLock
	if (m_vAt.z > (TERRAIN_CNTZ - 1) * TERRAIN_ITV - 10 * TERRAIN_ITV)
		m_vAt.z = (TERRAIN_CNTZ - 1) * TERRAIN_ITV - 10 * TERRAIN_ITV;
	if (m_vAt.z < 3.4 * TERRAIN_ITV)
		m_vAt.z = 3.4 * TERRAIN_ITV;
	if (m_vAt.x >(TERRAIN_CNTX - 1) * TERRAIN_ITV - 6 * TERRAIN_ITV)
		m_vAt.x = (TERRAIN_CNTX - 1) * TERRAIN_ITV - 6 * TERRAIN_ITV;
	if (m_vAt.x < 6 * TERRAIN_ITV)
		m_vAt.x = 6 * TERRAIN_ITV;


	m_vEye = m_vAt - m_vLook * m_fTargetDist;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vEye /*= { 0.f, 20.f, 0.f }*/, D3DXVECTOR3 vAt /*= { 0.f, 0.f, 10.f }*/)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);
	pInstance->m_vEye = vEye;
	pInstance->m_vAt = vAt;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
