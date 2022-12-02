#pragma once
#ifndef __SUPERCAR_H__

#include "GameObject.h"
class CSuperCar :
	public ENGINE::CGameObject
{
protected:
	explicit CSuperCar(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CSuperCar();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void Accel();	// 가속
	void Decel();	// 감속
	bool ReBound();
	void OutAnimation();

public:
	//Get
	const D3DXVECTOR3&	Get_Look()const;
	const D3DXVECTOR3&	Get_Pos();
	const float&		Get_Speed()const;
	const float&		Get_Size()const;
	const int&			Get_Hp()const;
	
public:
	//Set
	void Set_Target(CSuperCar* pTarget);
	void Set_TextureIndex(int iIndex);
	void Set_ReBoundDir(D3DXVECTOR3 vDir);
	void Set_ReBoundDistance(float fDistance);
	void Set_Accel(bool bCheck);
	void Set_Hp(int iHp);
	void Set_HpDecrease();
	void Set_Pos(D3DXVECTOR3 vPos);
	void Set_OutState(bool bCheck);

public:
	static CSuperCar* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

protected:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vLook;

	D3DXVECTOR3				m_vReBoundDir;
	float					m_fReBoundDistance;
	float					m_fReBoundTime;

	float					m_fSpeed;			// 
	float					m_fAccelSpeed;		// 
	float					m_fMaxSpeed;		// 최대치

	float					m_fSize;			// 충돌을 위한 범퍼카의 크기.

	int						m_iHp;				// 생명력.

	CSuperCar*				m_pTarget;
	int						m_iTextureIndex;	// 나중에 Ai 자동차들 색깔.

	bool					m_bIsAccel;			// 가속중 상태
	bool					m_bIsOut;			// 탈락 상태
};

#define __SUPERCAR_H__
#endif // !__SUPERCAR_H__
