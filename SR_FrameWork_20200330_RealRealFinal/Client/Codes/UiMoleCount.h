#pragma once
#ifndef __UIMOLECOUNT_H__

#include "UiPlate.h"
class CUiMoleCount :
	public CUiPlate
{
public:
	explicit CUiMoleCount(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiMoleCount();

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
	static CUiMoleCount* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureCount[8];
	int						m_iCount;		// 두더지를 잡고 얻는 점수.
};

#define __UIMOLECOUNT_H__
#endif // !__UIMOLECOUNT_H__
