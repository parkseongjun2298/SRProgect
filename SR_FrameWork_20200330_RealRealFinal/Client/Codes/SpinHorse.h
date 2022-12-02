#pragma once
#ifndef __SPINHORSE_H__

#include "GameObject.h"

class CSpinHorsePlane;
class CSpinHorsePillar;
class CSpinHorseHorse;
class CSpinHorse :
	public ENGINE::CGameObject
{
public:
	explicit CSpinHorse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpinHorse();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT AddComponent() override;

public:
	static CSpinHorse* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

public:
	D3DXMATRIX					GetWorldMatrix();
	const float&				GetAngle() const;
	virtual ENGINE::SAVE_INFO	GetSaveInfo() override;

public:
	void SpinHorsePlay();

private:
	vector<CSpinHorseHorse*>	m_vecHorse;
	vector<CSpinHorsePillar*>	m_vecPillar;
	vector<CSpinHorsePlane*>	m_vecPlane;

	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CKeyMgr*			m_pKeyMgr;

	//ENGINE::VTX_COL*			m_pOriginVtx;
	//ENGINE::VTX_COL*			m_pConvertVtx;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx;
	ENGINE::VTX_CUBETEX*		m_pConvertVtx;

	ENGINE::INFO				m_tInfo;
	float						m_fAngleY;
	bool						m_bIsStop;

	float						m_fAngle;

	bool						m_bPlay;
	float						m_fPlayTime;
};

#define __SPINHORSE_H__
#endif // !__SPINHORSE_H__
