
#pragma once
#include "GameObject.h"

class CLimitTile : public ENGINE::CGameObject
{
private:
	CLimitTile(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLimitTile();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CLimitTile* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleX, float fAngleZ);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;


	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fAngleZ;
	D3DXVECTOR3		m_vTemp;

};

