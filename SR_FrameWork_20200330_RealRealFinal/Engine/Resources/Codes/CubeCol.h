#pragma once

#ifndef __CUBECOL_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CCubeCol();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	HRESULT CreateBuffer(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, DWORD dwColor, PIVOT_TYPE eType);
	virtual void Release() override;

public:
	static CCubeCol* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, DWORD dwColor, PIVOT_TYPE eType);
};

END

#define __CUBECOL_H__
#endif
