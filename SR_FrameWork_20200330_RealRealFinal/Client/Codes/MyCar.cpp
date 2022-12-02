#include "stdafx.h"
#include "MyCar.h"
#include "CubeTex.h"
#include "Transform.h"
#include "PipeLine.h"
#include "CameraObserver.h"

CMyCar::CMyCar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSuperCar(pGraphicDev)
{
}


CMyCar::~CMyCar()
{
	Release();
}

int CMyCar::Update()
{
	CSuperCar::Update();
	KeyInput();

	return NO_EVENT;
}

void CMyCar::LateUpdate()
{
	CSuperCar::LateUpdate();
}

void CMyCar::Render()
{
	CSuperCar::Render();
}

HRESULT CMyCar::Initialize()
{
	CSuperCar::Initialize();
	m_iHp = 3;
	//m_fSpeed = 20;

	return S_OK;
}

HRESULT CMyCar::AddComponent()
{
	CSuperCar::AddComponent();
	return S_OK;
}

void CMyCar::Release()
{
	CSuperCar::Release();
}

void CMyCar::KeyInput()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	float	fMoveSpeed = m_fSpeed * m_pTimeMgr->GetDeltaTime();	// �ӵ�
	float	fMoveAngle = 0.f;	// ȸ����
	m_bIsAccel = false;			// �ӵ�����Ű�� �������ʾ��� �� ���ҽ�Ű��.

	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);

	// ����, ����
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_UP))
	{
		m_bIsAccel = true;
		//pTransform->MoveShift(m_vLook * fMoveSpeed);
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_DOWN))
	{
		//pTransform->MoveShift(m_vLook * -fMoveSpeed);
	}

	pTransform->MoveShift(m_vLook * fMoveSpeed);

	// ��ȸ��, ��ȸ��
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_LEFT))
	{
		fMoveAngle += 180.f * m_pTimeMgr->GetDeltaTime();
	}
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_RIGHT))
	{
		fMoveAngle -= 180.f * m_pTimeMgr->GetDeltaTime();
	}

	matRot = MATRIX::RotateY(D3DXToRadian(-fMoveAngle));
	m_vLook = VEC3::TransformNormal(m_vLook, matRot);	// Look ������
	m_vRight = VEC3::CrossProduct(D3DXVECTOR3(0.f, 1.f, 0.f), m_vLook);	// ����
	m_vUp = { 0.f, 1.f, 0.f };

	//// �극��ũ
	//if (m_pKeyMgr->KeyPressing(ENGINE::KEY_SPACE))
	//{

	//}
}

CMyCar * CMyCar::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CMyCar* pInstance = new CMyCar(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);

	return pInstance;
}
