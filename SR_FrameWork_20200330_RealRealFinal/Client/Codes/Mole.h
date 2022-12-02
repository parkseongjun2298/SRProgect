#pragma once
#ifndef __MOLE_H__

#include "GameObject.h"
class CEffectCount;
class CMole :
	public ENGINE::CGameObject
{
private:
	explicit CMole(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CMole();

public:
	// Set
	void SetMolePlay(bool bCheck);
	void SetMoleHit(bool bCheck);
	void SetMoleAngry(bool bCheck);

public:
	// Get
	bool GetMolePlay();
	bool GetMoleHit();
	bool GetMoleAngry();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void MolePlay();
	void MoleHit();
	void MoleEffectUpdate();
	void MoleEffectRender();

public:
	static CMole* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	D3DXVECTOR3				m_vPos;

	float	m_fAngle;
	float	m_fStandTime;	// 올라온 시간.

	float	m_fStandHeight;	// 올라온 높이.
	float	m_fSitHeight;	// 내려간 높이.

	bool	m_bIsPlay;
	bool	m_bIsHit;
	bool	m_bIsAngry;

	bool	m_bIsSound;		// 두더지올라올때 소리.

	CEffectCount*			m_pEffectCount;
};

#define __MOLE_H__
#endif // !__MOLE_H__
