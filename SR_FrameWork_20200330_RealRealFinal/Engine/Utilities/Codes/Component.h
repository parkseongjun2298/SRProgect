#pragma once

#ifndef __COMPONENT_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CComponent
{
protected:
	CComponent();

public:
	virtual ~CComponent() PURE;

public:
	virtual int Update() { return NO_EVENT; }
	virtual void LateUpdate() {}
	virtual void Render(WORD wIndex = 0) {};
};

END

#define __COMPONENT_H__
#endif
