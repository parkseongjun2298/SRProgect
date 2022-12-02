#include "ResourceMgr.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainCol.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "Texture.h"
#include "RcTexLying.h"
#include "CubeCol.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CResourceMgr)

CResourceMgr::CResourceMgr()
{
}


CResourceMgr::~CResourceMgr()
{
	Release();
}

HRESULT CResourceMgr::AddBuffer(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	RESOURCE_TYPE eResourceType, 
	BUFFER_TYPE eBufferType, 
	const wstring& wstrResourceKey, 
	const D3DXVECTOR3 vPos,
	const D3DXVECTOR3 vSize,
	DWORD	dwColor,
	PIVOT_TYPE eType,
	const WORD& wCntX, 
	const WORD& wCntZ, 
	const WORD& wItv)
{
	NULL_CHECK_RETURN(pGraphicDev, E_FAIL);

	auto iter_find = m_mapResource[m_eCurrentScene][eResourceType].find(wstrResourceKey);

	if (m_mapResource[m_eCurrentScene][eResourceType].end() != iter_find)
		return E_FAIL;

	CResources* pResource = nullptr;

	switch (eBufferType)
	{
	case BUFFER_RCCOL:
		pResource = CRcCol::Create(pGraphicDev);
		break;
	case BUFFER_RCTEX:
		pResource = CRcTex::Create(pGraphicDev, vPos, vSize, eType);
		break;
	case BUFFER_TERRAINCOL:
		pResource = CTerrainCol::Create(pGraphicDev, wCntX, wCntZ, wItv);
		break;
	case BUFFER_TERRAINTEX:
		pResource = CTerrainTex::Create(pGraphicDev, wCntX, wCntZ, wItv);
		break;
	case BUFFER_CUBETEX:
		pResource = CCubeTex::Create(pGraphicDev, vPos, vSize, eType);
		break;
	case BUFFER_RCTEXLYING:
		pResource = CRcTexLying::Create(pGraphicDev, vPos, vSize, eType);
		break;
	case BUFFER_CUBECOL:
		pResource = CCubeCol::Create(pGraphicDev, vPos, vSize, dwColor, eType);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_mapResource[m_eCurrentScene][eResourceType].insert(make_pair(wstrResourceKey, pResource));

	return S_OK;
}

HRESULT CResourceMgr::AddTexture(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	RESOURCE_TYPE eResourceType, 
	TEXTURE_TYPE eTextureType, 
	const wstring& wstrResourceKey, 
	const wstring& wstrFilePath, 
	const WORD& wCnt)
{
	NULL_CHECK_RETURN(pGraphicDev, E_FAIL);

	auto iter_find = m_mapResource[m_eCurrentScene][eResourceType].find(wstrResourceKey);

	if (m_mapResource[m_eCurrentScene][eResourceType].end() != iter_find)
		return E_FAIL;

	CResources* pResource = CTexture::Create(pGraphicDev, eTextureType, wstrFilePath, wCnt);
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[m_eCurrentScene][eResourceType].insert(make_pair(wstrResourceKey, pResource));

	return S_OK;
}

CResources* CResourceMgr::CloneResources(
	RESOURCE_TYPE eResourceType, 
	const wstring& wstrResourceKey)
{
	auto iter_find = m_mapResource[m_eCurrentScene][eResourceType].find(wstrResourceKey);

	if(m_mapResource[m_eCurrentScene][eResourceType].end() == iter_find)
		return nullptr;

	return iter_find->second->Clone();
}

void CResourceMgr::DeleteDynamicResources()
{
	for (auto& MyPair : m_mapResource[m_eCurrentScene][RESOURCE_DYNAMIC])
		SafeDelete(MyPair.second);

	m_mapResource[m_eCurrentScene][RESOURCE_DYNAMIC].clear();
}

void CResourceMgr::DeleteStaticResources()
{
	for (auto& MyPair : m_mapResource[m_eCurrentScene][RESOURCE_STATIC])
		SafeDelete(MyPair.second);

	m_mapResource[m_eCurrentScene][RESOURCE_STATIC].clear();
}

size_t CResourceMgr::GetTextureSize(RESOURCE_TYPE eType, wstring wstrTag)
{
	auto iter_find = m_mapResource[m_eCurrentScene][eType].find(wstrTag);

	if (m_mapResource[m_eCurrentScene][eType].end() == iter_find)
		return -1;
	return dynamic_cast<CTexture*>((*iter_find).second)->GetSize();
}

void CResourceMgr::Release()
{
	for (int j = 0; j < SCENE_END; ++j)
	{
		for (int i = 0; i < RESOURCE_END; ++i)
		{
			for (auto& MyPair : m_mapResource[j][i])
				SafeDelete(MyPair.second);

			m_mapResource[j][i].clear();
		}
	}
}
