#pragma once

#ifndef __MATHMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CMathMgr
{
private:
	CMathMgr();
	~CMathMgr();

public:
	static void MyRotationX(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian);
	static void MyRotationY(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian);
	static void MyRotationZ(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian);
};

END

#define __MATHMGR_H__
#endif

