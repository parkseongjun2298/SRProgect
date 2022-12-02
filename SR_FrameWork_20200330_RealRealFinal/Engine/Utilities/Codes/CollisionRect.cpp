#include "CollisionRect.h"

USING(ENGINE)

CCollisionRect::CCollisionRect()
{
}


CCollisionRect::~CCollisionRect()
{
}

void CCollisionRect::LateUpdate()
{
}

CCollisionRect * CCollisionRect::Create(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize)
{
	CCollisionRect* pInstance = new CCollisionRect;
	pInstance->m_rcCollision.left = vPos.x - vSize.x * 0.5f;
	pInstance->m_rcCollision.top = vPos.z - vSize.z * 0.5f;
	pInstance->m_rcCollision.right = vPos.x - vSize.x * 0.5f;
	pInstance->m_rcCollision.bottom = vPos.z - vSize.z * 0.5f;
	return pInstance;
}