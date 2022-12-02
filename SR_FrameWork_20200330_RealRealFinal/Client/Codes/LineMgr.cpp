#include "stdafx.h"
#include "LineMgr.h"
#include "CameraObserver.h"\

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
}

void CLineMgr::DrawBoxNoTransform(const RECT & rc, DWORD dwColor, float fWidth)
{
	ENGINE::CDeviceMgr::GetInstance()->GetLine()->SetWidth(fWidth);

	D3DXVECTOR2 vBox[5] = {};
	vBox[0] = { (float)rc.left, (float)rc.top };
	vBox[1] = { (float)rc.left, (float)rc.bottom };
	vBox[2] = { (float)rc.right, (float)rc.bottom };
	vBox[3] = { (float)rc.right, (float)rc.top };
	vBox[4] = { (float)rc.left, (float)rc.top };

	ENGINE::CDeviceMgr::GetInstance()->GetLine()->Draw(vBox, 5, dwColor);
}