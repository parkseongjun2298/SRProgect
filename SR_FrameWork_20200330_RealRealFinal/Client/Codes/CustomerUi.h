#pragma once
#ifndef __CUSTOMERUI_H__

#include "GameObject.h"
class CCustomerGage;
class CCustomer;
class CCustomerUi :
	public ENGINE::CGameObject
{
public:
	explicit CCustomerUi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomerUi();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Update_CustomerPos();
	void IsBillboarding();

public:
	//Set
	void Set_HogamdoGage(int iGage);

public:
	static CCustomerUi* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, CCustomer* pCustomer, float fAngle = 0.f);

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	CCustomer*				m_pCustomer;
	CCustomerGage*			m_pCustomerGage;

	int						m_iHogamdoGage;
	float					m_fTest;
};

#define __CUSTOMERUI_H__
#endif // !__CUSTOMERUI_H__
