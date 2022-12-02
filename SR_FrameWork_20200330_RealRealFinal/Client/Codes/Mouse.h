#pragma once

#ifndef __MOUSE_H__

BEGIN(ENGINE)
class CGameObject;
class CComponent;
END

class CCameraObserver;
class CMouse
{
	DECLARE_SINGLETON(CMouse);
private:
	CMouse();
	~CMouse();
public:
	void	SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	void	Update();
	void	LateUpdate();
	void	Render();
private:
	void	Release();
public:
	void	CreateMouse(wstring wstrTag, int iIdx);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	ENGINE::CGameObject*	m_pMouse;
	ENGINE::CGameObject*	m_pTile;
	ENGINE::CComponent*		m_pCollMouse;
	CCameraObserver*		m_pCameraObserver;
};

#define __LINEMGR_H__
#endif