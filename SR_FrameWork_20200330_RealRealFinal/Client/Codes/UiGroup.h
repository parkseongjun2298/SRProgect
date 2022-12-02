#pragma once

#ifndef __UIGROUP_H__

class CUiPlate;
class CUiGroup
{
private:
	CUiGroup(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CUiGroup();

private:
	HRESULT Initialize();
	void	Release();

public:
	int		Update(wstring* pwstrSelect, int* piSelect);
	void	LateUpdate();
	void	Render();

public:
	//wstring GetwstrSelect() { return m_wstrBuilding; }
	//size_t	GetiSelect() { return m_iBuildingIndex; }

public:
	static CUiGroup* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::BUILDING_UI_TYPE eType);
private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	CUiMgr*						m_pUiMgr;

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	ENGINE::BUILDING_UI_TYPE	m_eUiType;
	wstring						m_wstrBuilding;
	size_t						m_iBuildingIndex;
	vector<CUiPlate*>			m_vecUiPlate;
	vector<RECT>				m_vecCollisionBox;
};

#define __UIGROUP_H__
#endif