#include "MathMgr.h"

USING(ENGINE)

CMathMgr::CMathMgr()
{
}


CMathMgr::~CMathMgr()
{
}

void CMathMgr::MyRotationX(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian)
{
	/*
	1	0	0	0
	0	c	s	0
	0	-s	c	0
	0	0	0	1
	*/

	D3DXVECTOR3 vTemp = *pIn;

	pOut->x = vTemp.x;
	pOut->y = vTemp.y * cosf(fRadian) - vTemp.z * sinf(fRadian);
	pOut->z = vTemp.y * sinf(fRadian) + vTemp.z * cosf(fRadian);
}

void CMathMgr::MyRotationY(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian)
{
	/*
	c	0	-s	0
	0	1	0	0
	s	0	c	0
	0	0	0	1
	*/

	D3DXVECTOR3 vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fRadian) + vTemp.z * sinf(fRadian);
	pOut->y = vTemp.y;
	pOut->z = vTemp.x * -sinf(fRadian) + vTemp.z * cosf(fRadian);
}

void CMathMgr::MyRotationZ(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, float fRadian)
{
	/*
	c	s	0	0
	-s	c	0	0
	0	0	1	0
	0	0	0	1
	*/

	D3DXVECTOR3 vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fRadian) - vTemp.y * sinf(fRadian);
	pOut->y = vTemp.x * sinf(fRadian) + vTemp.y * cosf(fRadian);
	pOut->z = vTemp.z;
}
