#pragma once

#ifndef __TERRAIN_H__

#include "GameObject.h"

class CUiMgr;
class CTile;
class CTerrain : public ENGINE::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTerrain();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void	KeyInput();

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;
	CUiMgr*					m_pUiMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
};

#define __TERRAIN_H__
#endif