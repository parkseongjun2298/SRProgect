#pragma once

#ifndef __MOLECATCH_H__

#include "GameObject.h"

class CMoleMousePos;
class CUiMoleStart;
class CUiMoleEnd;
class CUiMoleTime;
class CUiMoleCount;
class CMole;
class CMoleCatch :
	public ENGINE::CGameObject
{
private:
	explicit CMoleCatch(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CMoleCatch();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CMoleCatch* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

public:
	void MoleCatchPlay();
	bool MoleAngry();
	void KeyInput();

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CKeyMgr*		m_pKeyMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	vector<CMole*>			m_vecMole;		// vector로 둔 이유 : 랜덤하게 호출해서 움직이게하려면 인덱스접근이 가능한 vector형태가 젤 적합하다고 판단함.
	D3DXVECTOR3				m_vPos;
	float					m_fTotalTime;	// 플레이할 총 시간.
	float					m_fCoolTime;	// 두더지가 올라올 쿨타임.
	float					m_fConvertTime;	// 현재시간
	float					m_fPrevTime;	// 가장 최근의 두더지를 올린 시간.

	float					m_fRemainingTime;	// 남은 시간.
	int						m_iCatchCount;	// 두더지잡은 횟수.
	CUiMoleCount*			m_pUiMoleCount;
	CUiMoleTime*			m_pUiMoleTime;

	CUiMoleStart*			m_pUiMoleStart;
	bool					m_bRenderStart;
	CUiMoleEnd*				m_pUiMoleEnd;
	bool					m_bRenderEnd;
	float					m_fRenderWaitTime;

	CMoleMousePos*			m_pMoleMousePos;
};


#define __MOLECATCH_H__
#endif // !__MOLECATCH_H__
