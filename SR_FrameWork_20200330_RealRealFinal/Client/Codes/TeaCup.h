#pragma once
#ifndef __TEACUP_H__

#include "GameObject.h"

class CCustomer;
class CTeaCup :
	public ENGINE::CGameObject
{
private:
	explicit CTeaCup(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTeaCup();

public:
	void SetLeftSpin(bool bCheck) { m_bIsLeftSpin = bCheck; }
	void SetRightSpin(bool bCheck) { m_bIsRightSpin = bCheck; }

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CTeaCup* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, ENGINE::CGameObject* pParent);

public:
	void SpinCup();
	void PushSeat(CCustomer* pCustomer);
	void ClearSeat(D3DXVECTOR3 vPos);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	ENGINE::CGameObject*	m_pParent;	// ºÎ¸ð°´Ã¼
	ENGINE::INFO			m_tInfo;

	float	m_fAngle;
	bool	m_bIsRightSpin;
	bool	m_bIsLeftSpin;

	CCustomer*				m_pCustomer;
};

#define __TEACUP_H__
#endif // !__TEACUP_H__
