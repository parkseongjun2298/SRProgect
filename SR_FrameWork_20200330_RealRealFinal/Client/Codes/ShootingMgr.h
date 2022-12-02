#pragma once

#ifndef __SHOOTINGMGR_H__
#include "GameObject.h"

class CShootingPlayer;
class CShootingMonster;
class CShootingCount;
class CShootingTime;
class CShootingStart;
class CShootingOver;
class CCrossHair;
class CShootingGun;
class CShootingEffect;
class CShootingMgr :public ENGINE::CGameObject
{
private:
	explicit CShootingMgr(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CShootingMgr();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CShootingMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0);
	void Start_Game();


private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	CShootingPlayer*			m_pPlayer;
	list<CShootingMonster*>		m_lMonster;
	CShootingCount*				m_pShootingCount;
	CShootingTime*				m_pShootingTime;
	CShootingStart*				m_pShootingStart;
	CShootingOver*				m_pShootingOver;
	CCrossHair*					m_pCrossHair;
	CShootingGun*				m_pShootingGun;
	CShootingEffect*			m_pShootingEffect;
	ENGINE::INFO				m_tInfo;
	float						m_fTime;
	int							m_iScore;

	bool						m_bStart;
};
#define __SHOOTINGMGR_H__
#endif
