#pragma once
#include "UiPlate.h"
class CTextureUi :
	public CUiPlate
{
private:
	explicit CTextureUi(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTextureUi();

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
	static CTextureUi* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

public:
	void Render_Talk();

public:
	void Update_Talk();
public:
	void SetTexture(wstring wstrTalk);

private:
	ENGINE::CFontMgr*		m_pFontMgr;

	list<wstring>			m_listTalk;
	wstring					m_wstrTalk;
	int						m_iIndex;
	float					m_fTime = 0.f;
	TCHAR					m_szTexture[128];
};

