#pragma once
#ifndef __MYCAR_H__

#include "SuperCar.h"
class CMyCar :
	public CSuperCar
{
public:
	explicit CMyCar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyCar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void KeyInput();

public:
	static CMyCar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);
};

#define __MYCAR_H__
#endif // !__MYCAR_H__
