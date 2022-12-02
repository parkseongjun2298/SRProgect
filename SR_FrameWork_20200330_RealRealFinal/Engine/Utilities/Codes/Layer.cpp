#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

USING(ENGINE)

CLayer::CLayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
}


CLayer::~CLayer()
{
	Release();
}

CComponent* CLayer::GetComponent(const wstring& wstrObjectKey, const wstring& wstrComponentKey)
{
	auto iter_find = m_mapObject.find(wstrObjectKey);

	if(m_mapObject.end() == iter_find)
		return nullptr;

	return iter_find->second.front()->GetComponent(wstrComponentKey);
}

list<CGameObject*>* CLayer::GetObjectLst(const wstring & wstrObjectKey)
{
	auto iter_find = m_mapObject.find(wstrObjectKey);

	if (m_mapObject.end() == iter_find)
		return nullptr;

	return &iter_find->second;
}

HRESULT CLayer::AddObject(const wstring& wstrObjectKey, CGameObject* pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_mapObject[wstrObjectKey].push_back(pObject);

	return S_OK;
}

void CLayer::CreateMap(const wstring & wstrObjectKey)
{
	m_mapObject[wstrObjectKey];
}

int CLayer::Update()
{
	for (auto& MyPair : m_mapObject)
	{
		for (auto iter = MyPair.second.begin(); iter != MyPair.second.end();)
		{
			if (DEAD_OBJ == (*iter)->Update())
			{
				SafeDelete(*iter);
				iter = MyPair.second.erase(iter);
			}
			else
				++iter;
		}
	}

	return NO_EVENT;
}

void CLayer::LateUpdate()
{
	for (auto& MyPair : m_mapObject)
	{
		for (auto& pObject : MyPair.second)
			pObject->LateUpdate();
	}
}

void CLayer::Render()
{
	for (auto& MyPair : m_mapObject)
	{
		if(MyPair.second.size() > 0)
			if(MyPair.second.front()->GetComponent(L"Texture") != nullptr)
				MyPair.second.front()->GetComponent(L"Texture")->Render(0);
		bool bAlphaTestOn = (MyPair.first == L"Customer");
		if (bAlphaTestOn)
		{
			// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 10); // 알파 기준 값 1로 설정.
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.
		}
		for (auto& pObject : MyPair.second)
			pObject->Render();
		if (bAlphaTestOn)
		{
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void CLayer::Release()
{
	for (auto& MyPair : m_mapObject)
	{
		for (auto& pObject : MyPair.second)
			SafeDelete(pObject);

		MyPair.second.clear();
	}

	m_mapObject.clear();
}

CLayer* CLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return new CLayer(pGraphicDev);
}
