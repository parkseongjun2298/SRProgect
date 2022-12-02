#pragma once

#ifndef __RCTEXLYING_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTexLying :	public CVIBuffer
{
private:
	explicit CRcTexLying(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CRcTexLying();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	HRESULT CreateBuffer(D3DXVECTOR3 vPivotPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType);
	virtual void Release() override;

public:
	static CRcTexLying* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,D3DXVECTOR3 vPivotPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType);
};

END

#define __RCTEX_H__
#endif
