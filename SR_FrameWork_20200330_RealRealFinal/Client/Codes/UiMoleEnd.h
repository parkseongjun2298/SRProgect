#pragma once
#ifndef __UIMOLEEND_H__

#include "UiPlate.h"
class CUiMoleEnd :
	public CUiPlate
{
public:
	explicit CUiMoleEnd(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiMoleEnd();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CUiMoleEnd* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	float		m_fSizeX;
	float		m_fSizeY;

};

#define __UIMOLEEND_H__
#endif // !__UIMOLEEND_H__
