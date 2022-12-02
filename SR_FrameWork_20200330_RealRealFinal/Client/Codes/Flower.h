#pragma once

#ifndef __FLOWER_H__

#include "GameObject.h"

class CFlower : public ENGINE::CGameObject
{
private:
	explicit CFlower(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CFlower();

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
	static CFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev,D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vSize;
};

#define __FLOWER_H__
#endif

