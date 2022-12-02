#pragma once

#ifndef __TRICOL_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CTriCol :	public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTriCol();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	virtual HRESULT CreateBuffer() override;
	virtual void Release() override;

public:
	static CTriCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END

#define __TRICOL_H__
#endif
