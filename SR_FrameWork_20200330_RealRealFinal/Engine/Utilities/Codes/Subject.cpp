#include "Subject.h"
#include "Observer.h"

USING(ENGINE)

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
	m_ObserverList.clear();
}

void CSubject::Subscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);
	m_ObserverList.push_back(pObserver);
}

void CSubject::UnSubscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	auto iter_find = find(m_ObserverList.begin(), m_ObserverList.end(), pObserver);

	if (m_ObserverList.end() == iter_find)
		return;

	m_ObserverList.erase(iter_find);
}

void CSubject::Notify(int iMessage, void* pData)
{
	for (auto pObserver : m_ObserverList)
		pObserver->Update(iMessage, pData);
}

CSubject* CSubject::Create()
{
	return new CSubject;
}
