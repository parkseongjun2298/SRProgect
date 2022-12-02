#pragma once

#ifndef __CROSSHAIR_H__

#include "UiPlate.h"
class CCrossHair :
	public CUiPlate
{
public:
	CCrossHair(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrossHair();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Render_Count();
	void Notify_Count(int iCount);

public:
	static CCrossHair* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	TCHAR					m_szTextureCount[8];
	int						m_iCount;		//  Á¡¼ö.

	
};

#define __CROSSHAIR_H__
#endif // !__CROSSHAIR_H__