
#pragma once
#include "GameObject.h"

class CGyroSwingLeg : public ENGINE::CGameObject
{
private:
	CGyroSwingLeg(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGyroSwingLeg();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CGyroSwingLeg* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleX, float fAngleZ);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;


	float			m_fAngleX;
	float			m_fAngleZ;
	D3DXVECTOR3		m_vTemp;

};

