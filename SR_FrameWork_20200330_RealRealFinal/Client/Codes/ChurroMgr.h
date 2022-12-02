
#pragma once

#ifndef __CHURROMGR_H__

#include "GameObject.h"
class CChurroShop;

class CChurroMgr :public ENGINE::CGameObject
{
private:
	explicit CChurroMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CChurroMgr();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CChurroMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);
	virtual ENGINE::SAVE_INFO GetSaveInfo() override;


private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	
	CChurroShop*					m_pChurroShop;
	float					m_fAngleY;
};

#define  __CHURROMGR_H__
#endif
