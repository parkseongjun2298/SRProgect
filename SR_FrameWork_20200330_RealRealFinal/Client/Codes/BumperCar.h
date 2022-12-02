#pragma once
#ifndef __BUMPERCAR_H__

#include "GameObject.h"
class CBumperLife;
class CBumperStart;
class CBumperEnd;
class CBumperCarMainField;
class CBumperCarField;
class CBumperCar :
	public ENGINE::CGameObject
{
public:
	explicit CBumperCar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperCar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Check_Life();
	void KeyInput();
	void Check_Ready();
	void Check_GameOver();

public:
	static CBumperCar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);

private:
	CBumperCarField*		m_pField;
	CBumperCarMainField*	m_pMainField;
	list<CBumperLife*>		m_listBumperLife;	// 플레이어 생명 UI
	CBumperStart*			m_pUiStart;
	CBumperEnd*				m_pUiEnd;
	bool					m_bIsStart;
	bool					m_bIsReady;
	bool					m_bIsUiStart;

	bool					m_bIsGameOver;
};

#define __BUMPERCAR_H__
#endif // !__BUMPERCAR_H__
