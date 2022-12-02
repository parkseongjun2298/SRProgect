#pragma once
#ifndef __UINPC_H__

#include "UiPlate.h"
class CUiNpc :
	public CUiPlate
{
public:
	explicit CUiNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiNpc();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CUiNpc* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);
};

#define __UINPC_H__
#endif // !__UINPC_H__
