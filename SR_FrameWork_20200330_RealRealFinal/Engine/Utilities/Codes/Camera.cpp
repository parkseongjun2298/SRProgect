#include "Camera.h"
#include "PipeLine.h"

USING(ENGINE)

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_fFovY(0.f),
	m_fAspect(0.f),
	m_fNear(0.f),
	m_fFar(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	ZeroMemory(&m_vEye, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vAt, sizeof(D3DXVECTOR3));
}


CCamera::~CCamera()
{
}

int CCamera::Update()
{
	// �����(ī�޶� �����) ���� �Լ�.
	CPipeLine::MakeViewMatrix(
		&m_matView, /* ��ȯ �� */
		m_vEye, /* Eye: ���� ī�޶��� ��ġ (����� ��ġ) */
		m_vAt, /* At: ���� ī�޶� �ٶ� ��ġ (����� ��ġ) */
		D3DXVECTOR3(0.f, 1.f, 0.f) /* Up: ���� �󿡼��� Y�� ���� */);

	// ������� ���� �Լ�
	CPipeLine::MakeProjectionMatrix(
		&m_matProj, /* ��ȯ �� */
		D3DXToRadian(m_fFovY), /* �þ߰� Y */
		m_fAspect, /* ��Ⱦ�� */
		m_fNear, /* Near (�����) */
		m_fFar /* Far (�����) */);	

	return NO_EVENT;
}
