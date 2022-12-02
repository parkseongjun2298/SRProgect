/*!
 * \file FontMgr.h
 * \date 2017/12/13 16:16
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CFont;

class ENGINE_DLL CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)

private:
	CFontMgr(void);
	~CFontMgr(void);

public:
	HRESULT AddFont(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey
		, const INT& iHeight, const UINT& iWidth, const UINT& iWeight);
	void Render(const TCHAR* pFontKey
		, const D3DXVECTOR3& vPos , const TCHAR* pString, D3DXCOLOR Color);

private:
	void Release(void);

private:
	typedef map<const TCHAR*, CFont*>		MAPFONT;
	MAPFONT		m_mapFont;
};

END

#endif // FontMgr_h__