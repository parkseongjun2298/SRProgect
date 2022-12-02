#pragma once
#ifndef __CAMERAMGR_H__

BEGIN(ENGINE)
class CCamera;
END
class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	CCameraMgr();
	~CCameraMgr();
public:
	void				SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; Initiailize();}
public:
	ENGINE::CCamera*	GetCurrentCamera() { return m_vecCamera[m_eCurrentScene][m_iIndex]; }
	int					GetCurrentIdx() { return m_iIndex; }
	void				PushCamera(ENGINE::CCamera* pCamera);
	void				ChangeSceneCamera(ENGINE::SCENE_TYPE eSceneType, bool bClear = false);
private:
	HRESULT	Initiailize();
public:
	int		Update();
	void	SetCameraIdx(int iIdx) { m_iIndex = iIdx; ::ShowCursor(TRUE); }
private:
	vector<ENGINE::CCamera*>	m_vecCamera[ENGINE::SCENE_TYPE::SCENE_END];
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::SCENE_TYPE			m_eCurrentScene;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	int							m_iIndex;
};

#define __CAMERAMGR_H__
#endif