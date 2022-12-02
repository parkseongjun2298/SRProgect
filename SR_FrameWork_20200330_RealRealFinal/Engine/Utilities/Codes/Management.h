#pragma once

#ifndef __MANAGEMENT_H__

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Renderer.h"

BEGIN(ENGINE)

class CGameObject;
class CComponent;
class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	~CManagement();

public:
	CComponent* GetComponent(
		LAYER_TYPE eLayer,
		const wstring& wstrObjectKey,
		const wstring& wstrComponentKey);
	list<CGameObject*>* GetObjectLst(LAYER_TYPE eLayer, const wstring& wstrObjectKey);

public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pGraphicDev);
	void Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	HRESULT AddObject(LAYER_TYPE eType, const wstring& wstrObjectKey, CGameObject* pObject);

public:
	template <typename T>
	HRESULT SceneChange(T& functor, SCENE_TYPE eType)
	{
		FAILED_CHECK_RETURN(functor(m_pGraphicDev, &m_mapScene[eType]), E_FAIL);
		FAILED_CHECK_RETURN(m_pRenderer->SetScene(m_mapScene[eType]), E_FAIL);
		m_eSceneType = eType;

		return S_OK;
	}
	HRESULT	SceneReturn();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<SCENE_TYPE, CScene*>	m_mapScene;
	CRenderer*					m_pRenderer;
	CKeyMgr*					m_pKeyMgr;
	CTimeMgr*					m_pTimeMgr;

	int							m_iEvent;
	SCENE_TYPE					m_eSceneType;
};

END

#define __MANAGEMENT_H__
#endif