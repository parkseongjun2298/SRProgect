#pragma once

#ifndef __MONSTER_H__

#include "GameObject.h"

class CMonster : public ENGINE::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CMonster();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_COL*		m_pOriginVtx;
	ENGINE::VTX_COL*		m_pConvertVtx;
};

#define __MONSTER_H__
#endif

