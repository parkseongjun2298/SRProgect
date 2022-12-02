#pragma once
#include "GameObject.h"
class CFerrisSeat;
class CFerrisBody;
class CFerrisArm : public ENGINE::CGameObject
{
private:
	CFerrisArm(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CFerrisArm();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CFerrisArm* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, float fAngleZ, CFerrisBody* Parent);
	void RideFerrisSeat();
	CFerrisBody* Get_Body() { return m_pBody; }
	float		 GetAngleZ() { return m_fAngleZ; }
	CFerrisSeat* GetFerrisSeat() { return m_pSeat; }
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	CFerrisSeat*	m_pSeat;
	float			m_fAngleZ;
	ENGINE::INFO	m_tInfo;


	CFerrisBody*   m_pBody;
};

