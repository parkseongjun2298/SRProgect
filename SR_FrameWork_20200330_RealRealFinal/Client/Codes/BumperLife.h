#pragma once
#ifndef __BUMPERLIFE_H__

#include "UiPlate.h"
class CBumperLife :
	public CUiPlate
{
public:
	explicit CBumperLife(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperLife();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CBumperLife* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);
};

#define __BUMPERLIFE_H__
#endif // !__BUMPERLIFE_H__
