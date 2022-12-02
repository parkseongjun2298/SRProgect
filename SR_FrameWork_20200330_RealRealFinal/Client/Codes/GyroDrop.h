#pragma once
#ifndef __GYRODROP_H__

#include "GameObject.h"
class CGyroDropBody;
class CGyroDropDevice;
class CGyroDrop : public ENGINE::CGameObject
{
private:
	explicit CGyroDrop(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CGyroDrop();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;
public:
	virtual D3DXVECTOR3 GetPos() override;
	virtual ENGINE::SAVE_INFO GetSaveInfo() override;
public:
	static CGyroDrop* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngle = 0);


private:
	ENGINE::CKeyMgr*			m_pKeyMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourceMgr*		m_pResourceMgr;
	ENGINE::CSubjectMgr*		m_pSubjectMgr;
	ENGINE::INFO				m_tInfo;

	vector<CGyroDropDevice*>	m_pVecGyroDevice;
	CGyroDropBody*				m_pGyroDropBody;
	float						m_fAngleY;
	bool						m_bPlay;

	float Temp;
};
#define __GYRODROP_H__
#endif
