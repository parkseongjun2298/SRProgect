#include "stdafx.h"
#include "Queue.h"
#include "Customer.h"
#include "Transform.h"
#include "FpsCamera.h"

CQueue::CQueue()
{
	m_lstCustomer = ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_OBJECT, L"Customer");
}


CQueue::~CQueue()
{
	for (auto Customer : m_lstQueue)
		ENGINE::SafeDelete(Customer);
	m_lstQueue.clear();
}

int CQueue::Update()
{
	//cout << "Queue" << m_lstQueue.size() << endl;
	if (m_lstCustomer->empty())
		return NO_EVENT;
	for (auto iter_begin = m_lstCustomer->begin(); iter_begin != m_lstCustomer->end();)
	{
		//cout << VEC3::Length((*iter_begin)->GetPos() - m_vRidePos) << endl;
		if (dynamic_cast<CCustomer*>(*iter_begin)->GetCurrentRide() == m_wstrRideTag 
			&& VEC3::Length((*iter_begin)->GetPos() - m_vRidePos) <= 15.f)
		{
			dynamic_cast<ENGINE::CTransform*>((*iter_begin)->GetComponent(L"Transform"))->SetPos({ 0.f, 0.f, 0.f });
			//cout << "Push Customer Queue" << endl;
			m_lstQueue.push_back(*iter_begin);
			if (CCameraMgr::GetInstance()->GetCurrentIdx() == 1)
				if (dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->GetSetCustomer() == (*iter_begin))
					dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->ClearCustomer();
			iter_begin = m_lstCustomer->erase(iter_begin);
		}
		else
			++iter_begin;
	}
	return NO_EVENT;
}

void CQueue::PushCustomer(ENGINE::CGameObject * pCustomer)
{
	m_lstQueue.push_back(pCustomer);
}

ENGINE::CGameObject * CQueue::PullCustomer()
{
	if (m_lstQueue.empty())
		return nullptr;
	ENGINE::CGameObject* pCustomer = m_lstQueue.front();
	m_lstQueue.pop_front();
	//cout << "Pull Customer Queue" << endl;
	return pCustomer;
}


CQueue* CQueue::Create(Vec3 vRidePos, wstring wstrRideTag)
{
	CQueue* pInstance = new CQueue;
	pInstance->m_vRidePos = vRidePos;
	pInstance->m_vRidePos.y = 0.f;
	pInstance->m_wstrRideTag = wstrRideTag;
	return pInstance;
}
