#pragma once

#ifndef __FERRISWHEEL_H__
#include "GameObject.h"
class CFerrisBody;
class CFerrisArm;
class CFerrisLeg;
class CFerrisWheel :public ENGINE::CGameObject
{
private:
	explicit CFerrisWheel(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CFerrisWheel();

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
	static CFerrisWheel* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0);


private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	CFerrisBody*				m_pBody;

	vector<CFerrisArm*>			m_pVecArm;
	vector<CFerrisLeg*>			m_pVecLeg;
	ENGINE::INFO				m_tInfo;
	float						m_fAngleY;
	bool						m_bRideCheck[8];
};
#define __FERRISWHEEL_H__
#endif
