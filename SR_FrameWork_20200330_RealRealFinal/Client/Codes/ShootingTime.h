#pragma once
#ifndef __SHOOTINGTIME_H__
#include "UiPlate.h"
class CShootingTime :
	public CUiPlate
{
public:
	CShootingTime(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingTime();
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
	static CShootingTime* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureTime[8];
	int						m_iTime;
};

#define __SHOOTINGTIME_H__
#endif // !__SHOOTINGTIME_H__