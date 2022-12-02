#pragma once

#ifndef __STATIC_CAMERA_H__

#include "Camera.h"

class CStaticCamera : public ENGINE::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CStaticCamera();

public:
	virtual int Update() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void KeyInput();

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CManagement* m_pManagement;
	ENGINE::CKeyMgr*	 m_pKeyMgr;
	ENGINE::CTimeMgr*	 m_pTimeMgr;
	ENGINE::CSubjectMgr* m_pSubjectMgr;

	float	m_fTargetDist; // 대상과의 거리.
	float	m_fAngle;
};

#define __STATIC_CAMERA_H__
#endif