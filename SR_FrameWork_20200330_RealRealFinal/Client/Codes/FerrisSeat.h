#pragma once
#include "GameObject.h"
class CFerrisBody;
class CFerrisArm;
class CFerrisSeat : public ENGINE::CGameObject
{
private:
	CFerrisSeat(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CFerrisSeat();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CFerrisSeat* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, CFerrisArm* Parent);
	void MoveSeat();
	void CheckDir();
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	D3DXVECTOR3				m_vPos;
	ENGINE::INFO			m_tInfo;
	CFerrisArm*				m_pArm;
};

