#pragma once
#ifndef __UIMOLEMOUSE_H__

#include "UiPlate.h"
class CMoleMousePos;
class CUiMoleMouse :
	public CUiPlate
{
public:
	explicit CUiMoleMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiMoleMouse();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Check_Frame();
	void Set_FrameStart(bool bCheck);

public:
	static CUiMoleMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, CMoleMousePos* pMoleMouse, int iIndex = 0);

private:
	CMoleMousePos*		m_pMoleMouse;
	float				m_fFrameIndex;
	bool				m_bFrameStart;
};

#define __UIMOLEMOUSE_H__
#endif // !__UIMOLEMOUSE_H__
