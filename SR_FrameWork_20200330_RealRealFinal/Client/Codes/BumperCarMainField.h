#pragma once
#ifndef __BUMPERCARMAINFIELD_H__

#include "GameObject.h"
class CBumperCarMainField :
	public ENGINE::CGameObject
{
public:
	explicit CBumperCarMainField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperCarMainField();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CBumperCarMainField* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vLook;
};

#define __BUMPERCARMAINFIELD_H__
#endif // !__BUMPERCARMAINFIELD_H__
