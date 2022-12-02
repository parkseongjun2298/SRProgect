
#pragma once

#ifndef __COTTENCANDYMGR_H__

#include "GameObject.h"
class CCottenCandyShop;

class CCottenCandyMgr :public ENGINE::CGameObject
{
private:
	explicit CCottenCandyMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCottenCandyMgr();
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
	static CCottenCandyMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);



private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	
	CCottenCandyShop*					m_pCottenCandtShop;
	float					m_fAngleY;
};

#define  __COTTENCANDYMGR_H__
#endif
