
#pragma once

#ifndef __GYROSWINGDEVICE_H__

#include "GameObject.h"

class CGyroSwingDevice : public ENGINE::CGameObject
{
private:
	CGyroSwingDevice(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGyroSwingDevice();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CGyroSwingDevice* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY);
public:
	D3DXMATRIX Get_Matrix();// { return m_Matworld; }
	int MoveDevice();
	void PlayGyroSwing();

	bool Get_FinCheck() { return m_bFinCheck; }
private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	ENGINE::ANGLE_TYPE	m_eAxis;	//ȸ�� ��
	float m_fAngle;//�� �����Ӵ� ȸ���� ���� 
	float m_fAngle2;
	float m_fSpeed;//�ӵ�
	float m_fPlayTime;//�ð�
	float m_fAngleTime;//�� �����Ӵ� ������ ���� 
	float m_fValue;//�����ϴ� ������

	bool m_bDirCheck;//29�ʵǸ� ���ߴ°�

	float			m_fAngleX;
	D3DXVECTOR3		m_vTemp;
	D3DXMATRIX      m_Matworld;

	bool			m_bFinCheck;
	float			m_fTime;
};

#define __GYROSWINGDEVICE_H__
#endif