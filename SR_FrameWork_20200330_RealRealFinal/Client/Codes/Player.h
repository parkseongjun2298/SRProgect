#pragma once

#ifndef __PLAYER_H__

#include "GameObject.h"

class CPlayer : public ENGINE::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CPlayer();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void KeyInput();

public:
	int		GetGold() { return m_iGold; }

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	int						m_iGold;
};

#define __PLAYER_H__
#endif

