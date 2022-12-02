#pragma once

#ifndef __SHOOTINGPLAYER_H__

#include "GameObject.h"
class CShootingBullet;

class CShootingPlayer : public ENGINE::CGameObject
{
private:
	explicit CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CShootingPlayer();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

private:
	void KeyInput();

public:
	static CShootingPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	list<CShootingBullet*>* Get_Bullet() { return &m_lBullet; }
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
	list<CShootingBullet*>	m_lBullet;
	CCameraMgr*				m_pCameraMgr;


};

#define __SHOOTINGPLAYER_H__
#endif

