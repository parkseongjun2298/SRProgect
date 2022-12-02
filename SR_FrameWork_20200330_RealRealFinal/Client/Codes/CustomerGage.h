#pragma once
#ifndef __CUSTOMERGAGE_H__

#include "GameObject.h"
class CCustomerGage :
	public ENGINE::CGameObject
{
public:
	explicit CCustomerGage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomerGage();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Set_Size(D3DXVECTOR3 vSize);
	void Set_Pos(D3DXVECTOR3 vPos);
	void IsBillboarding();

public:
	static CCustomerGage* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	D3DXVECTOR3				m_vUiPos;
};

#define __CUSTOMERGAGE_H__
#endif // !__CUSTOMERGAGE_H__
