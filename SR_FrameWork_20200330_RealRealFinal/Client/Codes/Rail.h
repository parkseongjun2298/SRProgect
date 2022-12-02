#pragma once

#ifndef __RAIL_H__
#include "GameObject.h"
class CRail : public ENGINE::CGameObject
{
private:
	CRail(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRail();

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
public:
	void	SetLook(Vec3 vLook, Vec3 vUp);
	void	SetUp(Vec3 vUp, Vec3 vRight);
	void	SetRight(Vec3 vRight, Vec3 vLook);
public:
	Vec3	GetStart() { return m_vStart; }
	Vec3	GetEnd() { return m_vEnd; }
	Vec3	GetRight() { return m_vRight; }
	Vec3	GetUp() { return m_vUp; }
	Vec3	GetLook() { return m_vLook; }
	ENGINE::RAIL_INFO	GetRailInfo();
public:
	static CRail*	Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, 
		D3DXVECTOR3 vLook, D3DXVECTOR3 vUp, Vec3 vSize, ENGINE::CGameObject* pParent);
private:
	//CCamera*		m_pCamera;
	//vector<CRail*>	m_vecRail;


	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	Vec3			m_vRight;
	Vec3			m_vUp;
	Vec3			m_vLook;
	Vec3			m_vPos;
	Vec3			m_vSize;
	float			m_fTimeCount;
	int				m_iRailIndex;
	float			m_fMoveValue;
	int				m_iRailNum;
	Vec3			m_vStart;
	Vec3			m_vEnd;

	ENGINE::CGameObject*	m_pParent;
};

#define __RAIL_H__
#endif