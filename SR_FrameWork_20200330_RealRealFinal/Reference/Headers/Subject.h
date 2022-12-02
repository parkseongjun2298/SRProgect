#pragma once

#ifndef __SUBJECT_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CObserver;
class ENGINE_DLL CSubject
{
private:
	CSubject();	

public:
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notify(int iMessage, void* pData);

public:
	static CSubject* Create();

private:
	list<CObserver*>	m_ObserverList;
};

END

#define __SUBJECT_H__
#endif