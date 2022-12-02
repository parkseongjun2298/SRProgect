#pragma once

#ifndef __TILE_H__

#include "GameObject.h"

class CTile : public ENGINE::CGameObject
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTile();

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
	ENGINE::SAVE_INFO	GetSaveInfo();

public:
	static CTile* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, wstring wstrTag, int iIndex = 0, float fSize = 1.f);

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
	D3DXVECTOR3				m_vPos;
	size_t					m_iTileIndex;
	wstring					m_wstrTileTag;
	bool					m_bDefault;
	float					m_fSize;
};

#define __TILE_H__
#endif

