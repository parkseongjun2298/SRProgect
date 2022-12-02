#include "TileMgr.h"
#include "Font.h"

USING(ENGINE)

IMPLEMENT_SINGLETON(CTIleMgr)

CTIleMgr::CTIleMgr(void)
{
	ZeroMemory(m_arrTile, sizeof(m_arrTile));
	ZeroMemory(m_arrTileType, sizeof(m_arrTileType));
}

CTIleMgr::~CTIleMgr(void)
{
}

HRESULT CTIleMgr::SetTile(D3DXVECTOR3 vPos, int iSize, TILE_TYPE eType)
{
	int iIdxXMin = (int)vPos.x / TERRAIN_ITV - iSize / 2;
	int iIdxZMin = (int)vPos.z / TERRAIN_ITV - iSize / 2;
	int iIdxXMax = iIdxXMin + iSize;
	int iIdxZMax = iIdxZMin + iSize;

	for(int i = iIdxZMin; i <= iIdxZMax; ++i)
		for(int j = iIdxXMin; j <= iIdxXMax; ++j)
			if (true == m_arrTile[i][j])
				return E_FAIL;

	for (int i = iIdxZMin; i <= iIdxZMax; ++i)
		for (int j = iIdxXMin; j <= iIdxXMax; ++j)
		{
			m_arrTile[i][j] = true;
			m_arrTileType[i][j] = eType;
		}

	return S_OK;
}

void CTIleMgr::DeleteTile(D3DXVECTOR3 vPos, int iSize)
{
	int iIdxXMin = (int)vPos.x / TERRAIN_ITV - iSize / 2;
	int iIdxZMin = (int)vPos.z / TERRAIN_ITV - iSize / 2;
	int iIdxXMax = iIdxXMin + iSize;
	int iIdxZMax = iIdxZMin + iSize;

	for (int i = iIdxZMin; i <= iIdxZMax; ++i)
		for (int j = iIdxXMin; j <= iIdxXMax; ++j)
		{
			m_arrTile[i][j] = false;
		}
}

TILE_TYPE CTIleMgr::GetTileType(D3DXVECTOR3 vPos)
{
	int iIdxX = (int)vPos.x / TERRAIN_ITV;
	int iIdxZ = (int)vPos.z / TERRAIN_ITV;
	return m_arrTileType[iIdxZ][iIdxX];
}

bool CTIleMgr::GetTileIdx(D3DXVECTOR3 vPos)
{
	int iIdxX = (int)vPos.x / TERRAIN_ITV;
	int iIdxZ = (int)vPos.z / TERRAIN_ITV;
	return m_arrTile[iIdxZ][iIdxX];
}
