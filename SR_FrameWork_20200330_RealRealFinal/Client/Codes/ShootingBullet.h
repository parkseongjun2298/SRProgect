#pragma once

#ifndef __SHOOTINGBULLET_H__

#include "GameObject.h"

class CShootingBullet : public ENGINE::CGameObject
{
private:
	explicit CShootingBullet(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CShootingBullet();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CShootingBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vLook);
public:
	void  Set_Dead() { m_bDead = true; }
private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx;
	ENGINE::VTX_CUBETEX*		m_pConvertVtx;

	Vec3					m_vRight;
	Vec3					m_vUp;
	Vec3					m_vLook;
	float m_fAngle;
	float m_fAngle2;
	float m_fSpeed;
	float m_Score;
	float m_fTime;
	bool  m_bDead;
};

#define __SHOOTINGBULLET_H__
#endif

