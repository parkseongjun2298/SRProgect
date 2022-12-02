#pragma once

#ifndef __VIKINGSHIP_H__

#include "GameObject.h"

class CCustomer;
class CVikingShip : public ENGINE::CGameObject
{
private:
	explicit CVikingShip(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CVikingShip();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CVikingShip* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

private:
	void MoveViking();
public:
	void PlayViking();
	bool GetPlaying() { return m_bDirCheck; }
	void PushSeat(int iIdx, CCustomer* pCustomer);
private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx[4];
	ENGINE::VTX_CUBETEX*		m_pConvertVtx[4];

	ENGINE::ANGLE_TYPE	m_eAxis;	//회전 축
	float m_fAngle;//한 프레임당 회전할 각도 
	float m_fAngle2;
	float m_fSpeed;//속도
	float m_fPlayTime;//시간
	float m_fAngleTime;//한 프레임당 증가할 각도 
	float m_fValue;//증가하는 증가값

	bool m_bDirCheck;//29초되면 멈추는거
};

#define __VIKINGSHIP_H__
#endif

