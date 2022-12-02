#pragma once
#ifndef __SPINTEACUP_H__

#include "GameObject.h"

class CTeaCupPlane;
class CSpinTeaCup :
	public ENGINE::CGameObject
{
private:
	explicit CSpinTeaCup(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CSpinTeaCup();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CSpinTeaCup* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

public:
	void RidesWorks();	// 놀이기구 작동함수
	void KeyInput();
	void PushCustomer();

public:
	virtual ENGINE::SAVE_INFO	GetSaveInfo() override;

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;
	ENGINE::INFO			m_tInfo;
	float					m_fAngleY;

	vector<CTeaCupPlane*>	m_vecTeaCupPlane;
	float					m_fWorkTime;	// 작동시간
	float					m_fPlayTime;
	bool					m_bPlay;
};

#define __SPINTEACUP_H__
#endif // !__SPINTEACUP_H__
