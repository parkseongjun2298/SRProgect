#pragma once

#ifndef __TEACUPPLANE_H__

#include "GameObject.h"

class CCustomer;
class CTeaCup;
class CTeaCupPlane :
	public ENGINE::CGameObject
{
public:
	explicit CTeaCupPlane(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTeaCupPlane();

public:
	void SetSpinRight(bool bCheck);
	void SetSpinLeft(bool bCheck);

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void SpinStateCheck();	// 어느방향으로 돌지 체크하는 함수.
	void RidesWorks();		// 찻잔이 부모로 갖는 바닥의 움직이는 함수.
	void PushSeat(int iIndex, CCustomer* pCustomer);
	void ClearSeat(D3DXVECTOR3 vPos);
	ENGINE::CGameObject* GetParent() { return m_pParent; }
public:
	static CTeaCupPlane* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, ENGINE::CGameObject* pParent);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_COL*		m_pOriginVtx;
	ENGINE::VTX_COL*		m_pConvertVtx;

	ENGINE::CGameObject*	m_pParent;
	ENGINE::INFO			m_tInfo;

	vector<CTeaCup*>		m_vecTeaCup;
	bool					m_bSpinRight;
	bool					m_bSpinLeft;

	float					m_fAngle;
};

#define __TEACUPPLANE_H__
#endif // !__TEACUPPLANE_H__
