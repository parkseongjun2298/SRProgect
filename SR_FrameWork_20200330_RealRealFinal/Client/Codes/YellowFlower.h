#pragma once

#ifndef __YELLOWFLOWER_H__

#include "GameObject.h"

class CYellowFlower : public ENGINE::CGameObject
{
private:
	explicit CYellowFlower(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CYellowFlower();

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
	static CYellowFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev,D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vSize;
};

#define __YELLOWFLOWER_H__
#endif

