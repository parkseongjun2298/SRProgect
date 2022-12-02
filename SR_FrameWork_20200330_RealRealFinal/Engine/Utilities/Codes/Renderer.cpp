#include "Renderer.h"
#include "Scene.h"

USING(ENGINE)

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev),
	m_pScene(nullptr)
{
}


CRenderer::~CRenderer()
{
}

HRESULT CRenderer::SetScene(CScene* pScene)
{
	NULL_CHECK_RETURN(pScene, E_FAIL);
	m_pScene = pScene;

	return S_OK;
}

void CRenderer::Render()
{
	NULL_CHECK(m_pScene);

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();

	m_pScene->Render();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	return new CRenderer(pGraphicDev);
}
