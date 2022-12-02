#pragma once

#ifndef __DYNAMIC_CAMERA_H__

#include "Camera.h"

class CDynamicCamera : public ENGINE::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CDynamicCamera();

public:
	virtual int Update() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void KeyInput();

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vEye = { 0.f, 20.f, 0.f }, D3DXVECTOR3 vAt = { 0.f, 0.f, 10.f });

private:
	ENGINE::CManagement* m_pManagement;
	ENGINE::CKeyMgr*	 m_pKeyMgr;
	ENGINE::CTimeMgr*	 m_pTimeMgr;
	ENGINE::CSubjectMgr* m_pSubjectMgr;

	float	m_fTargetDist; // 대상과의 거리.

	D3DXVECTOR3 m_vLook;
};

#define __DYNAMIC_CAMERA_H__
#endif