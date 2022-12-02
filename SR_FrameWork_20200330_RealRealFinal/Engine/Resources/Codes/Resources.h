#pragma once

#ifndef __RESOURCES_H__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CResources();

public:
	virtual CResources* Clone() PURE;
	virtual void Render(WORD wIndex = 0) PURE;

protected:
	virtual void Release() PURE;

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	DWORD*				m_pRefCnt;
};

END

#define __RESOURCES_H__
#endif
