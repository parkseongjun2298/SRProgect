#pragma once

#ifndef __ROLLERCOASTER_H__

#include "GameObject.h"

class CTrain;
class CRollerCoaster : public ENGINE::CGameObject
{
private:
	explicit CRollerCoaster(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CRollerCoaster();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	HRESULT	Initialize(ENGINE::ROLLER_INFO tInfo);
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	virtual ENGINE::SAVE_INFO GetSaveInfo() override;
	ENGINE::ROLLER_INFO GetRollerInfo();

public:
	static CRollerCoaster* Create(LPDIRECT3DDEVICE9 pGraphicDev, Vec3 vPos, Vec3 vSize, float fAngleY, bool bCreateMode = false, ENGINE::ROLLER_INFO tInfo = {});

private:
	ENGINE::CKeyMgr*		m_pKeyMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	CTrain*					m_pTrain;
	Vec3					m_vPos;
	Vec3					m_vSize;
	float					m_fAngle;
};

#define __ROLLERCOASTER_H__
#endif

