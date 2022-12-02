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
	// 뷰행렬(카메라 역행렬) 생성 함수.
	CPipeLine::MakeViewMatrix(
		&m_matView, /* 반환 값 */
		m_vEye, /* Eye: 현재 카메라의 위치 (월드상 위치) */
		m_vAt, /* At: 현재 카메라가 바라볼 위치 (월드상 위치) */
		D3DXVECTOR3(0.f, 1.f, 0.f) /* Up: 월드 상에서의 Y축 벡터 */);

	// 투영행렬 생성 함수
	CPipeLine::MakeProjectionMatrix(
		&m_matProj, /* 반환 값 */
		D3DXToRadian(m_fFovY), /* 시야각 Y */
		m_fAspect, /* 종횡비 */
		m_fNear, /* Near (근평면) */
		m_fFar /* Far (원평면) */);	

	return NO_EVENT;
}
