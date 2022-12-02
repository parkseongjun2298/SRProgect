#pragma once

#ifndef __COLLMOUSE_H__

#include "Component.h"

class CCameraObserver;
class CCollMouse : public ENGINE::CComponent
{
private:
	explicit CCollMouse(CCameraObserver* pObserver);

public:
	virtual ~CCollMouse();

public:
	bool PickTerrain(D3DXVECTOR3* pOut, ENGINE::LAYER_TYPE eLayerType, const wstring& wstrObjectKey);
	bool PickTile(D3DXVECTOR3* pOut, ENGINE::LAYER_TYPE eLayerType, const wstring& wstrObjectKey, 
		ENGINE::VTX_TEX* pVtx, D3DXMATRIX matWorld);
	bool PickMole(ENGINE::CGameObject* pObject);
	//bool PickObject();

private:
	void TranslateLocalSpace(const D3DXMATRIX* pWorld);

public:
	static CCollMouse* Create(CCameraObserver* pObserver);

private:
	D3DXVECTOR3			m_vRayPivot;	// ±§º± √‚πﬂ¡°
	D3DXVECTOR3			m_vRayDir;		// ±§º± πÊ«‚
	
	CCameraObserver*	m_pObserver;

	ENGINE::CManagement* m_pManagement;
};

#define __COLLMOUSE_H__
#endif