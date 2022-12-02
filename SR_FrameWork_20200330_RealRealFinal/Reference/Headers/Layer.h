#pragma once

#ifndef __LAYER_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CGameObject;
class CComponent;
class ENGINE_DLL CLayer
{
private:
	CLayer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	~CLayer();

public:
	CComponent* GetComponent(const wstring& wstrObjectKey, const wstring& wstrComponentKey);
	list<CGameObject*>* GetObjectLst(const wstring& wstrObjectKey);
	map<wstring, list<CGameObject*>>* GetMapObjectLst() { return &m_mapObject; }

public:
	HRESULT AddObject(const wstring& wstrObjectKey, CGameObject* pObject);
	void	CreateMap(const wstring& wstrObjectKey);
	int Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	typedef list<CGameObject*>	OBJECT_LIST;
	map<wstring, OBJECT_LIST>	m_mapObject;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
};

END

#define __LAYER_H__
#endif
