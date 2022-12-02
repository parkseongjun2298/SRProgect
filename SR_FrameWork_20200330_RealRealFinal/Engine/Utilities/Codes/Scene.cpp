#include "Scene.h"

USING(ENGINE)

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_eSceneType(SCENE_TYPE::SCENE_END)
{
}


CScene::~CScene()
{
	Release();
}

CComponent* CScene::GetComponent(
	LAYER_TYPE eLayer, 
	const wstring& wstrObjectKey, 
	const wstring& wstrComponentKey)
{
	auto iter_find = m_mapLayer.find(eLayer);

	if(m_mapLayer.end() == iter_find)
		return nullptr;

	return iter_find->second->GetComponent(wstrObjectKey, wstrComponentKey);
}

list<CGameObject*>* CScene::GetObjectLst(LAYER_TYPE eLayer, const wstring & wstrObjectKey)
{
	auto iter_find = m_mapLayer.find(eLayer);

	if (m_mapLayer.end() == iter_find)
		return nullptr;

	return iter_find->second->GetObjectLst(wstrObjectKey);
}

int CScene::Update()
{
	for (auto& MyPair : m_mapLayer)
		MyPair.second->Update();

	return NO_EVENT;
}

void CScene::LateUpdate()
{
	for (auto& MyPair : m_mapLayer)
		MyPair.second->LateUpdate();
}

void CScene::Render()
{
	for (auto& MyPair : m_mapLayer)
		MyPair.second->Render();
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

void CScene::Release()
{
	for (auto& MyPair : m_mapLayer)
		SafeDelete(MyPair.second);

	m_mapLayer.clear();
}

HRESULT CScene::AddObject(LAYER_TYPE eType, const wstring & wstrObjectKey, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_mapLayer[eType]->AddObject(wstrObjectKey, pObject);

	return S_OK;
}
