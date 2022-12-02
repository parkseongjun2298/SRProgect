
#pragma once

#ifndef __LIMITTILEMGR_H__

#include "GameObject.h"
class CLimitTile;

class CLimitTileMgr :public ENGINE::CGameObject
{
private:
	explicit CLimitTileMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLimitTileMgr();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CLimitTileMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);



private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	
	vector<CLimitTile*>  m_vTile;

};

#define  __LIMITTILEMGR_H__
#endif
