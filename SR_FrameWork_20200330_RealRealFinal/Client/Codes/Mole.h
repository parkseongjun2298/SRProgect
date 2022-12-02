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
	float	m_fStandTime;	// �ö�� �ð�.

	float	m_fStandHeight;	// �ö�� ����.
	float	m_fSitHeight;	// ������ ����.

	bool	m_bIsPlay;
	bool	m_bIsHit;
	bool	m_bIsAngry;

	bool	m_bIsSound;		// �δ����ö�ö� �Ҹ�.

	CEffectCount*			m_pEffectCount;
};

#define __MOLE_H__
#endif // !__MOLE_H__
