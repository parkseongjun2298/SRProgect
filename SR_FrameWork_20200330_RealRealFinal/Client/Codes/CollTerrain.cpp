#include "stdafx.h"
#include "CollTerrain.h"
#include "VIBuffer.h"
#include "GameObject.h"
#include "Transform.h"

CCollTerrain::CCollTerrain(ENGINE::CGameObject* pObject)
	: m_pObject(pObject),
	m_pManagement(ENGINE::CManagement::GetInstance())
{
}


CCollTerrain::~CCollTerrain()
{
}

int CCollTerrain::Update()
{
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_pManagement->GetComponent(ENGINE::LAYER_OBJECT, L"Terrain", L"Buffer"));
	NULL_CHECK_RETURN(pBuffer, NO_EVENT);

	ENGINE::VTX_TEX* pTerrainVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(pTerrainVtx, NO_EVENT);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_pObject->GetComponent(L"Transform"));
	NULL_CHECK_RETURN(pTransform, NO_EVENT);

	D3DXVECTOR3 vPlayerPos = pTransform->GetPos();

	int iRow = int(vPlayerPos.z / TERRAIN_ITV);
	int iCol = int(vPlayerPos.x / TERRAIN_ITV);

	int iIndex = iRow * TERRAIN_CNTX + iCol;

	D3DXPLANE plane;

	float fRatioX = vPlayerPos.x - pTerrainVtx[iIndex + TERRAIN_CNTX].vPos.x;
	float fRatioZ = pTerrainVtx[iIndex + TERRAIN_CNTX].vPos.z - vPlayerPos.z;

	if (fRatioZ < fRatioX) // ������ �ﰢ��
	{
		D3DXPlaneFromPoints(
			&plane,
			&pTerrainVtx[iIndex + TERRAIN_CNTX].vPos,
			&pTerrainVtx[iIndex + TERRAIN_CNTX + 1].vPos,
			&pTerrainVtx[iIndex + 1].vPos);
	}
	else // ���� �ﰢ��
	{
		D3DXPlaneFromPoints(
			&plane,
			&pTerrainVtx[iIndex + TERRAIN_CNTX].vPos,
			&pTerrainVtx[iIndex + 1].vPos,
			&pTerrainVtx[iIndex].vPos);
	}

	// ����� �������� �̿��ؼ� �÷��̾� ���� �¿�����!
	//0 = ax + by + cz + d: ���� ���� ����� ����Ѵ�.
	//0 < ax + by + cz + d: ���� ���� ��麸�� ���� �ִ�.
	//0 > ax + by + cz + d: ���� ���� ��麸�� �Ʒ��� �ִ�.
	//y = -(ax + cz + d) / b
	vPlayerPos.y = -(plane.a * vPlayerPos.x + plane.c * vPlayerPos.z + plane.d) / plane.b;
	vPlayerPos.y += 1.f;
	pTransform->SetPos(vPlayerPos);

	return NO_EVENT;
}

CCollTerrain* CCollTerrain::Create(ENGINE::CGameObject* pObject)
{
	NULL_CHECK_RETURN(pObject, nullptr);

	return new CCollTerrain(pObject);
}
