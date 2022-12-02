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

	vector<CMole*>			m_vecMole;		// vector�� �� ���� : �����ϰ� ȣ���ؼ� �����̰��Ϸ��� �ε��������� ������ vector���°� �� �����ϴٰ� �Ǵ���.
	D3DXVECTOR3				m_vPos;
	float					m_fTotalTime;	// �÷����� �� �ð�.
	float					m_fCoolTime;	// �δ����� �ö�� ��Ÿ��.
	float					m_fConvertTime;	// ����ð�
	float					m_fPrevTime;	// ���� �ֱ��� �δ����� �ø� �ð�.

	float					m_fRemainingTime;	// ���� �ð�.
	int						m_iCatchCount;	// �δ������� Ƚ��.
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
