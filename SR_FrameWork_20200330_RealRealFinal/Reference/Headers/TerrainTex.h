#pragma once

#ifndef __TERRAINTEX_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CTerrainTex :	public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTerrainTex();

public:
	virtual void Render(WORD wIndex = 0) override;

private:	
	virtual void Release() override;

private:
	HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv = 1);
	DWORD* LoadHeightMap(const wstring& wstrFilePath);

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv = 1);
};

END

#define __TERRAINTEX_H__
#endif
