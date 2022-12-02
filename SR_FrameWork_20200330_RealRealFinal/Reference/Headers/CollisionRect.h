#pragma once

#ifndef __COLLISIONRECT_H__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CCollisionRect :
	public CComponent
{
private:
	explicit CCollisionRect();
public:
	virtual ~CCollisionRect();

public:
	virtual void LateUpdate() override;

public:
	const RECT* GetCollisionBox() { return &m_rcCollision; }

public:
	static CCollisionRect* Create(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);

private:
	RECT	m_rcCollision;
};

END

#define __COLLISIONRECT_H__
#endif