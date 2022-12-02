#pragma once

#ifndef __CUSTOMER_H__

#include "GameObject.h"
class CCustomerUi;
class CUiStateBar;
class CCustomer : public ENGINE::CGameObject
{
private:
	explicit CCustomer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CCustomer();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	void		 Render(Matrix matTransform);

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void	MoveGoalPos();
	void	KeyInput();
	void	FindPath();
public:
	void	KeyInput(ENGINE::CGameObject* pRide, Matrix matParentWorld, Matrix matSeat);
	wstring	GetCurrentRide() { if (m_lstRideTag.empty()) return L""; return m_lstRideTag.front(); }
	void	RideSetting(Vec3 vDir = Vec3(0.f, 0.f, 1.f));

public:
	Vec3	GetDir() { return m_vDir; }
	void	SetGoalPos(list<Vec3> lstPos);

public:
	static CCustomer* Create(LPDIRECT3DDEVICE9 pGraphicDev, Vec3 vPos);

private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_TEX*			m_pOriginVtx;
	ENGINE::VTX_TEX*			m_pConvertVtx;
	CUiStateBar*				m_pStateBar;
	CCustomerUi*				m_pCustomerUi;

	list<Vec3>					m_lstGoalPos;
	Vec3						m_vDir;
	float						m_fSpeed;
	bool						m_bMove;
	list<wstring>				m_lstRideTag;

	bool						m_bUiRender;
};

#define __CUSTOMER_H__
#endif

