#pragma once
#ifndef __EFFECTPLUSONE_H__

#include "GameObject.h"
class CEffectCount :
	public ENGINE::CGameObject
{
private:
	explicit CEffectCount(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CEffectCount();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	//Get
	bool	Get_RenderState();
	//Set
	void	Set_RenderState(bool bCheck);
	void	Set_Pos(D3DXVECTOR3 vPos);
	void	Set_Minus(bool bCheck);

public:
	static CEffectCount* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::VTX_TEX*	m_pOriginVtx;
	ENGINE::VTX_TEX*	m_pConvertVtx;

	float					m_fLifeTime;

	D3DXVECTOR3				m_vPos;

	bool					m_bIsRender;
	bool					m_bIsMius;
};

#define __EFFECTPLUSONE_H__
#endif // !__EFFECTPLUSONE_H__
