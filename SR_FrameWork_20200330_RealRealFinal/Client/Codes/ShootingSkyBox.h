#pragma once

#ifndef __SHOOTINGSKYBOX_H__

#include "GameObject.h"

class CShootingSkyBox : public ENGINE::CGameObject
{
private:
	explicit CShootingSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CShootingSkyBox();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CShootingSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
};

#define __SHOOTINGSKYBOX_H__
#endif

