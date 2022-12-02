#pragma once

#ifndef __TERRAINCOL_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CTerrainCol :	public CVIBuffer
{
private:
	explicit CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTerrainCol();

public:
	virtual void Render(WORD wIndex = 0) override;

private:	
	virtual void Release() override;

private:
	HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv = 1);

public:
	static CTerrainCol* Create(LPDIRECT3DDEVICE9 pGraphicDev, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv = 1);
};

END

#define __TERRAINCOL_H__
#endif
