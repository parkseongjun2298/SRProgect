#pragma once

#ifndef __RCCOL_H__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcCol :	public CVIBuffer
{
private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CRcCol();

public:
	virtual void Render(WORD wIndex = 0) override;

private:
	virtual HRESULT CreateBuffer() override;
	virtual void Release() override;

public:
	static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END

#define __RCCOL_H__
#endif
