#pragma once
#include "UiPlate.h"
class CUiBase :
	public CUiPlate
{
private:
	explicit CUiBase(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CUiBase();
private:
	virtual void KeyInput();
public:
	static CUiBase* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag);
};

