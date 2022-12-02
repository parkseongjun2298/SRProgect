#pragma once

#ifndef __SCENE_H__

#include "Layer.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CScene();

public:
	CComponent* GetComponent(
		LAYER_TYPE eLayer, 
		const wstring& wstrObjectKey, 
		const wstring& wstrComponentKey);
	list<CGameObject*>* GetObjectLst(LAYER_TYPE eLayer, const wstring& wstrObjectKey);

protected: // 순수 가상 함수
	virtual HRESULT Add_Environment_Layer() PURE;
	virtual HRESULT Add_Object_Layer() PURE;
	virtual HRESULT Add_Effect_Layer() PURE;
	virtual HRESULT Add_UI_Layer() PURE;

public:	// 가상 함수
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render();

protected:
	virtual HRESULT Initialize();
	virtual void Release();

public:
	HRESULT AddObject(LAYER_TYPE eType, const wstring& wstrObjectKey, CGameObject* pObject);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<LAYER_TYPE, CLayer*>	m_mapLayer;
	SCENE_TYPE					m_eSceneType;
};

END

#define __SCENE_H__
#endif // __SCENE_H__



