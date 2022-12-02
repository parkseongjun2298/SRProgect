#pragma once
#ifndef __UIMOLESTART_H__

#include "UiPlate.h"
class CUiMoleStart :
	public CUiPlate
{
public:
	explicit CUiMoleStart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiMoleStart();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CUiMoleStart* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

};

#define __UIMOLESTART_H__
#endif // !__UIMOLESTART_H__
