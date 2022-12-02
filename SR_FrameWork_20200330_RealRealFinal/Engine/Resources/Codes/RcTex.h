#pragma once

#ifndef __RCTEX_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTex :	public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CRcTex();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	HRESULT CreateBuffer(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType);
	virtual void Release() override;

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType = PIVOT_MIDDLE);
};

END

#define __RCTEX_H__
#endif
