#pragma once

#ifndef __COLLTERRAIN_H__

#include "Component.h"

BEGIN(ENGINE)
class CGameObject;
END

class CCollTerrain : public ENGINE::CComponent
{
private:
	explicit CCollTerrain(ENGINE::CGameObject* pObject);

public:
	virtual ~CCollTerrain();

public:
	virtual int Update() override;

public:
	static CCollTerrain* Create(ENGINE::CGameObject* pObject);

private:
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CGameObject*	m_pObject;
};

#define __COLLTERRAIN_H__
#endif

