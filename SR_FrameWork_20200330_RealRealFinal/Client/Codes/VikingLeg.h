#pragma once
#ifndef __VIKINGLEG_H__
#include "GameObject.h"
class CVikingLeg : public ENGINE::CGameObject
{
private:
	CVikingLeg(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CVikingLeg();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CVikingLeg* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleX);// , float fAngle);
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	float					m_fAngle;
};

#define __VIKINGLEG_H__
#endif