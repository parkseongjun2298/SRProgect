#pragma once

#ifndef __GYRODROPDEVICE_H__
#include "GameObject.h"
class CCustomer;
class CGyroDropDevice : public ENGINE::CGameObject

{
private:
	CGyroDropDevice(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGyroDropDevice();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CGyroDropDevice* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, ENGINE::CGameObject* pBody);
	void GyroStart();
	void PlayGyro();
	bool GetPlayGyro() { return m_bPlay; }

	void PushSeat(int iIdx, CCustomer* pCustomer);
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	float					m_fAngle;
	float					m_fSpeed;
	bool					m_bCheckTop;//꼭대기 찍었나 체크
	bool					m_bRotateCheck; //한바퀴 도는 거 막는거
	bool					m_bCheckBot;
	bool					m_bPlay;
	float					m_fRebound;

	ENGINE::CGameObject*	m_pParent;

	D3DXVECTOR3 Temp;
};

#define __GYRODROPDEVICE_H__
#endif