#pragma once

#ifndef __DEADOBJ_H__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CDeadObj : public CComponent
{
private:
	explicit CDeadObj();

public:
	virtual ~CDeadObj();

public:
	virtual int Update() override;

public:
	void	SetDead() { m_bDead = true; }

public:
	bool	GetDead() { return m_bDead; }

public:
	static CDeadObj* Create();

private:
	bool	m_bDead;
};

END

#define __DEADOBJ_H__
#endif

