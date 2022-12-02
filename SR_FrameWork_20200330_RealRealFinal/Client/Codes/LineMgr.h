#pragma once

#ifndef __LINEMGR_H__

class CCameraObserver;
class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();
public:
	static void DrawBoxNoTransform(const RECT& rc, DWORD dwColor = 0xffff0000, float fWidth = 2.f);
};

#define __LINEMGR_H__
#endif