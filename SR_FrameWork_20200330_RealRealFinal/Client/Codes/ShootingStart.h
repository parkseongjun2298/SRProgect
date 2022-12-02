#pragma once

#ifndef __UISHOOTINGSTART_H__

#include "UiPlate.h"
class CShootingStart :
	public CUiPlate
{
public:
	CShootingStart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingStart();
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
	static CShootingStart* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureCount[8];
	int						m_iCount;		//  Á¡¼ö.

	bool					m_bCheck;
};

#define __UISHOOTINGSTART_H__
#endif // !__UISHOOTINGSTART_H__