#pragma once

#ifndef __LOGO_H__

#include "Scene.h"
BEGIN(ENGINE)
class CComponent;
END
class CUiPlate;
class CLogo : public ENGINE::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CLogo();

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
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	CUiPlate*				m_pUiPlate;
	ENGINE::CComponent*		m_pBuffer;
	ENGINE::CComponent*		m_pTexture;
};

#define __LOGO_H__
#endif