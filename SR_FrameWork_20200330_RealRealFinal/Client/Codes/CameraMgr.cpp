#include "stdafx.h"
#include "CameraMgr.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "FpsCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
	: m_pKeyMgr(ENGINE::CKeyMgr::GetInstance())
{
}


CCameraMgr::~CCameraMgr()
{
	for (int i = 0; i < ENGINE::SCENE_TYPE::SCENE_END; ++i)
	{
		for (auto Camera : m_vecCamera[i])
			ENGINE::SafeDelete(Camera);
		m_vecCamera[i].clear();
	}
}

void CCameraMgr::PushCamera(ENGINE::CCamera* pCamera)
{
	m_vecCamera[m_eCurrentScene].push_back(pCamera);
}

void CCameraMgr::ChangeSceneCamera(ENGINE::SCENE_TYPE eSceneType, bool bClear)
{
	if (m_eCurrentScene == eSceneType)
		return;

	if (bClear)
	{
		for (auto iter : m_vecCamera[m_eCurrentScene])
			ENGINE::SafeDelete(iter);
		m_vecCamera[m_eCurrentScene].clear();
	}

	m_eCurrentScene = eSceneType;
}

HRESULT CCameraMgr::Initiailize()
{
	m_vecCamera[ENGINE::SCENE_TYPE::SCENE_MAIN].push_back(CDynamicCamera::Create(m_pGraphicDev, { 0.f, 28.f, 0.f }, {0.f, 0.f, 14.f}));
	m_vecCamera[ENGINE::SCENE_TYPE::SCENE_MAIN].push_back(CFpsCamera::Create(m_pGraphicDev));
	return S_OK;
}

int CCameraMgr::Update()
{
	return m_vecCamera[m_eCurrentScene][m_iIndex]->Update();
}
