#pragma once

#ifndef __EVENTMGR_H__

class CPlayer;
class CUiTexture;
class CUiNpc;
class CEventMgr
{
	DECLARE_SINGLETON(CEventMgr)
private:
	CEventMgr();
	~CEventMgr();

public:
	int		Update();
	void	LateUpdate();
	void	Render();

public:
	void	Initialize(CPlayer* pPlayer, LPDIRECT3DDEVICE9 pGraphicDev);
	void	Release();

private:
	void	CreateUiTexture(vector<wstring> vecwstrTalk, size_t iSize);

public:
	void	NotifyQuestCondition(QUEST_CONDITION eType) { m_mapQuest[eType] = true; }
	void	SetQuestClear(int iQuestIdx) { if (iQuestIdx == m_iCurQuest) m_vecbQuest[iQuestIdx] = true; }
	bool	GetQuestClear(int iQuestIdx) { if (iQuestIdx >= m_vecbQuest.size()) return false; return m_vecbQuest[iQuestIdx]; }


private:
	ENGINE::CKeyMgr*	m_pKeyMgr;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CPlayer*			m_pPlayer;
	CUiTexture*			m_pTexture;
	CUiNpc*				m_pNpc;
	vector<bool>		m_vecbQuest;
	size_t				m_iCurQuest;

	map<QUEST_CONDITION, bool>			m_mapQuest;
};

#define __EVENTMGR_H__
#endif