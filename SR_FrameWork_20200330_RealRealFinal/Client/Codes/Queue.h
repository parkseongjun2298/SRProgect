#pragma once

#ifndef __QUEUE_H__

#include "Component.h"
BEGIN(ENGINE)
class CGameObject;
END
class CQueue : public ENGINE::CComponent
{
private:
	explicit CQueue();

public:
	virtual ~CQueue();

public:
	virtual int Update() override;

public:
	void					PushCustomer(ENGINE::CGameObject* pCustomer);
	ENGINE::CGameObject*	PullCustomer();

public:
	static CQueue* Create(Vec3 vRidePos, wstring wstrRideTag);

private:
	list<ENGINE::CGameObject*>	m_lstQueue;
	list<ENGINE::CGameObject*>* m_lstCustomer;
	Vec3						m_vRidePos;
	wstring						m_wstrRideTag;
};

#define __QUEUE_H__
#endif