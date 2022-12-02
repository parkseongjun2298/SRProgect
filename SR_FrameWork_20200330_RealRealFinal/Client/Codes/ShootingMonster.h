#pragma once

#ifndef __SHOOTINGMONSTER_H__

#include "GameObject.h"
class CEffectCountShot;
class CShootingMonster : public ENGINE::CGameObject
{
private:
	explicit CShootingMonster(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CShootingMonster();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	void MonEffectUpdate();
	void MonEffectRender();
	Vec3 Get_Pose() { return m_tInfo.vPos; }
public:
	int MoveMonster();
	float Get_Score() { return m_Score; }
	void  Set_Dead() { m_bDead = true; }
public:
	static CShootingMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos);

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	Vec3					m_vRight;
	Vec3					m_vUp;
	Vec3					m_vLook;
	ENGINE::INFO			m_tInfo;
	float m_fAngle;
	float m_fAngle2;
	float m_fSpeed;
	float m_Score;
	float m_fTime;
	bool  m_bDead;
	bool m_bIsAngry;
	CEffectCountShot*			m_pEffectCount;
};

#define __SHOOTINGMONSTER_H__
#endif

