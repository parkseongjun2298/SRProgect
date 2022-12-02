#include "Transform.h"
#include "PipeLine.h"

USING(ENGINE)

CTransform::CTransform()
{	
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_vPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vDir, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vLook, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vSize, sizeof(D3DXVECTOR3));
	ZeroMemory(m_fAngle, sizeof(m_fAngle));
}


CTransform::~CTransform()
{
}

const D3DXVECTOR3& CTransform::GetPos() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vPos;
}

const D3DXVECTOR3& CTransform::GetDir() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vDir;
}

const D3DXMATRIX& CTransform::GetWorldMatrix() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matWorld;
}

void CTransform::SetPos(const D3DXVECTOR3& vPos)
{
	m_vPos = vPos;
}

void CTransform::SetWorldMatrix(const D3DXMATRIX& matWorld)
{
	m_matWorld = matWorld;
}

void CTransform::SetWorldMatrixForRightUpLookPos(const D3DXVECTOR3 & vRight, const D3DXVECTOR3 & vUp, 
	const D3DXVECTOR3 & vLook, const D3DXVECTOR3 & vPos)
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, m_vSize.x, m_vSize.y, m_vSize.z);
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = vRight.x;	m_matWorld._12 = vRight.y;	m_matWorld._13 = vRight.z;
	m_matWorld._21 = vUp.x;		m_matWorld._22 = vUp.y;		m_matWorld._23 = vUp.z;
	m_matWorld._31 = vLook.x;	m_matWorld._32 = vLook.y;	m_matWorld._33 = vLook.z;
	m_matWorld._41 = vPos.x;	m_matWorld._42 = vPos.y;	m_matWorld._43 = vPos.z;

	m_matWorld = matScale * m_matWorld;
}

void CTransform::SetTransformForUi(D3DXVECTOR3 vSize, D3DXVECTOR3 vParentPos)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = vSize.x;
	m_matWorld._22 = vSize.y;
	m_matWorld._33 = vSize.z;

	m_matWorld._41 = (m_vPos.x + vParentPos.x) / (WINCX * 0.5f);
	m_matWorld._42 = (m_vPos.y + vParentPos.y) / (WINCY * 0.5f);
	m_matWorld._43 = m_vPos.z;
}

void CTransform::LateUpdate()
{
	CPipeLine::MakeWorldMatrix(&m_matWorld, m_vSize, m_fAngle, m_vPos);

	D3DXVec3TransformNormal(&m_vDir, &m_vLook, &m_matWorld);
	D3DXVec3Normalize(&m_vDir, &m_vDir);	
}

void CTransform::MovePos(float fMoveSpeed)
{
	m_vPos += m_vDir * fMoveSpeed;
}

void CTransform::MoveAngle(ANGLE_TYPE eAxis, float fMoveSpeed)
{
	m_fAngle[eAxis] += fMoveSpeed;
}

CTransform* CTransform::Create(
	const D3DXVECTOR3& vLook, 
	const D3DXVECTOR3& vPos, 
	const D3DXVECTOR3& vSize)
{
	CTransform* pInstance = new CTransform;

	pInstance->m_vLook = vLook;
	pInstance->m_vPos = vPos;
	pInstance->m_vSize = vSize;

	return pInstance;
}
