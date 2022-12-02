#pragma once

#ifndef __CAMERA_H__

#include "GameObject.h"

BEGIN(ENGINE)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CCamera();

public:
	virtual int Update() override;
	D3DXVECTOR3 GetEye() { return m_vEye; }

protected:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vAt;
	float		m_fFovY;
	float		m_fAspect;
	float		m_fNear;
	float		m_fFar;
};

END

#define __CAMERA_H__
#endif