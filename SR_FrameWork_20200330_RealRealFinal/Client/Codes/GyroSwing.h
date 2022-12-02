
#pragma once

#ifndef __GYROSWING_H__

#include "GameObject.h"


class CGyroSwingBody;
class CGyroSwingLeg;
class CGyroSwingDevice;
class CGyroSwingSpin;
class CGyroSwingDot;
class CGyroSwing :public ENGINE::CGameObject
{
private:
	explicit CGyroSwing(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGyroSwing();
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
	static CGyroSwing* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);



private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	CGyroSwingBody*         m_pBody;
	vector<CGyroSwingLeg*>  m_vLeg;
	CGyroSwingDevice*		m_pDevice;
	vector<CGyroSwingSpin*>	m_vSpin;
	CGyroSwingDot*			m_pDot;
	float					m_fAngleY;
	bool					m_bRideCheck;
};

#define  __GYROSWING_H__
#endif