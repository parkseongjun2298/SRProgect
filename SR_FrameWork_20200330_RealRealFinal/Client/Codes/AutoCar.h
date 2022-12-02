#pragma once
#ifndef __AUTOCAR_H__
#include "SuperCar.h"

class CAutoCar :
	public CSuperCar
{
public:
	explicit CAutoCar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAutoCar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void AutoMove();
	void AutoMove2();

public:
	static CAutoCar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, int m_iTexIndex);

private:
	D3DXVECTOR3		m_vTargetDir;		// 타겟으로 향할 방향.
	float			m_fMoveTime;
	float			m_fMoveAngle;
	float			m_fDotAngle;

};

#define __AUTOCAR_H__
#endif
