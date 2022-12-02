#include "stdafx.h"
#include "Seat.h"
#include "CameraObserver.h"
#include "VIBuffer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Customer.h"
#include "FpsCamera.h"

CSeat::CSeat(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iCount(0)
{
}


CSeat::~CSeat()
{
	for (auto Customer : m_vecCustmer)
		ENGINE::SafeDelete(Customer);
	m_vecCustmer.clear();
}

void CSeat::SetSeatPos(int iSeatIdx, Vec3 vSeatPos)
{
	m_vecSeatPos[iSeatIdx] = vSeatPos;
}

Matrix CSeat::GetSeatPos(int iSeatIdx, Matrix matTransform)
{
	Matrix vResult, vTransform;
	vTransform = MATRIX::Translation(m_vecSeatPos[iSeatIdx]);
	vResult = vTransform * matTransform;
	return vResult;
}

void CSeat::PushCustomer(int iSeatIdx, CCustomer * pCustomer)
{
	m_vecCustmer[iSeatIdx] = pCustomer;
	//cout << "Push Customer Seat" << endl;
}

void CSeat::ClearCustomer(Vec3 vExitPos)
{
	for (auto& Customer : m_vecCustmer)
	{
		if (Customer != nullptr)
		{
			//cout << "Clear Customer Seat" << endl;
			dynamic_cast<ENGINE::CTransform*>(Customer->GetComponent(L"Transform"))->SetPos(vExitPos);
			if (CCameraMgr::GetInstance()->GetCurrentIdx() == 1)
				if(Customer == dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->GetSetCustomer())
					dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->ClearCustomer();
			ENGINE::CManagement::GetInstance()->AddObject(ENGINE::LAYER_OBJECT, L"Customer", Customer);
		}
		Customer = nullptr;
	}
}

void CSeat::KeyInput(ENGINE::CGameObject* pRide, Matrix matParentWorld)
{
	for (size_t i = 0; i < m_vecCustmer.size(); ++i)
	{
		Matrix matTransform = MATRIX::Translation(m_vecSeatPos[i]);
		if(m_vecCustmer[i] != nullptr)
			m_vecCustmer[i]->KeyInput(pRide, matParentWorld, matTransform);
	}
}

void CSeat::Render(Matrix matTransform)
{
	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 10); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.
	for (size_t i = 0; i < m_vecCustmer.size(); ++i)
	{
		Matrix matSeatPos = MATRIX::Translation(m_vecSeatPos[i]);
		if (m_vecCustmer[i] != nullptr)
		{
			m_vecCustmer[i]->Render(matSeatPos * matTransform);
			++m_iCount;
		}
	}
	//cout << "Seat " << m_iCount << endl;
	m_iCount = 0;
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CSeat* CSeat::Create(LPDIRECT3DDEVICE9 pGraphicDev, int iSeatCount)
{
	CSeat* pInstance = new CSeat(pGraphicDev);
	pInstance->m_iSeatCount = iSeatCount;
	pInstance->m_vecSeatPos.resize(pInstance->m_iSeatCount);
	pInstance->m_vecCustmer.resize(pInstance->m_iSeatCount);
	return pInstance;
}
