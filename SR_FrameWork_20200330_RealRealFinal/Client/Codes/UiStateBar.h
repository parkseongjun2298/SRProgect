#pragma once
#ifndef __UISTATEBAR_H__

#include "UiPlate.h"
class CUiStateBar :
	public CUiPlate
{
private:
	explicit CUiStateBar(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CUiStateBar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Render_State();

public:
	void Move_StateBar();
	void PlusCustomer() { ++m_iCustomer; }
	void MinusCustomer() { --m_iCustomer; }
	void PlusCoin(int iCoin) { m_iCoin += iCoin; }

public:
	static CUiStateBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);


private:
	ENGINE::CFontMgr*		m_pFontMgr;
	bool					m_bIsMove;

	wstring					m_wstrCoin;
	wstring					m_wstrPopulation;

	D3DXVECTOR3				m_vPaintPos;

	float					m_fTime = 0.f;
	TCHAR					m_szTextureCoin[64];
	TCHAR					m_szTexturePopulation[64];

	int						m_iCoin;		// Player¿« ∞ÒµÂ∑Æ
	int						m_iCustomer; // º’¥‘¿« ¿Œ±∏ºˆ
};

#define __UISTATEBAR_H__
#endif // !__UISTATEBAR_H__
