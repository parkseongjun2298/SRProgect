#pragma once

#ifndef __UISHOOTINGCOUNT_H__

#include "UiPlate.h"
class CShootingCount :
	public CUiPlate
{
public:
	CShootingCount(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingCount();
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
	static CShootingCount* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureCount[8];
	int						m_iCount;		//  Á¡¼ö.
};

#define __UISHOOTINGCOUNT_H__
#endif // !__UISHOOTINGCOUNT_H__