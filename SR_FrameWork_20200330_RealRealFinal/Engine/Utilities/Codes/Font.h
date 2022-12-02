/*!
 * \file Font.h
 * \date 2017/12/13 15:51
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


#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CFont
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);

public:
	~CFont(void);

public:
	HRESULT InitFont(const INT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);
	void Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice
		, const INT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	LPD3DXFONT				m_pFont = nullptr;
	INT						m_iHeight = 0;
	UINT					m_iWidth = 0;
};

END

#endif // Font_h__