#include "stdafx.h"
#include "CollMouse.h"
#include "CameraObserver.h"
#include "VIBuffer.h"
#include "GameObject.h"
#include "Transform.h"

CCollMouse::CCollMouse(CCameraObserver* pObserver)
	: m_pObserver(pObserver),
	m_pManagement(ENGINE::CManagement::GetInstance())
{
}


CCollMouse::~CCollMouse()
{
}

bool CCollMouse::PickTerrain(D3DXVECTOR3* pOut, ENGINE::LAYER_TYPE eLayerType, const wstring& wstrObjectKey)
{
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_pManagement->GetComponent(eLayerType, wstrObjectKey, L"Buffer"));
	NULL_CHECK_RETURN(pBuffer, false);

	ENGINE::VTX_TEX* pTerrainVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(pTerrainVtx, false);

	D3DXMATRIX matTerrain;
	D3DXMatrixIdentity(&matTerrain);
	TranslateLocalSpace(&matTerrain);

	int iIndex = 0;

	float fU = 0.f, fV = 0.f, fDist = 0.f;

	for (int i = 0; i < TERRAIN_CNTZ - 1; ++i)
	{
		for (int j = 0; j < TERRAIN_CNTX - 1; ++j)
		{
			iIndex = i * TERRAIN_CNTX + j;

			// ������ �ﰢ��
			if (D3DXIntersectTri(
				&pTerrainVtx[iIndex + TERRAIN_CNTX].vPos,
				&pTerrainVtx[iIndex + TERRAIN_CNTX + 1].vPos,
				&pTerrainVtx[iIndex + 1].vPos,
				&m_vRayPivot,
				&m_vRayDir,
				&fU,
				&fV,
				&fDist /* ���� ���������� ���������� �Ÿ� */)) 
			{
				// ��ŷ ���� ���ϴ� ����.
				// V1 + U(V2 - V1) + V(V3 - V1)
				*pOut = pTerrainVtx[iIndex + TERRAIN_CNTX].vPos + 
					fU * (pTerrainVtx[iIndex + TERRAIN_CNTX + 1].vPos - pTerrainVtx[iIndex + TERRAIN_CNTX].vPos) +
					fV * (pTerrainVtx[iIndex + 1].vPos - pTerrainVtx[iIndex + TERRAIN_CNTX].vPos);
				pOut->x = (float)((int)pOut->x / TERRAIN_ITV) * TERRAIN_ITV;
				pOut->z = (float)((int)pOut->z / TERRAIN_ITV) * TERRAIN_ITV;
				return true;
			}

			// ���� �ﰢ��
			else if (D3DXIntersectTri(
				&pTerrainVtx[iIndex + TERRAIN_CNTX].vPos,
				&pTerrainVtx[iIndex + 1].vPos,
				&pTerrainVtx[iIndex].vPos,
				&m_vRayPivot,
				&m_vRayDir,
				&fU,
				&fV,
				&fDist /* ���� ���������� ���������� �Ÿ� */))
			{
				// ��ŷ ���� ���ϴ� ����.
				// V1 + U(V2 - V1) + V(V3 - V1)
				*pOut = pTerrainVtx[iIndex + TERRAIN_CNTX].vPos +
					fU * (pTerrainVtx[iIndex + 1].vPos - pTerrainVtx[iIndex + TERRAIN_CNTX].vPos) +
					fV * (pTerrainVtx[iIndex].vPos - pTerrainVtx[iIndex + TERRAIN_CNTX].vPos);
				pOut->x = (float)((int)pOut->x / TERRAIN_ITV) * TERRAIN_ITV;
				pOut->z = (float)((int)pOut->z / TERRAIN_ITV) * TERRAIN_ITV;
				return true;
			}
		}
	}

	return false;
}

bool CCollMouse::PickTile(D3DXVECTOR3 * pOut, ENGINE::LAYER_TYPE eLayerType, const wstring & wstrObjectKey, ENGINE::VTX_TEX* pVtx, D3DXMATRIX matWorld)
{
	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_pManagement->GetComponent(eLayerType, wstrObjectKey, L"Buffer"));
	NULL_CHECK_RETURN(pBuffer, false);

	ENGINE::VTX_TEX* pTerrainVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(pTerrainVtx, false);

	D3DXMATRIX matTile = matWorld;
	TranslateLocalSpace(&matTile);

	float fU = 0.f, fV = 0.f, fDist = 0.f;

	// ������ �ﰢ��
	if (D3DXIntersectTri(
		&pVtx[0].vPos,
		&pVtx[1].vPos,
		&pVtx[2].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist /* ���� ���������� ���������� �Ÿ� */))
	{
		return true;
	}

	// ���� �ﰢ��
	else if (D3DXIntersectTri(
		&pVtx[0].vPos,
		&pVtx[2].vPos,
		&pVtx[3].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist /* ���� ���������� ���������� �Ÿ� */))
	{
		return true;
	}

	return false;
}

bool CCollMouse::PickMole(ENGINE::CGameObject * pObject)
{
	/*
	0	1	|	4	5	| 4 5
	3	2	|	7	6	| 0	1
	*/

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pObject->GetComponent(L"Buffer"));
	NULL_CHECK_RETURN(pBuffer, false);

	ENGINE::VTX_CUBETEX* pCubeVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(pCubeVtx, false);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(pObject->GetComponent(L"Transform"));
	NULL_CHECK_RETURN(pTransform, false);

	// �δ����� ��������� ���ý����̽����� �ٿ�.
	D3DXMATRIX	matMole;
	matMole = pTransform->GetWorldMatrix();
	TranslateLocalSpace(&matMole);

	float fU = 0.f, fV = 0.f, fDist = 0.f;

	//// ����
	/* ������ �ﰢ��
	4	5		0 1	 4 5
	1		3 2	 7 6
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[4].vPos,
		&pCubeVtx[5].vPos,
		&pCubeVtx[1].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		//cout << "������ �浹" << endl;
		return true;
	}
	/* ���� �ﰢ��
	4
	2	1
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[4].vPos,
		&pCubeVtx[1].vPos,
		&pCubeVtx[2].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		//cout << "���� �浹" << endl;
		return true;
	}

	//// ������ ��
	/* ������ �ﰢ��
	1	5
	6
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[1].vPos,
		&pCubeVtx[5].vPos,
		&pCubeVtx[6].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{

		return true;
	}
	/* ���� �ﰢ��
	1
	2	6
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[1].vPos,
		&pCubeVtx[6].vPos,
		&pCubeVtx[2].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		return true;
	}

	//// ���� ��
	/* ������ �ﰢ��
	0	4
	7
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[0].vPos,
		&pCubeVtx[4].vPos,
		&pCubeVtx[7].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		return true;
	}
	/* ���� �ﰢ��
	0
	3	7
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[0].vPos,
		&pCubeVtx[7].vPos,
		&pCubeVtx[3].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		return true;
	}

	//// ����
	/* ������ �ﰢ��
	0	1
	2
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[0].vPos,
		&pCubeVtx[1].vPos,
		&pCubeVtx[2].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		return true;
	}
	/* ���� �ﰢ��
	0
	3	2
	*/
	if (D3DXIntersectTri(
		&pCubeVtx[0].vPos,
		&pCubeVtx[2].vPos,
		&pCubeVtx[3].vPos,
		&m_vRayPivot,
		&m_vRayDir,
		&fU,
		&fV,
		&fDist))
	{
		return true;
	}

	return false;
}

void CCollMouse::TranslateLocalSpace(const D3DXMATRIX* pWorld)
{
	POINT pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt); // ����Ʈ ����

	// ����Ʈ -> ���� �����̽�
	D3DXVECTOR3 vMouse = { 0.f, 0.f, 1.f };

	vMouse.x = float(pt.x) / (WINCX >> 1) - 1.f;
	vMouse.y = 1.f - float(pt.y) / (WINCY >> 1);

	// ���� �����̽� -> �� �����̽�
	D3DXMATRIX matInvProj;
	D3DXMatrixInverse(&matInvProj, nullptr, &m_pObserver->GetProjMatrix());
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matInvProj);

	// �� �����̽�
	m_vRayPivot = { 0.f, 0.f, 0.f };
	m_vRayDir = vMouse - m_vRayPivot;

	// �� �����̽� -> ���� �����̽�
	D3DXMATRIX matInvView;
	D3DXMatrixInverse(&matInvView, nullptr, &m_pObserver->GetViewMatrix());

	D3DXVec3TransformCoord(&m_vRayPivot, &m_vRayPivot, &matInvView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	// ���� �����̽� -> ���� �����̽�
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, nullptr, pWorld);

	D3DXVec3TransformCoord(&m_vRayPivot, &m_vRayPivot, &matInvWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvWorld);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

CCollMouse* CCollMouse::Create(CCameraObserver* pObserver)
{
	NULL_CHECK_RETURN(pObserver, nullptr);

	return new CCollMouse(pObserver);
}
