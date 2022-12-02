#pragma once

#ifndef __UIMGR_H__
BEGIN(ENGINE)
class CTransform;
class CDeadObj;
END
class CUiTexture;
class CUiGroup;
class CUiPlate;
class CUiMgr
{
	DECLARE_SINGLETON(CUiMgr)
private:
	CUiMgr();
	~CUiMgr();
public:
	bool	Update();
	void	LateUpdate();
	void	Render();
public:
	HRESULT		SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev);
	void		SetOpen(bool bOpen) { m_bOpen = bOpen; m_bFirstFrame = true; }
	wstring		GetwstrSelect();
	size_t		GetiSelect();
	ENGINE::CTransform* GetTransform() { return m_pTransform; }
private:
	void	Release();
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;
	vector<CUiGroup*>		m_vecUiGroup;
	CUiPlate*				m_pBackGround;
	//CUiTexture*				m_pUiTexture;	// Ãß°¡.
	size_t					m_iGroupIdx;
	ENGINE::CTransform*		m_pTransform;
	bool					m_bOpen;
	bool					m_bFirstFrame;

	int						m_iSelectIdx;
	wstring					m_wstrSelect;
};

#define __UIMGR_H__
#endif
