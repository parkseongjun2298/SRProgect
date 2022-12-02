#pragma once

#ifndef __VIKINGSHIP_H__

#include "GameObject.h"

class CCustomer;
class CVikingShip : public ENGINE::CGameObject
{
private:
	explicit CVikingShip(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CVikingShip();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	static CVikingShip* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);

private:
	void MoveViking();
public:
	void PlayViking();
	bool GetPlaying() { return m_bDirCheck; }
	void PushSeat(int iIdx, CCustomer* pCustomer);
private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*		m_pOriginVtx[4];
	ENGINE::VTX_CUBETEX*		m_pConvertVtx[4];

	ENGINE::ANGLE_TYPE	m_eAxis;	//ȸ�� ��
	float m_fAngle;//�� �����Ӵ� ȸ���� ���� 
	float m_fAngle2;
	float m_fSpeed;//�ӵ�
	float m_fPlayTime;//�ð�
	float m_fAngleTime;//�� �����Ӵ� ������ ���� 
	float m_fValue;//�����ϴ� ������

	bool m_bDirCheck;//29�ʵǸ� ���ߴ°�
};

#define __VIKINGSHIP_H__
#endif

