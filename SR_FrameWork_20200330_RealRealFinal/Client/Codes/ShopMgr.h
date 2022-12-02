
#pragma once

#ifndef __SHOPMGR_H__

#include "GameObject.h"
class CShop;

class CShopMgr :public ENGINE::CGameObject
{
private:
	explicit CShopMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CShopMgr();
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
	static CShopMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);



private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	
	CShop*					m_pShop;
	float					m_fAngleY;
};

#define  __SHOPMGR_H__
#endif
