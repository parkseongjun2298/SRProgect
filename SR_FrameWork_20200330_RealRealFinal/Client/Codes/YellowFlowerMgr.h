
#pragma once

#ifndef __YELLOWFLOWERMGR_H__

#include "GameObject.h"
	class CYellowFlower;

class CYellowFlowerMgr :public ENGINE::CGameObject
{
private:
	explicit CYellowFlowerMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CYellowFlowerMgr();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
	virtual ENGINE::SAVE_INFO GetSaveInfo() override;
public:
	static CYellowFlowerMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

public:
	virtual D3DXVECTOR3 GetPos();

private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;

	CYellowFlower*					m_pFlower;
	float					m_fAngleY;
};

#define  __YELLOWFLOWERMGR_H__
#endif
