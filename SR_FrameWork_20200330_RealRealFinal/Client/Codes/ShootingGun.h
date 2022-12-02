#pragma once

#ifndef __SHOOTINGGUN_H__

#include "UiPlate.h"
class CShootingGun :
	public CUiPlate
{
public:
	CShootingGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingGun();
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
	static CShootingGun* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	TCHAR					m_szTextureCount[8];
	int						m_iCount;		//  Á¡¼ö.

	
};

#define __SHOOTINGGUN_H__
#endif // !__SHOOTINGGUN_H__