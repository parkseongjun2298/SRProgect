#include "stdafx.h"
#include "MainApp.h"
#include "SceneSelector.h"
#include "SoundMgr.h"		// Ãß°¡.

CMainApp::CMainApp()
	: m_pDeviceMgr(ENGINE::CDeviceMgr::GetInstance()),
	m_pManagement(ENGINE::CManagement::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance()),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance())
{
}


CMainApp::~CMainApp()
{
	Release();
}

void CMainApp::Update()
{	
	m_pManagement->Update();
}

void CMainApp::LateUpdate()
{
	m_pManagement->LateUpdate();
}

void CMainApp::Render()
{
	m_pManagement->Render();

	//FPSÃâ·Â¿ë
	//m_pDeviceMgr->GetDevice()->BeginScene();
	//
	//RenderFPS();
	//
	//m_pDeviceMgr->GetDevice()->EndScene();
	//m_pDeviceMgr->GetDevice()->Present(nullptr, nullptr, nullptr, nullptr);
}

HRESULT CMainApp::Initialize()
{
	HRESULT hr = m_pDeviceMgr->InitDevice(g_hWnd, WINCX, WINCY, ENGINE::MODE_FULL);
	FAILED_CHECK_MSG_RETURN(hr, L"InitDevice Failed", E_FAIL);

	hr = m_pManagement->InitManagement(m_pDeviceMgr->GetDevice());
	FAILED_CHECK_MSG_RETURN(hr, L"InitManagement Failed", E_FAIL);
	
	hr = m_pManagement->SceneChange(CSceneSelector(SCENE_END, SCENE_LOGO), ENGINE::SCENE_MAIN);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Change Failed", E_FAIL);

	hr = m_pFontMgr->AddFont(m_pDeviceMgr->GetDevice(), L"¹ÙÅÁ", 30, 18, FW_NORMAL);
	hr = m_pFontMgr->AddFont(m_pDeviceMgr->GetDevice(), L"±Ã¼­", 20, 13, FW_THIN);
	hr = m_pFontMgr->AddFont(m_pDeviceMgr->GetDevice(), L"µÕ±Ù¸ð²Ã", 30, 18, FW_NORMAL);
	hr = m_pFontMgr->AddFont(m_pDeviceMgr->GetDevice(), L"µÕ±Ù¸ð²ÃµÎ²¨¿ò", 50, 36, FW_HEAVY);
	m_pSoundMgr->Initialize();

	return S_OK;
}

void CMainApp::Release()
{	
	m_pFontMgr->DestroyInstance();
	CUiMgr::GetInstance()->DestroyInstance();
	CEventMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	m_pSoundMgr->DestroyInstance();
	m_pManagement->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	m_pSubjectMgr->DestroyInstance();
	m_pResourceMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

void CMainApp::RenderFPS()
{
	++m_dwCount;

	m_fTime += ENGINE::CTimeMgr::GetInstance()->GetDeltaTime();

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFps, L"FPS : %d", m_dwCount);
		m_fTime = 0.f;
		m_dwCount = 0;
	}
	m_pFontMgr->Render(L"±Ã¼­", D3DXVECTOR3(0.f, 0.f, 0.f), m_szFps, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}