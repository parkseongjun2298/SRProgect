#pragma once

#ifndef __UIPLATE_H__

#include "GameObject.h"

class CUiPlate : public ENGINE::CGameObject
{
protected:
	explicit CUiPlate(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CUiPlate();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	virtual void KeyInput();

public:
	wstring		GetwstrTag() { return m_wstrTextureTag; }
	size_t		GetIndex() { return m_iTextureIndex; }

public:
	static CUiPlate* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, 
		D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0, bool bMgr = true);

protected:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vSize;
	wstring					m_wstrTextureTag;
	size_t					m_iTextureIndex;
	bool					m_bMgr;
};

#define __UIPLATE_H__
#endif

