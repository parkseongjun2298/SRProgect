#pragma once

#ifndef __RESOURCEMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CResources;
class ENGINE_DLL CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)

private:
	CResourceMgr();
	~CResourceMgr();

public:
	HRESULT AddBuffer(
		LPDIRECT3DDEVICE9 pGraphicDev,
		RESOURCE_TYPE eResourceType,
		BUFFER_TYPE eBufferType,
		const wstring& wstrResourceKey,
		const D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f },
		const D3DXVECTOR3 vSize = { 2.f, 2.f, 2.f },
		DWORD	dwColor = 0xffffffff,
		PIVOT_TYPE eType = PIVOT_MIDDLE,
		const WORD& wCntX = 0,
		const WORD& wCntZ = 0,
		const WORD& wItv = 1);
	HRESULT AddTexture(
		LPDIRECT3DDEVICE9 pGraphicDev,
		RESOURCE_TYPE eResourceType,
		TEXTURE_TYPE eTextureType,
		const wstring& wstrResourceKey,
		const wstring& wstrFilePath,
		const WORD& wCnt);
	CResources* CloneResources(
		RESOURCE_TYPE eResourceType, 
		const wstring& wstrResourceKey);
	void DeleteDynamicResources();
	void DeleteStaticResources();

	size_t GetTextureSize(RESOURCE_TYPE eType, wstring wstrTag);
	void ChangeSceneType(SCENE_TYPE eSceneType) 
	{ if (m_eCurrentScene != eSceneType) m_eCurrentScene = eSceneType; }

private:
	void Release();

private:
	typedef map<wstring, CResources*>	MAP_RESOURCE;
	MAP_RESOURCE m_mapResource[SCENE_TYPE::SCENE_END][RESOURCE_END];
	SCENE_TYPE	 m_eCurrentScene;
};

END

#define __RESOURCEMGR_H__
#endif

