#pragma once

#ifndef __GRASS_H__

#include "GameObject.h"

class CGrass : public ENGINE::CGameObject
{
private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CGrass();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void IsBillboarding();

public:
	static CGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
};

#define __GRASS_H__
#endif

