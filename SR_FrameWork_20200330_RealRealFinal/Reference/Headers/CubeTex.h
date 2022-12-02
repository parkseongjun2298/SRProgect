#pragma once

#ifndef __CUBETEX_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CCubeTex :	public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CCubeTex();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	HRESULT CreateBuffer(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType);
	virtual void Release() override;

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType);
};

END

#define __CUBETEX_H__
#endif
