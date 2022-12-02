#include "Management.h"
#include "Scene.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pGraphicDev(nullptr),
	m_pRenderer(nullptr),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_iEvent(NO_EVENT),
	m_eSceneType(SCENE_TYPE::SCENE_MAIN)
{
}


CManagement::~CManagement()
{
	Release();
}

CComponent* CManagement::GetComponent(
	LAYER_TYPE eLayer, 
	const wstring& wstrObjectKey, 
	const wstring& wstrComponentKey)
{
	NULL_CHECK_RETURN(m_mapScene[m_eSceneType], nullptr);

	return m_mapScene[m_eSceneType]->GetComponent(eLayer, wstrObjectKey, wstrComponentKey);
}

list<CGameObject*>* CManagement::GetObjectLst(LAYER_TYPE eLayer, const wstring & wstrObjectKey)
{
	NULL_CHECK_RETURN(m_mapScene[m_eSceneType], nullptr);

	return m_mapScene[m_eSceneType]->GetObjectLst(eLayer, wstrObjectKey);
}

HRESULT CManagement::InitManagement(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, E_FAIL);
	m_pGraphicDev = pGraphicDev;

	m_pRenderer = CRenderer::Create(pGraphicDev);
	NULL_CHECK_MSG_RETURN(m_pRenderer, L"Renderer Create Failed", E_FAIL);

	return S_OK;
}

void CManagement::Update()
{
	NULL_CHECK(m_mapScene[m_eSceneType]);

	m_pTimeMgr->Update();
	m_pKeyMgr->Update();	
	m_iEvent = m_mapScene[m_eSceneType]->Update();
}

void CManagement::LateUpdate()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	NULL_CHECK(m_mapScene[m_eSceneType]);
	m_mapScene[m_eSceneType]->LateUpdate();
}

void CManagement::Render()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	NULL_CHECK(m_pRenderer);
	m_pRenderer->Render();
}

void CManagement::Release()
{
	for (auto Scene : m_mapScene)
		SafeDelete(Scene.second);
	m_mapScene.clear();
	SafeDelete(m_pRenderer);

	m_pKeyMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
}

HRESULT CManagement::AddObject(LAYER_TYPE eType, const wstring & wstrObjectKey, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_mapScene[m_eSceneType]->AddObject(eType, wstrObjectKey, pObject);

	return S_OK;
}
HRESULT	CManagement::SceneReturn()
{
	auto iter = m_mapScene.find(SCENE_SUB);
	SafeDelete((*iter).second);
	m_eSceneType = SCENE_TYPE::SCENE_MAIN;
	FAILED_CHECK_RETURN(m_pRenderer->SetScene(m_mapScene[m_eSceneType]), E_FAIL);
	return S_OK;
}