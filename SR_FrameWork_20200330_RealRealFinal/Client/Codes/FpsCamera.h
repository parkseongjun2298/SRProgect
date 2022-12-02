#pragma once

#ifndef __FPS_CAMERA_H__

#include "Camera.h"

BEGIN(ENGINE)
class CGameObject;
END
class CFpsCamera : public ENGINE::CCamera
{
private:
	explicit CFpsCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CFpsCamera();

public:
	virtual int Update() override;
	int			Update(Matrix matParentWorld);

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void	KeyInput();
	void	CameraRebound();
	void	CameraBreath();
public:
	void	CameraRebound(float fPower);
	void	CameraBreath(bool bSet);
	void	SetCustomer(ENGINE::CGameObject* pGameObject = nullptr, ENGINE::CGameObject* pParent = nullptr, Matrix matParentWorld = MATRIX::Identity(), Matrix matSeat = MATRIX::Identity());
	void	ClearCustomer();
	void	SetRail(ENGINE::CGameObject* pParent, ENGINE::CGameObject* pGameObject);

	ENGINE::CGameObject* GetSetCustomer() { if (m_bSet) return m_pSet; return nullptr; }
public:
	static CFpsCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CManagement* m_pManagement;
	ENGINE::CKeyMgr*	 m_pKeyMgr;
	ENGINE::CTimeMgr*	 m_pTimeMgr;
	ENGINE::CSubjectMgr* m_pSubjectMgr;
	//POINT				 m_ptOldpt;
	float				 m_fLockAngleY;

	bool				 m_bRebound;
	float				 m_fPower;
	Vec3				 m_vReboundDir;

	bool				 m_bBreath;
	float				 m_fBreathTime;

	ENGINE::CGameObject* m_pSet;
	ENGINE::CGameObject* m_pParentSet;
	bool				 m_bSet;
	Matrix				 m_matParentWorld;
	Matrix				 m_matSeat;

	Vec3				 m_vUp;
};

#define __FPS_CAMERA_H__
#endif