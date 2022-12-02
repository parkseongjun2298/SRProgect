#include "Font.h"

USING(ENGINE)

CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{

}

CFont::~CFont(void)
{
	Release();
}

HRESULT CFont::InitFont(const INT& iHeight
	, const UINT& iWidth, const UINT& iWeight, const TCHAR* pFaceName)
{
	D3DXFONT_DESC		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Height = m_iHeight = iHeight;
	tFontInfo.Width = m_iWidth = iWidth;
	tFontInfo.Weight = iWeight;
	lstrcpy(tFontInfo.FaceName, pFaceName);
	tFontInfo.CharSet = HANGUL_CHARSET;

	//FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont));
	D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont);
	return S_OK;
}

void CFont::Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos)
{
	RECT		rc = {(long)vPos.x, (long)vPos.y
	, long(vPos.x + m_iWidth * lstrlen(pString)), long(vPos.y + m_iHeight)};
	m_pFont->DrawTextW(nullptr, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pDevice
	, const INT& iHeight, const UINT& iWidth, const UINT& iWeight
	, const TCHAR* pFaceName)
{
	CFont* pFont = new CFont(pDevice);
	if(FAILED(pFont->InitFont(iHeight, iWidth, iWeight, pFaceName)))
		SafeDelete(pFont);

	return pFont;
}

void CFont::Release(void)
{
	SafeRelease(m_pFont);
}

