#pragma once

#ifndef __SHOOTINGGAME_H__

#include "Scene.h"


class CShootingGame : public ENGINE::CScene
{
private:
	explicit CShootingGame(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CShootingGame();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT Add_Environment_Layer() override;
	virtual HRESULT Add_Object_Layer() override;
	virtual HRESULT Add_Effect_Layer() override;
	virtual HRESULT Add_UI_Layer() override;

public:
	static CShootingGame* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	CUiMgr*					m_pUiMgr;
	CCameraMgr*				m_pCameraMgr;

};

#define __SHOOTINGGAME_H__
#endif