#pragma once

#ifndef __RENDERER_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CScene;
class ENGINE_DLL CRenderer // ·»´õ¸µ ÃÑ°ý Å¬·¡½º
{
public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	~CRenderer();

public:
	HRESULT SetScene(CScene* pScene);
	void Render();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CScene*				m_pScene;
};

END

#define __RENDERER_H__
#endif