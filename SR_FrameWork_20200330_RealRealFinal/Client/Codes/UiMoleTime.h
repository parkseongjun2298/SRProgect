#pragma once
#ifndef __UIMOLETIME_H__

#include "UiPlate.h"
class CUiMoleTime :
	public CUiPlate
{
public:
	explicit CUiMoleTime(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiMoleTime();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Render_Time();
	void Notify_Time(int iTime);

public:
	static CUiMoleTime* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureTime[8];
	int						m_iTime;
};

#define __UIMOLETIME_H__
#endif // !__UIMOLETIME_H__
