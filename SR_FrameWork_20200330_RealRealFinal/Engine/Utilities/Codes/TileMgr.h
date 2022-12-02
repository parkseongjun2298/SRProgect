#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)
class ENGINE_DLL CTIleMgr
{
public:
	DECLARE_SINGLETON(CTIleMgr)

private:
	CTIleMgr(void);
	~CTIleMgr(void);

public:
	HRESULT		SetTile(D3DXVECTOR3 vPos, int iSize, TILE_TYPE eType);
	void		DeleteTile(D3DXVECTOR3 vPos, int iSize);
	TILE_TYPE	GetTileType(D3DXVECTOR3 vPos);
	bool		GetTileIdx(D3DXVECTOR3 vPos);
private:

private:
	bool		m_arrTile[TERRAIN_CNTZ - 1][TERRAIN_CNTX - 1];
	TILE_TYPE	m_arrTileType[TERRAIN_CNTZ - 1][TERRAIN_CNTX - 1];
};

END

#endif // __TILEMGR_H__