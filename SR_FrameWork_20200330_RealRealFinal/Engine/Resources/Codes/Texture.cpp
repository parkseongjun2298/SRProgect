#include "Texture.h"

USING(ENGINE)

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{
}


CTexture::~CTexture()
{
	Release();
}

CResources* CTexture::Clone()
{
	++(*m_pRefCnt);

	return new CTexture(*this); // 복사 생성
}

void CTexture::Render(WORD wIndex/* = 0*/)
{
	m_pGraphicDev->SetTexture(0, m_vecTexture[wIndex]);
}

void CTexture::Release()
{
	if (0 == --(*m_pRefCnt))
	{
		for_each(m_vecTexture.begin(), m_vecTexture.end(), SafeRelease<IDirect3DBaseTexture9*>);
		m_vecTexture.clear();
	}	
}

HRESULT CTexture::LoadTexture(
	TEXTURE_TYPE eType, 
	const wstring& wstrFilePath, 	
	const WORD& wCount)
{	
	IDirect3DBaseTexture9* pTexture = nullptr;
	TCHAR szFilePath[MAX_STR] = L"";
	HRESULT hr = 0;

	for (WORD i = 0; i < wCount; ++i)
	{
		swprintf_s(szFilePath, wstrFilePath.c_str(), i);

		switch (eType)
		{
		case TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(
				m_pGraphicDev,
				szFilePath,
				(IDirect3DTexture9**)&pTexture);
			break;
		case TEXTURE_CUBE:
			hr =D3DXCreateCubeTextureFromFile(
				m_pGraphicDev,
				szFilePath,
				(IDirect3DCubeTexture9**)&pTexture);
			break;
		}		

		FAILED_CHECK_RETURN(hr, E_FAIL);
		m_vecTexture.push_back(pTexture);
	}	

	return S_OK;
}

CTexture* CTexture::Create(
	LPDIRECT3DDEVICE9 pGraphicDev,
	TEXTURE_TYPE eType, 
	const wstring& wstrFilePath, 	
	const WORD & wCount)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CTexture* pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->LoadTexture(eType, wstrFilePath, wCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
