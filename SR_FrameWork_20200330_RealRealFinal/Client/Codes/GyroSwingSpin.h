
#pragma once
#ifndef __GYROSWINGSPIN_H__
#include "GameObject.h"
class CCustomer;
class CGyroSwingDevice;
class CGyroSwingSpin : public ENGINE::CGameObject
{
private:
	CGyroSwingSpin(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGyroSwingSpin();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	static CGyroSwingSpin* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY, CGyroSwingDevice* Parent);
public:
	void PushSeat(int iIdx, CCustomer* pCustomer);

private:
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CResourceMgr*	m_pResourceMgr;
	ENGINE::CSubjectMgr*	m_pSubjectMgr;

	ENGINE::VTX_CUBETEX*	m_pOriginVtx;
	ENGINE::VTX_CUBETEX*	m_pConvertVtx;

	D3DXMATRIX				m_Matworld;
	ENGINE::INFO			m_tInfo;
	CGyroSwingDevice*       m_MParent;
};

#define __GYROSWINGSPIN_H__
#endif