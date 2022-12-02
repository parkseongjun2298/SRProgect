#pragma once

#ifndef __SUBSTAGEEXAMPLE_H__

#include "Scene.h"

class CPlayer;
class CTerrain;
class CSubStageExample : public ENGINE::CScene
{
private:
	explicit CSubStageExample(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CSubStageExample();

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
	static CSubStageExample* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;	
	CUiMgr*					m_pUiMgr;
	CCameraMgr*				m_pCameraMgr;
};

#define __SUBSTAGEEXAMPLE_H__
#endif