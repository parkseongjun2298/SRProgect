#pragma once
#ifndef __UITEXTURE_H__

#include "UiPlate.h"
class CUiTexture :
	public CUiPlate
{
public:
	explicit CUiTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUiTexture();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	virtual void KeyInput();

public:
	static CUiTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

public:
	void Render_Talk();
	int	 Update_Talk();

public:
	void SetTexture(wstring wstrTalk);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	list<wstring>			m_listTalk;
	wstring					m_wstrTalk;
	size_t					m_iIndex;
	float					m_fTime = 0.f;
	TCHAR					m_szTexture[128];
};

#define __UITEXTURE_H__
#endif // !__UITEXTURE_H__
