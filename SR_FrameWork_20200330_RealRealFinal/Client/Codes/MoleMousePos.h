#pragma once
#ifndef __MOLEMOUSEPOS_H__

#include "GameObject.h"
class CUiMoleMouse;
class CMoleMousePos :
	public ENGINE::CGameObject
{
public:
	explicit CMoleMousePos(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoleMousePos();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Update_MousePos();
	void Attack_Hammer();

public:
	//Get
	D3DXMATRIX	Get_MouseWorldMatrix();
	//Set
	void		Set_AttHammer(bool bCheck) { m_bIsHammer = bCheck; }

public:
	static CMoleMousePos* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CUiMoleMouse*	m_pUiMouse;
	bool	m_bIsHammer;
	bool	m_bIsAttack;
};

#define __MOLEMOUSEPOS_H__
#endif // !__MOLEMOUSEPOS_H__
