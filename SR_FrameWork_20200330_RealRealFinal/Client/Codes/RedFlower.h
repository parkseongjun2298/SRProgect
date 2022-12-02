#pragma once

#ifndef __REDFLOWER_H__

#include "GameObject.h"

class CRedFlower : public ENGINE::CGameObject
{
private:
	explicit CRedFlower(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CRedFlower();

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
	static CRedFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev,D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vSize;
};

#define __REDFLOWER_H__
#endif

