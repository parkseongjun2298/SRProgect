#pragma once

#ifndef __TEXTURE_H__

#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CTexture :	public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTexture();

public:
	virtual CResources* Clone() override;
	virtual void Render(WORD wIndex = 0) override;

private:
	virtual void Release() override;

public:
	HRESULT LoadTexture(
		TEXTURE_TYPE eType,
		const wstring& wstrFilePath,		
		const WORD& wCount);	
	size_t	GetSize() { return m_vecTexture.size(); }

public:
	static CTexture* Create(
		LPDIRECT3DDEVICE9 pGraphicDev,
		TEXTURE_TYPE eType,
		const wstring& wstrFilePath,	
		const WORD& wCount);

private:
	//LPDIRECT3DTEXTURE9
	//LPDIRECT3DCUBETEXTURE9

	typedef vector<IDirect3DBaseTexture9*>	VEC_TEXTURE;	
	VEC_TEXTURE	m_vecTexture;
};

END

#define __TEXTURE_H__
#endif