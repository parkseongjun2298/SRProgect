#pragma once

#ifndef __DEVICEMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9	GetDevice() const;
	LPD3DXSPRITE		GetSprite() const;
	LPD3DXFONT			GetFont() const;
	LPD3DXLINE			GetLine() const;
public:
	HRESULT InitDevice(HWND hWnd, const WORD& wCX, const WORD& wCY, DISPLAY_MODE eMode);

public:
	void				DrawFont(D3DXMATRIX matTrans, TCHAR* strOut, DWORD dwARGB);

private:
	void Release();

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
};

END

#define __DEVICEMGR_H__
#endif