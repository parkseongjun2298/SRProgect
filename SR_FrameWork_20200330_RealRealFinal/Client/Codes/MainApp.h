#pragma once

#ifndef __MAINAPP_H__

class CSoundMgr;
class CMainApp
{
private:
	explicit CMainApp();

public:
	~CMainApp();

public:
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	void RenderFPS();

public:
	static CMainApp* Create();

private:
	ENGINE::CDeviceMgr*		m_pDeviceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CFontMgr*		m_pFontMgr;
	CSoundMgr*				m_pSoundMgr;

	float		m_fTime = 0.f;
	DWORD		m_dwCount = 0;
	TCHAR		m_szFps[128];
};

#define __MAINAPP_H__
#endif
