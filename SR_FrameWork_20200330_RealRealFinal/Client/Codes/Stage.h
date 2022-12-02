#pragma once

#ifndef __STAGE_H__

#include "Scene.h"

class CPlayer;
class CTerrain;
class CStage : public ENGINE::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CStage();

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
	HRESULT	SaveObject();
	HRESULT LoadObject();

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	CCameraMgr*				m_pCameraMgr;
	CUiMgr*					m_pUiMgr;
	CEventMgr*				m_pEventMgr;
	bool					m_bBuildingUiRender;
	float					m_fCustomerCreateTime;
};

#define __STAGE_H__
#endif