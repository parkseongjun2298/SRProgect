#pragma once

#ifndef __TRAIN_H__
#include "GameObject.h"
class CRail;
class CCustomer;
class CTrain : public ENGINE::CGameObject
{
private:
	CTrain(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTrain();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
private:
	void	KeyInput();
	void	SetLook(Vec3 vLook, Vec3 vUp);
	void	SetUp(Vec3 vUp, Vec3 vRight);
	void	SetRight(Vec3 vRight, Vec3 vLook);
	void	MoveRail();
	void	SettingSpeed();
public:
	void	SetRailInfo(ENGINE::ROLLER_INFO tInfo);
public:
	vector<CRail*>* GetRail() { return &m_vecRail; }
	void	PushSeat(int iIdx, CCustomer* pCustomer);
public:
	static CTrain*	Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize,
		ENGINE::CGameObject* pParent);
private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;
	ENGINE::CGameObject*		m_pParent;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx;
	ENGINE::VTX_CUBETEX*		m_pConvertVtx;

	vector<CRail*>	m_vecRail;

	Vec3			m_vRight;
	Vec3			m_vUp;
	Vec3			m_vLook;
	int				m_iRailIndex;
	float			m_fSpeed;
	float			m_fMoveValue;

	bool			m_bCreateMode;
	bool			m_bAxisRotate[6];
	float			m_fCreateTime;
	int				m_iRailCount;
};

#define __TRAIN_H__
#endif