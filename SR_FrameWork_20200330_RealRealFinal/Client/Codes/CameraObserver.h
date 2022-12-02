#pragma once

#ifndef __CAMERA_OBSERVER_H__

#include "Observer.h"

class CCameraObserver : public ENGINE::CObserver
{
private:
	CCameraObserver();

public:
	virtual ~CCameraObserver();

public:
	const D3DXVECTOR3& GetCameraPos() const;
	const D3DXMATRIX& GetViewMatrix() const;
	const D3DXMATRIX& GetProjMatrix() const;
	const D3DXVECTOR3& GetCameraLook() const;

public:
	virtual void Update(int iMessage, void* pData) override;

public:
	static CCameraObserver* Create();

private:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_vCameraPos;
	D3DXVECTOR3 m_vCameraLook;
};

#define __CAMERA_OBSERVER_H__
#endif