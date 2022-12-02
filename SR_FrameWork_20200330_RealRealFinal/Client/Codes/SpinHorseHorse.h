#pragma once
#ifndef __SPINHORSE_HORSE_H__

#include "GameObject.h"
class CCustomer;
class CSpinHorse;
class CSpinHorseHorse :
	public ENGINE::CGameObject
{
public:
	explicit CSpinHorseHorse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpinHorseHorse();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT AddComponent() override;

public:
	static CSpinHorseHorse* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, CSpinHorse* pParent, float fAngle = 0.f);

public:
	void HorseMove();
	void PushSeat(int iIdx, CCustomer* pCustomer);
	void ClearSeat(D3DXVECTOR3 vPos);

private:
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CKeyMgr*			m_pKeyMgr;

	ENGINE::VTX_COL*			m_pOriginVtx;
	ENGINE::VTX_COL*			m_pConvertVtx;

	ENGINE::INFO				m_tInfo;
	CSpinHorse*					m_pParent;

	float						m_fSpeed;
	float						m_fAngle;

	CCustomer*					m_pCustomer;
};

#define __SPINHORSE_HORSE_H__
#endif // !__SPINHORSE_HORSE_H__
