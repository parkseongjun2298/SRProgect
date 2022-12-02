#include "stdafx.h"
#include "FpsCamera.h"
#include "Transform.h"
#include "Subject.h"
#include "Customer.h"
#include "Rail.h"
#include "PipeLine.h"
#include "SoundMgr.h"

CFpsCamera::CFpsCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev),
	m_pManagement(ENGINE::CManagement::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fLockAngleY(0.f)
{
}


CFpsCamera::~CFpsCamera()
{
	Release();
}

int CFpsCamera::Update()
{
	if (m_bSet)
	{
		//KeyInput();
		if (m_pParentSet != nullptr)
			this->Update(m_pParentSet->GetWorldMatrix());
		else
			this->Update(MATRIX::Identity());
		return NO_EVENT;
	}
	//::ShowCursor(FALSE);
	else
	{
		KeyInput();

		POINT pt, ptSet;
		::GetCursorPos(&pt);
		ClientToScreen(g_hWnd, &pt);
		POINT ptSetting = { WINCX >> 1, WINCY >> 1 };
		ScreenToClient(g_hWnd, &ptSetting);

		ptSet = { WINCX >> 1,WINCY >> 1 };
		ClientToScreen(g_hWnd, &ptSet);

		POINT ptGap = { pt.x - ptSetting.x, pt.y - ptSetting.y };
		::SetCursorPos(ptSetting.x, ptSetting.y);

		float		fAngle;
		D3DXVECTOR3 vLook;
		D3DXMATRIX	matRotate;

		if (ptGap.x != 0)
		{
			fAngle = ptGap.x * 0.005f;
			D3DXMatrixRotationY(&matRotate, fAngle);
			D3DXVec3TransformNormal(&vLook, &(m_vAt - m_vEye), &matRotate);
			//this->SetLook(vLook, D3DXVECTOR3(0.f, 1.f, 0.f));//m_tInfo.vUp);
			m_vAt = m_vEye + vLook;
		}

		if (ptGap.y != 0)
		{
			fAngle = ptGap.y * 0.005f;
			if (fabs(m_fLockAngleY) < D3DXToRadian(89.f))
			{
				m_fLockAngleY += fAngle;
				if (fabs(m_fLockAngleY) >= D3DXToRadian(89.f))
				{
					fAngle = 0.f;
					if (m_fLockAngleY < 0)
						m_fLockAngleY = -D3DXToRadian(88.9f);
					else
						m_fLockAngleY = D3DXToRadian(88.9f);
				}
				//cout << m_fLockAngleY << endl;
				Vec3 vRight = VEC3::CrossProduct(Vec3(0.f, 1.f, 0.f), VEC3::Normalize(m_vAt - m_vEye), true);
				D3DXMatrixRotationAxis(&matRotate, &vRight, fAngle);
				D3DXVec3TransformNormal(&vLook, &(m_vAt - m_vEye), &matRotate);
				//this->SetLook(vLook, D3DXVECTOR3(0.f, 1.f, 0.f));//m_tInfo.vUp);
				m_vAt = m_vEye + vLook;
			}
		}

		CameraRebound();
		CameraBreath();

		//cout << vLook.x << ", " << vLook.y << ", " << vLook.z << endl;
		ENGINE::CCamera::Update();
		//cout << m_vEye.x << ", " << m_vEye.y << ", " << m_vEye.z << endl;

		m_pSubjectMgr->Notify(L"Camera", DATA_CAMERAPOS, &m_vEye);
		m_pSubjectMgr->Notify(L"Camera", DATA_CAMERALOOK, &VEC3::Normalize(m_vAt - m_vEye));
		m_pSubjectMgr->Notify(L"Camera", DATA_VIEW, &m_matView);
		m_pSubjectMgr->Notify(L"Camera", DATA_PROJECTION, &m_matProj);
	}

	

	return NO_EVENT;
}

int CFpsCamera::Update(Matrix matParentWorld)
{
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_BACKSPACE))
	{
		this->ClearCustomer();
		CSoundMgr::GetInstance()->StopAll();
		CSoundMgr::GetInstance()->PlayBGM(L"놀이동산브금.mp3");
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.2f);
		return NO_EVENT;
	}

	//pTransform->GetWorldMatrix() * matSeat * pRide->GetWorldMatrix()* matParentWorld
	Matrix matCamera = m_pSet->GetWorldMatrix() * m_matSeat * matParentWorld * m_matParentWorld;

	m_vEye = Vec3(matCamera._41, matCamera._42, matCamera._43);
	if(matParentWorld == MATRIX::Identity())
		m_vEye += 2.f * Vec3(matCamera._21, matCamera._22, matCamera._23);
	m_vEye += 2.f * Vec3(matCamera._21, matCamera._22, matCamera._23);

	/*Matrix matParent = matParentWorld * m_matParentWorld;
	m_vEye = m_pSet->GetPos();
	if (m_pParentSet != nullptr)
		m_vEye += m_pParentSet->GetPos();
		
	m_vEye += Vec3(m_matParentWorld._41, m_matParentWorld._42, m_matParentWorld._43);
	m_vEye += Vec3(m_matSeat._41, m_matSeat._42, m_matSeat._43);
	m_vEye += 2.f * Vec3(matParentWorld._21, matParentWorld._22, matParentWorld._23);*/

	//cout << m_vEye.x << ", " << m_vEye.y << ", " << m_vEye.z << endl;

	Vec3 vLook = dynamic_cast<CCustomer*>(m_pSet)->GetDir();
	vLook = VEC3::TransformNormal(vLook, matParentWorld * m_matParentWorld);
	m_vAt = m_vEye + vLook;
	//cout << vLook.x << ", " << vLook.y << ", " << vLook.z << endl;
	//cout << m_vAt.x << ", " << m_vAt.y << ", " << m_vAt.z << endl;
	
	////////////////////////////////////////////////////////////////////////////////
	//Customer Right, Up, Look벡터로 구동
	Matrix matSetWorld = m_pSet->GetWorldMatrix();
	Vec3 vSetUp = { matSetWorld._21, matSetWorld._22, matSetWorld._23 };
	//memcpy(&vSetUp, &matSetWorld[1], sizeof(Vec3));

	m_vUp = VEC3::TransformNormal(vSetUp, matSetWorld * matParentWorld * m_matParentWorld);

	//cout << m_vUp.x << ", " << m_vUp.y << ", " << m_vUp.z << endl;

	// 뷰행렬(카메라 역행렬) 생성 함수.
	ENGINE::CPipeLine::MakeViewMatrix(
		&m_matView, /* 반환 값 */
		m_vEye, /* Eye: 현재 카메라의 위치 (월드상 위치) */
		m_vAt, /* At: 현재 카메라가 바라볼 위치 (월드상 위치) */
		m_vUp /* Up: 월드 상에서의 Y축 벡터 */);

	// 투영행렬 생성 함수
	ENGINE::CPipeLine::MakeProjectionMatrix(
		&m_matProj, /* 반환 값 */
		D3DXToRadian(m_fFovY), /* 시야각 Y */
		m_fAspect, /* 종횡비 */
		m_fNear, /* Near (근평면) */
		m_fFar /* Far (원평면) */);

	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERAPOS, &m_vEye);
	m_pSubjectMgr->Notify(L"Camera", DATA_CAMERALOOK, &VEC3::Normalize(m_vAt - m_vEye));
	m_pSubjectMgr->Notify(L"Camera", DATA_VIEW, &m_matView);
	m_pSubjectMgr->Notify(L"Camera", DATA_PROJECTION, &m_matProj);

	return NO_EVENT;
}

HRESULT CFpsCamera::Initialize()
{
	m_fFovY = 90.f;
	m_fAspect = (float)WINCX / WINCY;
	
	m_fNear = 0.001f;
	m_fFar = 1000.f;

	m_vEye = { 0.f, 2.f, 0.f };
	m_vAt = { 0.f, 2.f, 1.f };

	::SetCursorPos(WINCX >> 1, WINCY >> 1);
	//m_ptOldpt = {WINCX >> 1, WINCY >> 1};

	HRESULT hr = m_pSubjectMgr->AddSubject(L"Camera", ENGINE::CSubject::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Camera Subject Add Failed", E_FAIL);

	CameraBreath(false);

	return S_OK;
}

HRESULT CFpsCamera::AddComponent()
{
	return S_OK;
}

void CFpsCamera::Release()
{
}

void CFpsCamera::KeyInput()
{
	Vec3 vLook = m_vAt - m_vEye;
	//vLook.y = 0.f;
	vLook = VEC3::Normalize(vLook);
	Vec3 vRight = VEC3::CrossProduct({ 0.f, 1.f, 0.f }, vLook, true);
	float fMoveSpeed = 20.f * m_pTimeMgr->GetDeltaTime();
	
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_LEFT))
	{
		m_vEye -= vRight * fMoveSpeed;
		m_vAt -= vRight * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_RIGHT))
	{
		m_vEye += vRight * fMoveSpeed;
		m_vAt += vRight * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_UP))
	{
		m_vEye += vLook * fMoveSpeed;
		m_vAt += vLook * fMoveSpeed;
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_DOWN))
	{
		m_vEye -= vLook * fMoveSpeed;
		m_vAt -= vLook * fMoveSpeed;
	}
	//if (m_pKeyMgr->KeyDown(ENGINE::KEY_LBUTTON))
	//{
	//	CameraRebound(10.f);
	//}
}

void CFpsCamera::CameraRebound()
{
	if (m_bRebound)
	{
		float fRotateValue = -m_fPower * m_pTimeMgr->GetDeltaTime();
		m_fPower /= 1.1f;
		if (m_fPower < 0.1f)
		{
			m_fPower = 0.f;
			m_bRebound = false;
			m_vReboundDir = { 0.f, 0.f, 0.f };
		}
		Matrix matRotate;
		Vec3 vLook;
		if (fabs(m_fLockAngleY) < D3DXToRadian(89.f))
		{
			m_fLockAngleY += fRotateValue;
			if (fabs(m_fLockAngleY) >= D3DXToRadian(89.f))
			{
				fRotateValue = 0.f;
				if (m_fLockAngleY < 0)
					m_fLockAngleY = -D3DXToRadian(88.9f);
				else
					m_fLockAngleY = D3DXToRadian(88.9f);
			}
			//cout << m_fLockAngleY << endl;
			Vec3 vRight = VEC3::CrossProduct(Vec3(0.f, 1.f, 0.f), VEC3::Normalize(m_vAt - m_vEye), true);
			D3DXMatrixRotationAxis(&matRotate, &vRight, fRotateValue);
			D3DXVec3TransformNormal(&vLook, &(m_vAt - m_vEye), &matRotate);
			//this->SetLook(vLook, D3DXVECTOR3(0.f, 1.f, 0.f));//m_tInfo.vUp);
			m_vAt = m_vEye + vLook;
		}
	}
}

void CFpsCamera::CameraBreath()
{
	if (m_bBreath && !m_bRebound)
	{
		m_fBreathTime += m_pTimeMgr->GetDeltaTime();
		m_vAt.y += cosf(D3DX_PI * m_fBreathTime) * 0.0003f;
		m_vEye.y += cosf(D3DX_PI * m_fBreathTime) * 0.00001f;
	}
}

void CFpsCamera::CameraRebound(float fPower)
{
	m_bRebound = true;
	m_fPower += fPower;
	m_vReboundDir = { rand() % 250 - 125.f, float(rand() % 500) + 500.f, 0.f };
	m_vReboundDir = VEC3::Normalize(m_vReboundDir);
}

void CFpsCamera::CameraBreath(bool bSet)
{
	m_bBreath = bSet;
}

void CFpsCamera::SetCustomer(ENGINE::CGameObject * pGameObject, ENGINE::CGameObject* pParent, Matrix matParentWorld, Matrix matSeat)
{
	if (pGameObject == nullptr)
	{
		m_bSet = false;
		return;
	}
	m_pSet = pGameObject;
	if (pParent != nullptr)
	{
		m_pParentSet = pParent;
	}
	m_matParentWorld = matParentWorld;
	m_matSeat = matSeat;
	m_bSet = true;
	m_fLockAngleY = 0.f;
	m_vEye = pGameObject->GetPos() + Vec3(0.f, 2.f, 0.f);
	if(pParent != nullptr)
		m_vEye += pParent->GetPos();
	m_vEye += Vec3(m_matSeat._41, m_matSeat._42, m_matSeat._43);
	Vec3 vLook = dynamic_cast<CCustomer*>(pGameObject)->GetDir();
	if (pParent != nullptr)
		vLook = VEC3::TransformNormal(vLook, pParent->GetWorldMatrix());
	m_vAt = m_vEye + vLook;

	//노래틀기
	if (pParent != nullptr)
	{
		switch (pParent->GetRideType())
		{
		case ENGINE::RIDE_VIKING:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"바이킹소리.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.2f);
			break;
		case ENGINE::RIDE_FERRIS:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"관람차소리.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.1f); 
			break;
		case ENGINE::RIDE_SPINHORSE:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"SpinHorseTheme.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.02f);
			break;
		case ENGINE::RIDE_SPINTEACUP:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"둥글게둥글게.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.02f);
			break;
		case ENGINE::RIDE_GYRODROP:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"자이로드롭소리.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);
			break;
		case ENGINE::RIDE_GYROSWING:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"나비보벳따우.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);
			break;
		case ENGINE::RIDE_ROLLERCOASTER:
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlaySound(L"나비보벳따우.mp3", CSoundMgr::CHANNELID::RIDES);
			CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.05f);
			break;
		default:
			break;
		}
	}
}

void CFpsCamera::ClearCustomer()
{
	m_bSet = false;
	m_pSet = nullptr;
	m_pParentSet = nullptr;
	m_matParentWorld = MATRIX::Identity();
	CCameraMgr::GetInstance()->SetCameraIdx(0);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::RIDES);

	// 나중에 테스트해보고 지울지말지 정해주세염 (위에 놀이기구에서 모든소리를 꺼서 일인칭이 풀릴때 BGM다시 시작되게하는 것)
	CSoundMgr::GetInstance()->PlayBGM(L"놀이동산브금.mp3");
	CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::BGM, 0.2f);
}

void CFpsCamera::SetRail(ENGINE::CGameObject* pParent, ENGINE::CGameObject * pGameObject)
{
	if (m_pSet == pGameObject)
		return;
	m_pSet = pGameObject;
	m_fLockAngleY = 0.f;
	m_vEye = pGameObject->GetPos() + Vec3(0.f, 2.f, 0.f) + pParent->GetPos();
	m_vAt = m_vEye + dynamic_cast<CRail*>(pGameObject)->GetLook();
}

CFpsCamera* CFpsCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CFpsCamera* pInstance = new CFpsCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
