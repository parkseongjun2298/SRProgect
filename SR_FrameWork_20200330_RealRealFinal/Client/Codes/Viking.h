#pragma once

#ifndef __VIKING_H__

#include "GameObject.h"
class CVikingLeg;
class CVikingShip;
class CViking :public ENGINE::CGameObject
{
private:
	explicit CViking(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CViking();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	virtual D3DXVECTOR3 GetPos() override;
	virtual ENGINE::SAVE_INFO GetSaveInfo() override;

public:
	static CViking* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);



private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	vector<CVikingLeg*>		m_VecVikingLeg;
	CVikingShip*			m_VikingShip;

	ENGINE::INFO			m_tInfo;
	float					m_fAngleY;
};

#define  __VIKING_H__
#endif