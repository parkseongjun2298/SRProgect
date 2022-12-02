#include "FontMgr.h"
#include "Font.h"

USING(ENGINE)

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr(void)
{

}

CFontMgr::~CFontMgr(void)
{
	Release();
}

HRESULT CFontMgr::AddFont(LPDIRECT3DDEVICE9 pDevice
	, const TCHAR* pFontKey, const INT& iHeight, const UINT& iWidth
	, const UINT& iWeight)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));
	if(iter != m_mapFont.end())
		return E_FAIL;

	CFont*	pFont = CFont::Create(pDevice, iHeight, iWidth, iWeight, pFontKey);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.emplace(pFontKey, pFont);
	return S_OK;
}

void CFontMgr::Render(const TCHAR* pFontKey
	, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));
	if(iter == m_mapFont.end())
		return;

	iter->second->Render(pString, Color, vPos);
}

void CFontMgr::Release(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

