#pragma once
#ifndef __GYRODROPBODY_H__

#include "GameObject.h"
class CGyroDropBody : public ENGINE::CGameObject
{
private:
	explicit CGyroDropBody(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CGyroDropBody();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CGyroDropBody* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);


private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx;
	ENGINE::VTX_CUBETEX*		m_pConvertVtx;
};
#define __GYRODROPBODY_H__
#endif
