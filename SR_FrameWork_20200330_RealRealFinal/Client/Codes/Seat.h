#pragma once

#ifndef __SEAT_H__

#include "Component.h"

class CCustomer;
class CSeat : public ENGINE::CComponent
{
private:
	explicit CSeat(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CSeat();

public:
	void	SetSeatPos(int iSeatIdx, Vec3 vSeatPos);
	Matrix	GetSeatPos(int iSeatIdx, Matrix matTransform);
	void	PushCustomer(int iSeatIdx, CCustomer* pCustomer);
	void	ClearCustomer(Vec3 vExitPos);
	void	KeyInput(ENGINE::CGameObject* pRide, Matrix matParentWorld);
	void	Render(Matrix matTransform);

public:
	static CSeat* Create(LPDIRECT3DDEVICE9 pGraphicDev, int iSeatCount);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	vector<Vec3>		m_vecSeatPos;
	vector<CCustomer*>	m_vecCustmer;
	int					m_iSeatCount;
	int					m_iCount;
};

#define __SEAT_H__
#endif