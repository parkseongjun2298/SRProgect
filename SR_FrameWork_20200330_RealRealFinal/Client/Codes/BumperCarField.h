#pragma once
#ifndef __BUMPERCARFIELD_H__

#include "GameObject.h"
class CSuperCar;
class CBumperCarField :
	public ENGINE::CGameObject
{
public:
	explicit CBumperCarField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperCarField();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CBumperCarField* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0.f);

public:
	void Find_Target();
	void Check_DeadLine();
	void Check_BumperCarHp();

public:
	int	 Get_PlayTime();
	int	 Get_PlayerHp();
	int	 Get_AiCount();

private:
	vector<CSuperCar*>		m_vecBumperCar;

	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;

	ENGINE::VTX_TEX*		m_pOriginVtx;
	ENGINE::VTX_TEX*		m_pConvertVtx;

	float					m_fFindTargetTime;
	float					m_fBumperConvertTime;
	float					m_fBumperTotalTime;

	int						m_iAiCount;			// 생존해 있는 ai 자동차의 개수.

	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vLook;

};

#define __BUMPERCARFIELD_H__
#endif // !__BUMPERCARFIELD_H__
