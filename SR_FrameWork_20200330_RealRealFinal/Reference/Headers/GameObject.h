#pragma once

#ifndef __GAMEOBJECT_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CGameObject();

public:
	CComponent* GetComponent(const wstring& wstrComponentKey);
	virtual D3DXVECTOR3 GetPos();
	virtual ENGINE::SAVE_INFO GetSaveInfo();
	D3DXMATRIX	GetWorldMatrix();
	D3DXMATRIX  GetResultWorldMatrix() { return m_matResultWorld; }
	RIDE_TYPE	GetRideType() { return m_eRideType; }
public:

protected:
	void	SetCreatePosSize(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);
	void	SetResultWorldMatrix(D3DXMATRIX matWorldResult) { m_matResultWorld = matWorldResult; }
	// 순수 가상 함수
protected:
	virtual HRESULT AddComponent() PURE;

	// 가상 함수
public:
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render();

private:
	virtual HRESULT Initialize();
	virtual void Release();

protected:
	map<wstring, CComponent*>	m_mapComponent;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DXMATRIX					m_matResultWorld;
	RIDE_TYPE					m_eRideType;
};

END

#define __GAMEOBJECT_H__
#endif

