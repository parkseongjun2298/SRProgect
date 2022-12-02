#pragma once

#ifndef __OBSERVER_H__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CObserver : public CComponent
{
protected:
	CObserver();

public:
	virtual ~CObserver();

public:
	virtual void Update(int iMessage, void* pData) PURE;
};

END

#define __OBSERVER_H__
#endif