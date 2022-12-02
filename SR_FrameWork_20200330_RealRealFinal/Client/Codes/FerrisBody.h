#pragma once
#include "GameObject.h"
class CFerrisBody : public ENGINE::CGameObject
{
private:
	CFerrisBody(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CFerrisBody();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CFerrisBody* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);
	D3DXMATRIX Get_Matrix() { return m_Matworld; }
	void MoveBody();
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	D3DXMATRIX				m_Matworld;
};

