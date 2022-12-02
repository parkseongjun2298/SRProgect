#pragma once
#ifndef __EFFECT_H__

#include "GameObject.h"
class CEffect :
	public ENGINE::CGameObject
{
public:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual void Render();

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);
};

#define __EFFECT_H__
#endif // !__EFFECT_H__
