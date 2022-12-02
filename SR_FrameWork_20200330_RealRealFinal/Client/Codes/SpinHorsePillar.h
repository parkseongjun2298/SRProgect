#pragma once
#ifndef __SPINHORSE_PILLAR_H__

#include "GameObject.h"

class CSpinHorse;
class CSpinHorsePillar :
	public ENGINE::CGameObject
{
public:
	explicit CSpinHorsePillar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpinHorsePillar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT AddComponent() override;

public:
	static CSpinHorsePillar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, CSpinHorse* pParent);

public:
	void HeightCheck();

private:
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CKeyMgr*			m_pKeyMgr;

	ENGINE::VTX_COL*		m_pOriginVtx;
	ENGINE::VTX_COL*		m_pConvertVtx;

	ENGINE::INFO				m_tInfo;
	CSpinHorse*					m_pParent;

	float						m_fSpeed;
};

#define __SPINHORSE_PILLAR_H__
#endif // !__SPINHORSE_PILLAR_H__
